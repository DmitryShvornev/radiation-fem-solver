#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include "Data.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/triangular.hpp>

using namespace boost::numeric::ublas;

class FredholmSolver {
public:
	Mesh mesh;
	vector<double> global_vector;
	matrix<double> global_matrix;
	vector<double> solution;
	FredholmSolver(const Mesh p_mesh);
	matrix<double> createLocalMatrix(Element p_element);
	vector<double> createLocalVector(Element p_element);
	void assembleGlobalSystem();
	void solveGlobalSystem(); 
	void printToMV2();
};
