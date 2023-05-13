#include "ConductivitySolver.h"

ConductivitySolver::ConductivitySolver(const Mesh<Element3D> p_mesh, const json p_mesh_JSON_data, vector<double> p_q_previous_solution, std::string p_suffix, bool p_is_with_radiation) {
	this->m_mesh = p_mesh;
    this->m_q_previous_solution = p_q_previous_solution;
    this->m_mesh_JSON_data = p_mesh_JSON_data;
    this->m_suffix = p_suffix;
    this->m_is_with_radiation = p_is_with_radiation;
}

void ConductivitySolver::init() {
    int i = 0;
    for (auto raw_element : this->m_mesh_JSON_data["inner"]) {
        for (int j = 0; j < 3; j++) {
            if (this->m_q_boundary_nodes.find(raw_element[j]) == this->m_q_boundary_nodes.end()) {
                int key = raw_element[j];
                this->m_q_boundary_nodes.insert(key);
                this->m_map_q_previous_solution.insert({ key, this->m_q_previous_solution[i] });
                i++;
            }
        }
    }
    for (auto raw_element : this->m_mesh_JSON_data["up"]) {
        for (int j = 0; j < 3; j++) {
            if (this->m_theta_up_boundary_nodes.find(raw_element[j]) == this->m_theta_up_boundary_nodes.end()) {
                int key = raw_element[j];
                this->m_theta_up_boundary_nodes.insert(key);
            }
        }
    }
    for (auto raw_element : this->m_mesh_JSON_data["down"]) {
        for (int j = 0; j < 3; j++) {
            if (this->m_theta_down_boundary_nodes.find(raw_element[j]) == this->m_theta_down_boundary_nodes.end()) {
                int key = raw_element[j];
                this->m_theta_down_boundary_nodes.insert(key);
            }
        }
    }
}

bool ConductivitySolver::isOnThetaUp(int p_id) {
    return this->m_theta_up_boundary_nodes.find(p_id) != this->m_theta_up_boundary_nodes.end();
}

bool ConductivitySolver::isOnThetaDown(int p_id) {
    return this->m_theta_down_boundary_nodes.find(p_id) != this->m_theta_down_boundary_nodes.end();
}

bool ConductivitySolver::isOnQ(int p_id) {
    return this->m_q_boundary_nodes.find(p_id) != this->m_q_boundary_nodes.end();
}

matrix<double> ConductivitySolver::createGradientMatrix(Element3D p_element) {
    matrix<double> baricentricCoords = p_element.getBaricentricCoords();
    matrix<double> B_buff(4, 3);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            B_buff(i, j - 1) = baricentricCoords(i, j);
        }
    };
    matrix<double> B = trans(B_buff);
    return B;
}

matrix<double> ConductivitySolver::createLocalMatrix(Element3D p_element) {
    matrix<double> B = createGradientMatrix(p_element);
    matrix<double> lambda = zero_matrix<double>(3,3);
    lambda(0, 0) = LAMBDA; lambda(1, 1) = LAMBDA; lambda(2, 2) = LAMBDA;
    matrix<double> bcs = zero_matrix<double>(4, 4);
    std::vector<unsigned> element_ids({ p_element.iGlobalID(), p_element.jGlobalID(), p_element.kGlobalID(), p_element.lGlobalID() });
    int element_count = 0;
    std::vector<int> surface_ids(3);
    for (auto i : element_ids) {
        if (this->isOnThetaUp(i) || this->isOnThetaDown(i)) {
            element_count++;
            surface_ids.push_back(i);
        }
    }
    if (element_count == 3) {
        Element2D temp(m_mesh.nodes()[surface_ids[0]], m_mesh.nodes()[surface_ids[1]], m_mesh.nodes()[surface_ids[2]]);
        double S = temp.getSquare();
        bcs(1, 1) = 2; bcs(1, 2) = 1; bcs(1, 3) = 1;
        bcs(2, 1) = 1; bcs(2, 2) = 2; bcs(2, 3) = 1;
        bcs(3, 1) = 1; bcs(3, 2) = 1; bcs(3, 3) = 2;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                bcs(i, j) *= ALPHA * S / 12;
            }
        }
    }

    double V = p_element.getVolume();
    matrix<double> B_T_lambda = prod(trans(B), lambda);
    matrix<double> B_T_lambda_B = prod(B_T_lambda, B);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            B_T_lambda_B(i, j) += bcs(i, j);
            B_T_lambda_B(i, j) *= V;
        }
    }
    return prod(B_T_lambda, B);
}

