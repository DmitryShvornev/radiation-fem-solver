#pragma once
#include "boost/numeric/ublas/matrix.hpp"
#include <vector>
#include <map>
#include <gmsh.h>
#include "Data.h"

using namespace boost::numeric::ublas;
using namespace std;

class FredholmSolver {
private:
	std::vector<Element> mesh_elements;
	std::vector<Node> mesh_nodes;
	multimap<Node, Element> node_map;
	std::vector<double> global_vector;
	matrix<double> global_matrix;
public:
	FredholmSolver(const std::vector<double> p_coords);
	matrix<double> createLocalMatrix(const Element p_element);
	std::vector<double> createLocalVector(const Element p_element);
	void assembleGlobalSystem();
	std::vector<Node> getNodes();
	std::vector<double> solveGlobalSystem();
	void printToMV2();
};
