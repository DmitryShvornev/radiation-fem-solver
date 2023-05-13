#pragma once
#include <vector>
#include <set>
#include <map>
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

const int THETA_UP = 873;

const int THETA_DOWN = 273;


const int LAMBDA = 83;

const int ALPHA = 11.6*pow(10,-6);


class ConductivitySolver {
	Mesh<Element3D> m_mesh;
	vector<double> m_global_vector;
	matrix<double> m_global_matrix;
	vector<double> m_solution;
	json m_mesh_JSON_data;
	vector<double> m_q_previous_solution;
	std::set<int> m_q_boundary_nodes;
	std::set<int> m_theta_up_boundary_nodes;
	std::set<int> m_theta_down_boundary_nodes;
	std::map<int, double> m_map_q_previous_solution;
	std::string m_suffix;
	bool m_is_with_radiation;
public:
	ConductivitySolver() = default;
	ConductivitySolver(const Mesh<Element3D> p_mesh, const json p_mesh_JSON_data, vector<double> p_q_previous_solution, std::string p_suffix, bool p_is_with_radiation);
	void init();
	bool isOnThetaUp(int p_id);
	bool isOnThetaDown(int p_id);
	bool isOnQ(int p_id);
	matrix<double> createGradientMatrix(Element3D p_element);
	matrix<double> createLocalMatrix(Element3D p_element);
	vector<double> createLocalVector(Element3D p_element);
	void assembleGlobalSystem();
	void solveGlobalSystem();
	vector<double> calculateThermalFlow();
	void printToMV2();
	~ConductivitySolver() {};
};