vector<double> ConductivitySolver::createLocalVector(Element3D p_element) {
    vector<double> l_Res = zero_vector<double>(4);
    double V = p_element.getVolume();
    std::vector<unsigned> element_ids({ p_element.iGlobalID(), p_element.jGlobalID(), p_element.kGlobalID(), p_element.lGlobalID() });
    int element_count_up = 0;
    int element_count_down = 0;
    int element_count_q = 0;
    std::vector<int> surface_ids(3);
    for (unsigned i = 0; i < 4; i++) {
        if (this->isOnQ(element_ids[i])) {
            element_count_q++;
            surface_ids.push_back(element_ids[i]);
        }
        if (this->isOnThetaUp(element_ids[i])) {
            element_count_up++;
            surface_ids.push_back(element_ids[i]);
        }
        if (this->isOnThetaDown(element_ids[i])) {
            element_count_down++;
            surface_ids.push_back(element_ids[i]);
        }
    }
    vector<double> bcs = zero_vector<double>(4);
    
    if (element_count_up == 3) {
        Element2D temp(m_mesh.nodes()[surface_ids[0]], m_mesh.nodes()[surface_ids[1]], m_mesh.nodes()[surface_ids[2]]);
        double S = temp.getSquare();
        bcs(1) = 1; bcs(2) = 1; bcs(3) = 1;
        for (int i = 0; i < 4; i++)
        {
            bcs(i) *= ALPHA * THETA_UP * S / 3;
        }
    }
    if (element_count_down == 3) {
        Element2D temp(m_mesh.nodes()[surface_ids[0]], m_mesh.nodes()[surface_ids[1]], m_mesh.nodes()[surface_ids[2]]);
        double S = temp.getSquare();
        bcs(1) = 1; bcs(2) = 1; bcs(3) = 1;
        for (int i = 0; i < 4; i++)
        {
            bcs(i) *= ALPHA * THETA_DOWN * S / 3;
        }
    }
    if (element_count_q == 3 && this->m_is_with_radiation) {
        Element2D temp(m_mesh.nodes()[surface_ids[0]], m_mesh.nodes()[surface_ids[1]], m_mesh.nodes()[surface_ids[2]]);
        double S = temp.getSquare();
        bcs(1) = 1; bcs(2) = 1; bcs(3) = 1;
        for (int i = 1; i < 4; i++)
        {
            bcs(i) *= this->m_map_q_previous_solution[surface_ids[i]] * S;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        l_Res[i] += bcs(i);
    }
    return l_Res;
}

void ConductivitySolver::assembleGlobalSystem() {
    std::cout << "Assembling global Conductivity system..." << std::endl;
    const int SIZE = this->m_mesh.nodes().size();
    const int elementsSize = this->m_mesh.elements().size();
    mapped_matrix<double> G(SIZE, SIZE, SIZE);
    vector<double> H = zero_vector<double>(SIZE);
    std::vector<Element3D> elements = this->m_mesh.elements();
    int i;
    omp_set_num_threads(4);
#pragma omp parallel for private(i)
    for (i = 0; i < elementsSize; i++) {
        std::vector<std::size_t> IDs({ elements[i].iGlobalID(), elements[i].jGlobalID(), elements[i].kGlobalID(), elements[i].lGlobalID() });
        for (int j = 0; j < 4; j++) {
            int i_global = IDs[j];
            if (isOnThetaUp(i_global)) {
                G(i_global, i_global) = 1;
                H[i_global] = THETA_UP;
            }
            else if (isOnThetaDown(i_global)) {
                G(i_global, i_global) = 1;
                H[i_global] = THETA_DOWN;
            }
        }
    }
    for (int i = 0; i < elementsSize; i++)
    {
        std::vector<std::size_t> IDs({ elements[i].iGlobalID(), elements[i].jGlobalID(), elements[i].kGlobalID(), elements[i].lGlobalID() });
        matrix<double> M = createLocalMatrix(elements[i]);
        vector<double> F = createLocalVector(elements[i]);
        int j, k, i_global, j_global;
        for (j = 0; j < 4; j++)
        {
            i_global = IDs[j];
            if (!isOnThetaDown(i_global) && !isOnThetaUp(i_global)) {
                for (k = 0; k < 4; k++)
                {
                    j_global = IDs[k];
                    if (isnan(M(j, k)) || isinf(M(j, k))) {
                        continue;
                    }
                    if (!isOnThetaDown(j_global) && !isOnThetaUp(j_global)) {
                        G(i_global, j_global) += M(j, k);
                    }
                    else {
                        if (isOnThetaDown(j_global)) {
                            H[i_global] -= M(j, k) * THETA_DOWN;
                        }
                        else if (isOnThetaUp(j_global)) {
                            H[i_global] -= M(j, k) * THETA_UP;
                        }
                    }
                    
                }
            }
            if (isnan(F[j]) || isinf(F[j])) {
                continue;
            }
            H[i_global] += F[j];
            
        }
    }
    std::cout << "Local SLAEs created" << std::endl;
    this->m_global_vector = H;
    this->m_global_matrix = G;
}

void ConductivitySolver::solveGlobalSystem() {
    std::cout << "Solving global Conductivity system..." << std::endl;
    const double EPS = 1e-5;
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
        std::cout << "Discrepancy " << rSquare << std::endl;
    }
    std::cout << "Number of iterations: " << numIter << std::endl;
    this->m_solution = x;
}

