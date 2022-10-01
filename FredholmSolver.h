#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include "Data.h"

class FredholmSolver {
public:
	Mesh mesh;
	std::vector<double> global_vector;
	std::vector<std::vector<double>> global_matrix;
	std::vector<double> solution;
	FredholmSolver(const Mesh p_mesh);
	std::vector<std::vector<double>> createLocalMatrix(Element p_element);
	std::vector<double> createLocalVector(Element p_element);
	void assembleGlobalSystem();
	void solveGlobalSystem(); 
	void printToMV2();
};