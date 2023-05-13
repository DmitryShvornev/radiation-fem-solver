#pragma once
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

template<class __DataType>
bool InvertMatrix(const ublas::matrix<__DataType>&input, ublas::matrix<__DataType>&inverse) {
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    matrix<__DataType> A(input);
    pmatrix pm(A.size1());
    int res = lu_factorize(A, pm);
    if (res != 0) return false;
    inverse.assign(ublas::identity_matrix<__DataType>(A.size1()));
    lu_substitute(A, pm, inverse);

    return true;
}

template<class __DataType>
__DataType Determinant(const ublas::matrix<__DataType>& matrix)
{
    ublas::matrix<__DataType> mLu(matrix);
    ublas::permutation_matrix<std::size_t> pivots(matrix.size1());

    auto isSingular = ublas::lu_factorize(mLu, pivots);
    if (isSingular)
        return static_cast<__DataType>(0);

    __DataType det = static_cast<__DataType>(1);
    for (std::size_t i = 0; i < pivots.size(); ++i)
    {
        if (pivots(i) != i)
            det *= static_cast<__DataType>(-1);

        det *= mLu(i, i);
    }

    return det;
}