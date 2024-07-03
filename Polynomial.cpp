#include "Polynomial.hpp"

#include <iostream>
#include <string>
#include <vector>

// https://google.github.io/googletest/
#include <gtest/gtest.h>

// First, a few framework functions.

/**
 * operator<<
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

/**
 * EXPECT_FLOAT_VECTOR_EQ(X, Y)
 *
 * Returns true if vectors X and Y are approximately equal.
 */
void EXPECT_FLOAT_VECTOR_EQ(const std::vector<float>& x,
        const std::vector<float>& y)
{
    EXPECT_EQ(x.size(), y.size());

    for (std::size_t i = 0; i < x.size(); ++i)
    {
        EXPECT_NEAR(x[i], y[i], std::abs(x[i] * .000001));
    }
}

/**
 * EXPECT_FLOAT_POLY_VECTOR_EQ(P, V)
 *
 * Returns true if the coefficient vector of polynomial P is approximately
 * equal to vector V.
 */
void EXPECT_FLOAT_POLY_VECTOR_EQ(const Polynomial<float>& p,
        const std::vector<float>& v)
{
    EXPECT_FLOAT_VECTOR_EQ(static_cast<const std::vector<float>&>(p), v);
}

/**
 * The unit tests.
 */
TEST(PolyFit, FitQuadratic)
{
    // Test 1: a polynomial fit for a quadratic.
    std::vector<float> X = { 0, 1, 2, 3 };
    std::vector<float> Y = { 2.1, 0.7, -0.1, 1.3 };
    Polynomial<float> p(3);
    p.fit(X, Y);
    std::vector<float> Cexp = { 2.18, -2.42, 0.7 };

    EXPECT_FLOAT_POLY_VECTOR_EQ(p, Cexp);
}

TEST(PolyFit, Value)
{
    std::vector<float> X = { 0, 1, 2, 3 };
    std::vector<float> Y = { 2.1, 0.7, -0.1, 1.3 };
    Polynomial<float> p(3);
    p[0] = 2.18;
    p[1] = -2.42;
    p[2] = 0.7;

    // Test 2: polynomial from test 1, test value() at various points
    std::vector<float> pY(X.size());
    for (std::size_t i = 0; i < X.size(); ++i)
    {
        pY[i] = p.value(X[i]);
    }
    std::vector<float> pYexp = { 2.18, 0.46, 0.14, 1.22 };

    EXPECT_FLOAT_VECTOR_EQ(pY, pYexp);
}

TEST(PolyFit, MSE)
{
    std::vector<float> X = { 0, 1, 2, 3 };
    std::vector<float> Y = { 2.1, 0.7, -0.1, 1.3 };
    Polynomial<float> p(3);
    p[0] = 2.18;
    p[1] = -2.42;
    p[2] = 0.7;

    // Test 3: test the mse()
    float MSEexp = 0.128;
    float MSE = p.mse(X, Y);

    EXPECT_FLOAT_EQ(MSE, MSEexp);
}


/**
 * main
 *
 * @summary: Run the GoogleTests.
 *
 * @return: exit code.
 */
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
