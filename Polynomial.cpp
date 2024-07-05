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
 * @brief Used for printing vectors and polynomials.
 * @param os - std::ostream to insert.
 * @param v - arbitrary vector whose elements are to be inserted.
 * @return std::ostream to continue.
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
 * @brief Verifies that vectors X and Y are approximately equal.
 */
void EXPECT_FLOAT_VECTOR_EQ(const std::vector<float>& x,
        const std::vector<float>& y)
{
    EXPECT_EQ(x.size(), y.size());

    for (std::size_t i = 0; i < x.size(); ++i)
    {
        // The usual EXPECT_FLOAT_EQ() wasn't sloppy enough.
        EXPECT_NEAR(x[i], y[i], std::abs(x[i] * .000001));
    }
}

/**
 * The unit tests.
 */

/**
 * PolynomialTest
 *
 * @brief This is a test fixture class, used for tests below.  PolynomialTest
 * is declared as a fried to the Polynomial<> class that's under test so that
 * it has access to private members for testing.
 *
 * All tests using PolynomialTest as a fixture get derived Test classes, so
 * that their test bodies are run as class methods.  The using statements in
 * this declaration promote private members to protected so that these Test
 * classes have access too.
 */
class PolynomialTest: public testing::Test, public Polynomial<float>
{
protected:
    // Derived-class TEST_F's implicitly call their default constructors.
    // This provides a useful one for testing.
    PolynomialTest()    : Polynomial<float>(3)  {}

    // Give all fixture derived classes direct access to C.
    using Polynomial<float>::C;
};

TEST_F(PolynomialTest, FitQuadratic)
{
    // Test 1: a polynomial fit for a quadratic.
    std::vector<float> X = { 0, 1, 2, 3 };
    std::vector<float> Y = { 2.1, 0.7, -0.1, 1.3 };

    // Call the local fit().
    fit(X, Y);

    std::vector<float> Cexp = { 2.18, -2.42, 0.7 };
    // Direct access to C.
    EXPECT_FLOAT_VECTOR_EQ(C, Cexp);
}

/**
 * Ordinary TEST without a fixture.
 */
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
