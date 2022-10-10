#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include <omp.h>
#include "Data.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;

// постоянная Стефана - Больцмана
const double SIGMA_0 = 5.67 * pow(10, -8);

const double pi = 3.14159265358979323846;

class FredholmSolver {
private:
	Mesh m_mesh;
	double m_epsilon;
	vector<double> m_global_vector;
	matrix<double> m_global_matrix;
	vector<double> m_solution;
public:
	FredholmSolver(const Mesh p_mesh, const double p_epsilon);
	matrix<double> createLocalMatrix(Element p_element);
	vector<double> createLocalVector(Element p_element);
	void assembleGlobalSystem();
	void solveGlobalSystem(); 
	void printToMV2();
};
