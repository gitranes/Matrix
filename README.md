# Matrix
Templated C++ matrix library with Google unit-tests. Compiler supporting C++17 is required. The class implements basic Matrix operations such as matrix product, transpose, scalar product. In the future, the Matrix class will also provide basic linear algebra eg. inverse, linear equation solver, LU-factorization and. SVD-factorization.

For now, the Matrix class supports *built-in* arithmetic types and Fraction as the underlying type (with C++20 concepts that is bound to change).

## Construction
Matrix can be constructed in various ways presented below.

### Size construction
With size construction the elements are always default-constructed, hence with `int` we get zeros.
```cpp
// 3x3 of zeros
Matrix<int> zeros(3, 3)

// Square matrices can also be constructed with a single size argument.
Matrix<int> zeros2(3);

// 3x5 of 0 doubles
Matrix<double> d_zeros(3, 5);
```
### Fill construction
When fill constructing you declare both the size and the fill_type you want the Matrix to be filled with.
```cpp
// Explicit way of initializing a zero-matrix
Matrix<int> zeros(3, 3, fill_type::zeros);

// Matrix of ones
Matrix<unsigned> ones(3, fill_type::ones);

// Random whole number matrices. For now, the min and max can't be altered.
Matrix<int> randi(3, fill_type::randi);

// Random real number matrices
Matrix<double> rand(3, fill_type::rand);

// Random real numbers are truncated to int.
Matrix<int> rand2(4, fill_type::rand);
```

### WolframAlpha-like syntax
Matrices can also be constructed from `std::initializer_list`s. The size is always deduced. Erroneously sized *init list* will result in assertion failure.
```cpp
Matrix<int> zeros = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

// Type can also be deduced (C++17 class template argument deduction)
Matrix ones = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

// Assertion failure: syntax does not represent a Matrix.
Matrix<int> fail = {{1,1}, {1, 1, 1}}
```

## Basic operations

### Arithmetic and equality
The following arithmetic and assigment operations are available `+, -, *, +=, -=, *=`. `*`-operation denotes either scalar product or matrix product depending on the arguments. One can also compare matrices with `==` and `!=` operations. Inequality operations are not well-defined for matrices hence they are not available.
 
### Matrix operations
Matrix operations like *power, trace, transpose* are also implemented. Here *power* translates to simultaneous matrix products eg `A^3 = A*A*A`.

## Linear Algebra
This part is largely under construction. Only simple LU-factorization is available. 

### LU-factorization
For now, LU-factorization ends if the the pivot element is 0, further implementation is required. One should note that for *integral types* LU-factorization returns Matrices of type Fraction (see my other project) and for *floating-point types* the LU type matches the type of the Matrix.
```cpp
Matrix<int> randi(3, fill_type::randi);

// Structured binding is the preferred syntax
auto [L, U] = randi.lu();

// Explicit type is also available
LU_T<int> lu = randi.lu()

// LU_T<int> -> typename Matrix<int>::LU

// L and U are public members
Matrix<Fraction> L = lu.L;
Matrix<Fraction> U = lu.U;

Matrix<double> rand(3, fill_type::rand);

LU_T<double> = rand.lu()

// The L and U are now of type double
Matrix<double> L = lu.L;
Matrix<double> U = lu.U;
``` 
