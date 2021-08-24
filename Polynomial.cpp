#include "Polynomial.hpp"

#include <iostream>
#include <string>
#include <vector>

// First, a few framework functions.

/* operator<<
 *
 * Used for printing vectors and polynomials.
 */
template<typename T> std::ostream& operator<<(std::ostream& os,
        const std::vector<T>& v)
{
    for (auto t : v)
    {
        os << t << ' ';
    }

    return os;
}

template<typename T> std::ostream& operator<<(std::ostream& os,
        const Polynomial<T>& p)
{
    for (std::size_t i = 0; i < p.size(); ++i)
    {
        os << p[i] << ' ';
    }

    return os;
}

/* approx(x, y)
 *
 * Returns true if scalars x and y are approximately equal.
 */
template<typename T> bool approx(T x, T y)
{
    if (x == 0)
    {
        return y == 0;
    }

    return abs((x - y) / x) < .000001;
}

/* approx(X, Y)
 *
 * Returns true if vectors X and Y are approximately equal.
 */
template<typename T> bool approx(const std::vector<T>& x,
        const std::vector<T>& y)
{
    if (x.size() != y.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < x.size(); ++i)
    {
        if (!approx(x[i], y[i]))
        {
            return false;
        }
    }

    return true;
}

/* approx(P, V)
 *
 * Returns true if the coefficient vector of polynomial P is approximately
 * equal to vector V.
 */
template<typename T> bool approx(const Polynomial<T>& p,
        const std::vector<T>& v)
{
    return approx(static_cast<const std::vector<T>&>(p), v);
}

/* test
 *
 * The unit test.  Returns true if successful.
 */
bool test(void)
{
    // Test 1: a polynomial fit for a quadratic.
    std::cout << "Test 1" << std::endl;
    std::vector<float> X = { 0, 1, 2, 3 };
    std::vector<float> Y = { 2.1, 0.7, -0.1, 1.3 };
    Polynomial<float> p(3);
    p.fit(X, Y);
    std::vector<float> Cexp = { 2.18, -2.42, 0.7 };
    std::cout << "C = " << p << ", expecting " << Cexp << std::endl;
    if (!approx(p, Cexp))
        return false;

    // Test 2: polynomial from test 1, test value() at various points
    std::cout << std::endl << "Test 2" << std::endl;
    std::vector<float> pY(X.size());
    for (std::size_t i = 0; i < X.size(); ++i)
    {
        pY[i] = p.value(X[i]);
    }
    std::vector<float> pYexp = { 2.18, 0.46, 0.14, 1.22 };
    std::cout << "pY = " << pY << ", expecting " << pYexp << std::endl;
    if (!approx(pY, pYexp))
        return false;

    // Test 3: test the mse()
    std::cout << std::endl << "Test 3" << std::endl;
    float MSEexp = 0.128;
    float MSE = p.mse(X, Y);
    std::cout << "mse = " << MSE << " expecting " << MSEexp << std::endl;
    if (!approx(MSE, MSEexp))
        return false;

    return true;
}


/* main
 *
 */
int main(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    if (!test())
    {
        std::cout << std::endl << "FAILED!" << std::endl;
        return 1;
    }

    std::cout << std::endl << "PASSED!" << std::endl;
    return 0;
}
