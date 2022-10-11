#include "FredholmSolver.h"
#include <cmath>


double theta(double x, double y) {
    return 476.72*cos(atan(y / x) / 2.29);
}

FredholmSolver::FredholmSolver(const Mesh p_mesh, const double p_epsilon) {
    this->m_mesh = p_mesh;
    if (p_epsilon < 0 || p_epsilon > 1) {
        throw IncorrectEmissivityException(p_epsilon);
    }
    this->m_epsilon = p_epsilon;
}

matrix<double> FredholmSolver::createLocalMatrix(Element p_element) {
    matrix<double> M = zero_matrix<double>(3, 3);
    matrix<double> E = identity_matrix<double>(3, 3);
    double square = p_element.getSquare();
    Point3D n_M = p_element.getNormal(); 
    std::vector<Node> current_nodes({ p_element.iNode(), p_element.jNode(), p_element.kNode()});

    Point3D target_center;
    double target_square;
    Point3D n_N;
    int i, j;
    double C_1, C_2, C_3, C_4;
    std::vector<Element> elements = this->m_mesh.elements();
    std::for_each(elements.begin(), elements.end(), [&](Element element) {
        if (!(element == p_element)) {
            target_center = element.getCenter();
            std::vector<Node> target_nodes({ element.iNode(), element.jNode(), element.kNode()});
            target_square = element.getSquare();
            n_N = element.getNormal();
            if (n_M * n_N < -0.866) {
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        Node temp = current_nodes[i] - target_nodes[j];
                        Point3D r_MN(temp.x(), temp.y(), temp.z());
                        C_1 = target_square / (r_MN * r_MN);
                        C_2 = (r_MN * n_N);
                        C_3 = (square / (12 * pi)) * (1 - this->m_epsilon);
                        C_4 = (r_MN * n_M);
                        if (C_4 == 0 || C_2 == 0) {
                            continue;
                        }
                        else {
                            M(i, j) += C_1 * C_2 * C_3 * C_4;
                        }
                    }
                }
            }
        }
    });
    E *= square / 12;
    return E - M;
}

vector<double> FredholmSolver::createLocalVector(Element p_element) {
    double element = SIGMA_0 * this->m_epsilon  * p_element.getSquare() / 3;
    std::vector<Node> nodes({ p_element.iNode(), p_element.jNode(), p_element.kNode()});
    vector<double> res(3);
    double x, y;
    for (size_t i = 0; i < res.size(); i++)
    {
        x = nodes[i].x();
        y = nodes[i].y();
        res[i] = element * pow(theta(x,y), 4);
    }
    return res;
}

void FredholmSolver::assembleGlobalSystem() {
    std::cout << "Assembling global system..." << std::endl;
    const int SIZE = this->m_mesh.nodes().size();
    matrix<double> G = zero_matrix<double>(SIZE,SIZE);
    vector<double> H = zero_vector<double>(SIZE);
    std::vector<Element> elements = this->m_mesh.elements();
    int i;
#pragma omp parallel for private(i)
    for (i = 0; i < elements.size(); i++)
    {
        std::vector<std::size_t> IDs({ elements[i].iGlobalID(), elements[i].jGlobalID(), elements[i].kGlobalID()});
        matrix<double> M = createLocalMatrix(elements[i]);
        vector<double> F = createLocalVector(elements[i]);
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
    }
    this->m_global_vector = H;
    this->m_global_matrix = G;
}


void FredholmSolver::solveGlobalSystem() {
    std::cout << "Solving global system..." << std::endl;
    const double EPS = 1e-15;
    vector<double> x = zero_vector<double>(this->m_global_matrix.size1());
    vector<double> r = this->m_global_vector - prec_prod(this->m_global_matrix, x);
    vector<double> p = r;
    double rSquare = inner_prod(r, r);
    int numIter = 0;
    vector<double> temp;
    double alpha;
    vector<double> rNew;
    double rNewSquare;
    double beta;
    while (rSquare > EPS)
    {
        numIter++;
        temp = prec_prod(this->m_global_matrix, p);
        alpha = rSquare / inner_prod(temp, p);
        x = x + alpha * p;

        rNew = r - alpha * temp;
        rNewSquare = inner_prod(rNew, rNew);
        beta = rNewSquare / rSquare;
        r = rNew;
        rSquare = rNewSquare;
        p = r + beta * p;
    }
    std::cout << "Number of iterations: " << numIter << std::endl;
    this->m_solution = x;
}

void FredholmSolver::printToMV2() {
    std::ofstream out("result.mv2");
    int nodes_size = this->m_mesh.nodes().size();
    int elements_size = this->m_mesh.elements().size();
    std::vector<Node> nodes = this->m_mesh.nodes();
    std::vector<Element> elements = this->m_mesh.elements();
    out << nodes_size << " 3 2 q theta" << std::endl;
    for (int i = 0; i < nodes_size; ++i)
    {
        out << (i + 1) << " " << nodes[i].x() << " "
            << nodes[i].y() << " " << nodes[i].z() << " "
            << this->m_solution[i] << " " << theta(nodes[i].x(), nodes[i].y()) << std::endl;
    }

    out << elements_size << " 3 3 BC_id mat_id mat_id_Out" << std::endl;
    for (int i = 0; i < elements_size; ++i)
    {
        out << (i + 1) << " " << (elements[i].iGlobalID() + 1)
            << " " << (elements[i].jGlobalID() + 1) << " "
            << (elements[i].kGlobalID() + 1) << " "
            << 1 << " " << 1 << " " << 0 << std::endl;
    }
    out.close();
}