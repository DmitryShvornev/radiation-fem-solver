#pragma once
#include <vector>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;


class CSRMatrix {
private:
	double m_N;
	std::vector<double> m_elements;
	std::vector<int> m_i_indexes;
	std::vector<int> m_j_indexes;
public:
	CSRMatrix();
	CSRMatrix(double p_n);
	CSRMatrix(const CSRMatrix& p_matrix);
	const CSRMatrix& operator=(const CSRMatrix& p_matrix);
	vector<double> operator*(const vector<double>& p_vector);
	double At(int i, int j);
	void set(int i, int j, double value);
	~CSRMatrix() {};
};