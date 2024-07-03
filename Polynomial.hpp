#ifndef _POLYNOMIAL_HPP_
#define _POLYNOMIAL_HPP_

#include <vector>

/* Polynomial fit and value routines.  Uses Least-Squares to find a
 * fit for a data set, pivoting rows of the matrix for best accuracy.
 * Other methods: return the polynomial's value at a point on the independent
 * axis with the least number of multiplications, and return the mean squared-
 * error of the polynomial fit over its degrees of freedom.
 */

template<typename Scalar> class Polynomial
{
public:
    Polynomial(std::size_t _length);        // order of polynomial plus 1

    void fit(const std::vector<Scalar>& X, const std::vector<Scalar>& Y);
    Scalar value(Scalar x) const;
    Scalar mse(const std::vector<Scalar>& X, const std::vector<Scalar>& Y) const;

    std::size_t size(void) const                    { return C.size(); }
    Scalar& operator[](std::size_t i)               { return C[i]; }
    const Scalar& operator[](std::size_t i) const   { return C[i]; }
    operator const std::vector<Scalar>&() const     { return C; }

private:
    friend class PolynomialTest;

    void _solve(void);
    Scalar _ipow(Scalar x, std::size_t n) const;
    Scalar _fabs(Scalar x) const    { return x >= 0 ? x : -x; }

    std::vector<std::vector<Scalar>> A;     // the NxN matrix
    std::vector<Scalar> B;                  // right-side
    std::vector<Scalar> C;                  // result
    std::vector<Scalar> S;                  // scale factors
    std::vector<std::size_t> P;             // permutator
};

/* Polynomial(length)
 *
 * Constructor.  The length parameter is the number of coefficients; i.e. the
 * order of the polynomial plus 1.  Allocates all the memory needed for
 * subsequent calls to fit().
 */
template<typename Scalar> Polynomial<Scalar>::Polynomial(std::size_t _length)
:   A(_length), B(_length), C(_length), S(_length), P(_length)
{
    for (auto& row: A)
        row.reserve(_length);
}

/* fit(X, Y)
 *
 * Find coefficients of a polynomial which best fits a data set.
 * Vectors X and Y specify the (x,y) coordinates of the data set to
 * be fit.  The polynomial solution will be written to C,
 * where C[0] is the constant coefficient.
 */
template<typename Scalar> void Polynomial<Scalar>::fit(
        const std::vector<Scalar>& X, const std::vector<Scalar>& Y)
{
    // Fill the matrix A and right-side vector B
    for (std::size_t i = 0; i < 2 * C.size() - 1; ++i)
    {
        Scalar s = 0;
        if (i < C.size())
            B[i] = 0;
        for (std::size_t j = 0; j < X.size(); ++j)
        {
            Scalar t = _ipow(X[j], i);
            s += t;
            if (i < C.size())
                B[i] += Y[j] * t;
        }
        for (std::size_t j = 0; j <= i; ++j)
            if (j < C.size() && i - j < C.size())
                A[j][i - j] = s;
    }

    _solve();
}

/* _solve()
 *
 * Linear equations to solve polynomial fits are notoriously unbalanced.
 * This solve() ranks each row and uses the ranks in the LU decomposition.
 * The results are stored in vector C.
 */
template<typename Scalar> void Polynomial<Scalar>::_solve(void)
{
    // Assign scale factors by biggest element in each row.
    for (std::size_t k = 0; k < C.size(); ++k)
    {
        P[k] = k;
        S[k] = 0;
        for (std::size_t j = 0; j < C.size(); ++j)
            if (_fabs(A[k][j]) > S[k])
                S[k] = _fabs(A[k][j]);
    }

    // Do a Pivoting LU decomposition of A.
    for (std::size_t k = 0; k < C.size(); ++k)
    {
        std::size_t j = k;
        for (std::size_t i = k; i < C.size(); ++i)
            if (_fabs(A[P[i]][k]) / S[P[i]] > _fabs(A[P[j]][k]) / S[P[j]])
                j = i;
        // permutate
        std::size_t i = P[j];
        P[j] = P[k];
        P[k] = i;

        // forward substitution
        for (std::size_t i = k + 1; i < C.size(); ++i)
        {
            Scalar s = A[P[i]][k] / A[P[k]][k];  // the scale factor
            for (std::size_t j = k + 1; j < C.size(); ++j)
                A[P[i]][j] -= s * A[P[k]][j];
            B[P[i]] -= s * B[P[k]];
        }
    }

    // backward substitution to find C
    for (signed int i = C.size() - 1; i >= 0; --i)
    {
        C[i] = B[P[i]];
        for (std::size_t j = i + 1; j < C.size(); ++j)
        {
            C[i] -= A[P[i]][j] * C[j];
        }
        C[i] /= A[P[i]][i];
    }
}

/* Scalar _ipow(Scalar x, int n)
 *
 * Returns x to the nth power.
 */
template<typename Scalar> Scalar Polynomial<Scalar>::_ipow(Scalar x,
        std::size_t n) const
{
    Scalar r = 1.;

    for (std::size_t i = 0; i < n; ++i)
        r *= x;

    return r;
}

/* Scalar value(Scalar x)
 *
 * Efficiently computes C(x)
 */
template<typename Scalar> Scalar Polynomial<Scalar>::value(Scalar x) const
{
    Scalar r = C[C.size() - 1]; /* start with leading coefficient */

    // ((C[3] * x + C[2]) * x + C[1]) * x + C[0]
    for (signed int i = C.size() - 2; i >= 0; --i)
        r = r * x + C[i];

    return r;
}

/* Scalar mse(X, Y)
 *
 * Computes the Mean Squared-Error over data points (X, Y).  The MSE is
 * defined over the remaining dimensions.
 */
template<typename Scalar> Scalar Polynomial<Scalar>::mse(
        const std::vector<Scalar>& X, const std::vector<Scalar>& Y) const
{
    if (X.size() <= C.size())
        return 0.;  // exact fit

    Scalar sqe(0);

    for (std::size_t i = 0; i < X.size(); ++i)
    {
        Scalar e = value(X[i]) - Y[i];
        sqe += e * e;
    }

    return sqe / (X.size() - C.size());   // error over extra dimensions only
}

#endif
