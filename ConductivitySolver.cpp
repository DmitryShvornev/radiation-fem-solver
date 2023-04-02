#include "ConductivitySolver.h"

ConductivitySolver::ConductivitySolver(const Mesh<Element3D> p_mesh, const json p_mesh_JSON_data) {
	this->m_mesh = p_mesh;
	for (auto raw_element : p_mesh_JSON_data["inner"]) {
		for (int j = 0; j < 3; j++) {
			if (this->m_q_boundary_nodes.find(raw_element[j]) == this->m_q_boundary_nodes.end()) {
				int key = raw_element[j];
				this->m_q_boundary_nodes.insert(key);
			}
		}
	}
	for (auto raw_element : p_mesh_JSON_data["up"]) {
		for (int j = 0; j < 3; j++) {
			if (this->m_theta_up_boundary_nodes.find(raw_element[j]) == this->m_theta_up_boundary_nodes.end()) {
				int key = raw_element[j];
				this->m_theta_up_boundary_nodes.insert(key);
			}
		}
	}
	for (auto raw_element : p_mesh_JSON_data["down"]) {
		for (int j = 0; j < 3; j++) {
			if (this->m_theta_down_boundary_nodes.find(raw_element[j]) == this->m_theta_down_boundary_nodes.end()) {
				int key = raw_element[j];
				this->m_theta_down_boundary_nodes.insert(key);
			}
		}
	}
}

void ConductivitySolver::solveGlobalSystem() {
    std::cout << "Solving global system..." << std::endl;
    const double EPS = 1e-25;
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

void ConductivitySolver::printToMV2() {
    std::ofstream out("result_3d.mv2");
    int nodes_size = this->m_mesh.nodes().size();
    int elements_size = this->m_mesh.elements().size();
    std::vector<Node> nodes = this->m_mesh.nodes();
    std::vector<Element3D> elements = this->m_mesh.elements();
    out << nodes_size << " 3 1 theta" << std::endl;
    for (int i = 0; i < nodes_size; ++i)
    {
        out << (i + 1) << " " << nodes[i].x() << " "
            << nodes[i].y() << " " << nodes[i].z() << " "
            << this->m_solution[i] << std::endl;
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