#include "FredholmSolver.h"
#include <cmath>
// постоянная Стефана - Больцмана
double SIGMA_0 = 5.67 * pow(10, -8);

// излучательная способность
double EPSILON = 0.7;

// температура
double THETA = 573;

double pi = 3.14159265358979323846;


double theta(double x, double y) {
    return 476.72*cos(atan(y / x) / 2.29);
}

FredholmSolver::FredholmSolver(const Mesh p_mesh) {
    mesh = p_mesh;
}

matrix<double> FredholmSolver::createLocalMatrix(Element p_element) {
    matrix<double> M = zero_matrix<double>(3, 3);
    matrix<double> E = identity_matrix<double>(3, 3);
    double square = p_element.getSquare();
    Point3D n_M = p_element.getNormal(); 
    std::vector<Node> current_nodes({ p_element.iNode(), p_element.jNode(), p_element.kNode()});
    std::for_each(mesh.elements.begin(), mesh.elements.end(), [&](Element element) {
        if (!(element == p_element)) {
            Point3D target_center = element.getCenter();
            std::vector<Node> target_nodes({ element.iNode(), element.jNode(), element.kNode()});
            double target_square = element.getSquare();
            Point3D n_N = element.getNormal();
            int j = 0;
#pragma omp parallel for private(j)
            for (int i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    Node temp = current_nodes[i] - target_nodes[j];
                    Point3D r_MN(temp.x(), temp.y(), temp.z());
                    double C_1 = target_square / (r_MN * r_MN);
                    double C_2 = (r_MN * n_N);
                    double C_3 = (square / (12 * pi)) * (1 - EPSILON);
                    double C_4 = (r_MN * n_M);
                    if (C_4 == 0 || C_2 == 0) {
                        continue;
                    }
                    else {
                        M(i, j) += C_1 * C_2 * C_3 * C_4;
                    }
                }
            }
            
        }
    });
    E *= square / 12;
    return E - M;
}

vector<double> FredholmSolver::createLocalVector(Element p_element) {
    double element = SIGMA_0 * EPSILON  * p_element.getSquare() / 3;
    std::vector<Node> nodes({ p_element.iNode(), p_element.jNode(), p_element.kNode()});
    vector<double> res(3);
    for (size_t i = 0; i < res.size(); i++)
    {
        double x = nodes[i].x();
        double y = nodes[i].y();
        res[i] = element * pow(theta(x,y), 4);
    }
    return res;
}

void FredholmSolver::assembleGlobalSystem() {
    std::cout << "Assembling global system..." << std::endl;
    const int SIZE = mesh.nodes.size();
    matrix<double> G = zero_matrix<double>(SIZE,SIZE);
    vector<double> H = zero_vector<double>(SIZE);
    std::for_each(mesh.elements.begin(), mesh.elements.end(), [&](Element element) {
        std::vector<std::size_t> IDs({ element.iGlobalID(), element.jGlobalID(), element.kGlobalID()});
        matrix<double> M = createLocalMatrix(element);
        vector<double> F = createLocalVector(element);
        for (size_t i = 0; i < 3; i++)
        {
            std::size_t i_global = IDs[i];
            H[i_global] += F[i];
            for (size_t j = 0; j < 3; j++)
            {
                std::size_t j_global = IDs[j];
                G(i_global, j_global) += M(i, j);
            }
        }
    });
    global_vector = H;
    global_matrix = G;
}


void FredholmSolver::solveGlobalSystem() {
    std::cout << "Solving global system..." << std::endl;
    const double EPS = 1e-15;
    vector<double> x = zero_vector<double>(global_matrix.size1());
    vector<double> r = global_vector - prec_prod(global_matrix, x);
    vector<double> p = r;
    double rSquare = inner_prod(r, r);
    int numIter = 0;
    while (rSquare > EPS)
    {
        numIter++;
        vector<double> temp = prec_prod(global_matrix, p);
        double alpha = rSquare / inner_prod(temp, p);
        x = x + alpha * p;

        vector<double> rNew = r - alpha * temp;
        double rNewSquare = inner_prod(rNew, rNew);
        double beta = rNewSquare / rSquare;
        r = rNew;
        rSquare = rNewSquare;
        p = r + beta * p;
    }
    std::cout << "Number of iterations: " << numIter << std::endl;
    solution = x;
}

void FredholmSolver::printToMV2() {
    std::ofstream out("result.mv2");
    int nodes_size = mesh.nodes.size();
    int elements_size = mesh.elements.size();
    out << nodes_size << " 3 2 q theta" << std::endl;
    for (int i = 0; i < nodes_size; ++i)
    {
        out << (i + 1) << " " << mesh.nodes[i].x() << " "
            << mesh.nodes[i].y() << " " << mesh.nodes[i].z() << " "
            << solution[i] << " " << theta(mesh.nodes[i].x(), mesh.nodes[i].y()) << std::endl;
    }

    out << elements_size << " 3 3 BC_id mat_id mat_id_Out" << std::endl;
    for (int i = 0; i < elements_size; ++i)
    {
        out << (i + 1) << " " << (mesh.elements[i].iGlobalID() + 1)
            << " " << (mesh.elements[i].jGlobalID() + 1) << " "
            << (mesh.elements[i].kGlobalID() + 1) << " "
            << 1 << " " << 1 << " " << 0 << std::endl;
    }
    out.close();
}