vector<double> ConductivitySolver::calculateThermalFlow() {
    const int SIZE = this->m_mesh.nodes().size();
    const int elementsSize = this->m_mesh.elements().size();
    std::vector<Element3D> elements = this->m_mesh.elements();
    vector<double> Q = zero_vector<double>(SIZE);
    for (int i = 0; i < elementsSize; i++)
    {
        std::vector<std::size_t> IDs({ elements[i].iGlobalID(), elements[i].jGlobalID(), elements[i].kGlobalID(), elements[i].lGlobalID() });
        double V = elements[i].getVolume();
        matrix<double> B = createGradientMatrix(elements[i]);
        vector<double> T(4);
        for (int k = 0; k < 4; k++)
        {
            int id = IDs[k];
            T[k] = this->m_solution[id];
        }
        vector<double> q_local = prec_prod(B,T);
        double q_grad = sqrt(q_local[0] * q_local[0] + q_local[1] * q_local[1] + q_local[2] * q_local[2]);
        int j, i_global;
        for (j = 0; j < 4; j++)
        {
            i_global = IDs[j];
            if (this->m_is_with_radiation) {
                if (isOnQ(i_global)) {
                    Q[i_global] = this->m_map_q_previous_solution[i_global];
                }
                else {
                    Q[i_global] += LAMBDA * q_grad;
                }
            }
            else {
                Q[i_global] += LAMBDA * q_grad;
            }
        }
    }
    return Q;
}

void ConductivitySolver::printToMV2() {
    std::ofstream out("result_3d_" + this->m_suffix +".mv2");
    int nodes_size = this->m_mesh.nodes().size();
    int elements_size = this->m_mesh.elements().size();
    std::vector<Node> nodes = this->m_mesh.nodes();
    std::vector<Element3D> elements = this->m_mesh.elements();
    vector<double> Q = calculateThermalFlow();
    out << nodes_size << " 3 2 theta q" << std::endl;
    for (int i = 0; i < nodes_size; ++i)
    {
        out << (i + 1) << " " << nodes[i].x() << " "
            << nodes[i].y() << " " << nodes[i].z() << " "
            << this->m_solution[i] << " " << Q[i] << std::endl;
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