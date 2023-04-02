#pragma once
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <omp.h>
#include "Data.h"
#include <cmath>
#include <nlohmann/json.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

using json = nlohmann::json;


class ConductivitySolver {
	Mesh<Element3D> m_mesh;
	vector<double> m_global_vector;
	matrix<double> m_global_matrix;
	vector<double> m_solution;
	std::set<int> m_q_boundary_nodes;
	std::set<int> m_theta_up_boundary_nodes;
	std::set<int> m_theta_down_boundary_nodes;
public:
	ConductivitySolver() = default;
	ConductivitySolver(const Mesh<Element3D> p_mesh, const json p_mesh_JSON_data);
	matrix<double> createLocalMatrix(Element3D p_element);
	vector<double> createLocalVector(Element3D p_element);
	void assembleGlobalSystem();
	void solveGlobalSystem();
	void printToMV2();
	~ConductivitySolver() {};
};