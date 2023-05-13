#pragma once
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <omp.h>
#include "Data.h"
#include <nlohmann/json.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <cmath>
#include <boost/numeric/ublas/matrix_sparse.hpp>

using namespace boost::numeric::ublas;
using json = nlohmann::json;

// постоянная Стефана - Больцмана
const double SIGMA_0 = 5.67 * pow(10, -8);

const double pi = 3.14159265358979323846;

class FredholmSolver {
private:
	Mesh<Element2D> m_mesh;
	double m_epsilon;
	vector<double> m_global_vector;
	matrix<double> m_global_matrix;
	vector<double> m_solution;
	std::string m_suffix;
public:
	FredholmSolver(const Mesh<Element2D> p_mesh, const double p_epsilon, const std::string p_suffix);
	class IncorrectEmissivityException : public std::exception {
		std::string m_message;
	public:
		IncorrectEmissivityException(double p_emissivity) {
			m_message.assign("FATAL ERROR: Incorrect emissivity value " +
				std::to_string(p_emissivity) + " is not in range [0,1]");
		}

		const char* what() const {
			return m_message.c_str();
		}
	};
	matrix<double> createLocalMatrix(Element2D p_element);
	vector<double> createLocalVector(Element2D p_element);
	vector<double> solution();
	void assembleGlobalSystem();
	void solveGlobalSystem(); 
	void printToMV2();
	~FredholmSolver() {};
};
