# PolyFit

## Overview

**Polynomial** is a template class that finds a least-squares polynomial fit to
data.  Because a polynomial fit linear equation is notoriously unbalanced,
it ranks the rows and uses those ranks in the L-U decomposition for safety.

Public methods:
 * Constructor, which takes the length of the polynomial,
 * fit, which takes X and Y data vectors and computes the fit,
 * value, which efficiently computes C(x), and
 * mse, which takes X and Y data and computes the mean squared-error.

All memory is organized in std::vectors of the template scalar type, and
allocated once in the constructor.  Thereafter, each call to fit() reuses
the pre-allocated memory.

**Polynomial.hpp** contains the template class and all methods.
**Polynomial.cpp** contains a test-driver and example code.
