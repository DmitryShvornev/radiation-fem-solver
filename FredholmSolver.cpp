#include "FredholmSolver.h"
// постоянная Стефана - Больцмана
double SIGMA_0 = 5.67 * pow(10, -8);

// излучательная способность
double EPSILON = 0.7;

// температура
double THETA = 573;

double pi = 3.14159265358979323846;

FredholmSolver::FredholmSolver(const Mesh p_mesh) {
    mesh = p_mesh;
}

std::vector<std::vector<double>> FredholmSolver::createLocalMatrix(Element p_element) {
    Point3D center = p_element.getCenter();
    double square = p_element.getSquare();
    double weight = 0;
    std::for_each(mesh.elements.begin(), mesh.elements.end(), [&](Element element) {
        if (element != p_element) {
            Point3D target_center = element.getCenter();
            double target_square = element.getSquare();
            Point3D r = target_center - center;
            double k_N = r * target_center;
            double k_M = r * center;
            double distance = target_center.getDistance(center);
            weight += k_M * k_N * target_square / (distance * distance);
        }
    });
    weight *= square / (12 * pi);
    std::vector<std::vector<double>> M(3, std::vector<double>(3));
    for (std::size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            M[i][j] = (1 - EPSILON) * weight - (1 / 12) * square;
        }
    }
    return M;
}

std::vector<double> FredholmSolver::createLocalVector(Element p_element) {
    double element = (1 / 3) * SIGMA_0 * EPSILON * pow(THETA, 4) * p_element.getSquare();
    std::vector<double> res({ element, element, element });
    return res;
}

void FredholmSolver::assembleGlobalSystem() {
    const int SIZE = mesh.nodes.size();
    std::vector<std::vector<double>> G(SIZE, std::vector<double>(SIZE));
    for (auto& row : G) {
        for (auto& col : row) {
            col = 0;
        }
    }
    std::vector<double> H(SIZE);
    std::fill(H.begin(), H.end(), 0);
    std::for_each(mesh.elements.begin(), mesh.elements.end(), [&](Element element) {
        std::vector<std::size_t> IDs({ element.i_globalID, element.j_globalID, element.k_globalID });
        std::vector<std::vector<double>> M = createLocalMatrix(element);
        std::vector<double> F = createLocalVector(element);
        for (size_t i = 0; i < 3; i++)
        {
            std::size_t i_global = IDs[i];
            H[i_global] += F[i];
            for (size_t j = 0; j < 3; j++)
            {
                std::size_t j_global = IDs[j];
                G[i_global][j_global] += M[i][j];
            }
        }
    });
    global_vector = H;
    global_matrix = G;
}

void FredholmSolver::solveGlobalSystem() {
    for (auto& row : global_matrix) {
        for (auto& col : row) {
            std::cout << col << " ";
        }
    }
}

void FredholmSolver::printToMV2() {
    std::ofstream out("result.mv2");
    int nodes_size = mesh.nodes.size();
    int elements_size = mesh.elements.size();
    out << nodes_size << " 3 1 q" << std::endl;
    for (int i = 0; i < nodes_size; ++i)
    {
        out << (i + 1) << " " << mesh.nodes[i].x << " "
            << mesh.nodes[i].y << " " << mesh.nodes[i].z << " "
            << solution[i] << std::endl;
    }

    out << elements_size << " 3 3 BC_id mat_id mat_id_Out" << std::endl;
    for (int i = 0; i < elements_size; ++i)
    {
        out << (i + 1) << " " << (mesh.elements[i].i_globalID + 1)
            << " " << (mesh.elements[i].j_globalID + 1) << " "
            << (mesh.elements[i].k_globalID + 1) << " "
            << 1 << " " << 1 << " " << 0 << std::endl;
    }
}