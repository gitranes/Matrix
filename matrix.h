#pragma once

#include "pch.h"
#include "VectorOps.h"

/*
* -- Fill types are --
* zeros - fills matrix with zeros
* ones - fills matrix with ones
* identity - fills main diagonal with ones
* randi - fills the matrix with random integers
* rand - fills the matrix with random real numbers
* >> Sensible with floating point types, for the rest result is
* equivalent to using randi.
*/
enum class fill_type
{
	// TODO: Remove zeros fill-type?
	zeros,
	ones,
	identity,
	randi,
	rand
};

template<typename T>
class Matrix;

// Forward declare to-be-templated friend methods
template<typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& obj);


// TODO: Matrix Base Class

template<typename T> 
class Matrix
{
public:
	// Non-initializing constructors:

	// Square Matrix constructor
	explicit Matrix(const std::size_t n);

	// Non-square Matrix constructor
	explicit Matrix(const std::size_t n, const std::size_t m);


	// Filling constructors:

	// Fills a square Matrix with the fill_type
	explicit Matrix(const std::size_t n, const fill_type fill_type);

	// Fill a non-square Matrix with the fill_type
	explicit Matrix(
		const std::size_t n, const std::size_t m, fill_type fill_type);

	// std::initializer_list and vector constructors. Implicit conversions are
	// allowed. Copies do not matter as the elements are trivially copyable.

	// Size and elements are derived from the initializer list. 
	 Matrix(std::initializer_list<std::vector<T>> init_list);

	// Size is derived from the vector
	Matrix(const std::vector<std::vector<T>>& vectors);

	// Destructor, copy and move operations are implicit

	// Conversion from integral types to Fraction
	template <typename U = T>
	operator std::enable_if_t<std::is_integral_v<U>, Matrix<Fraction>>() const
	{
		// Construct a Fraction Matrix
		Matrix<Fraction> frac_mat(col_size_, row_size_);

		// Assign values from *this
		for (unsigned i = 0; i < col_size_; ++i)
		{
			for (unsigned j = 0; j < row_size_; ++j)
			{
				frac_mat[i][j] = vectors_[i][j];
			}
		}
		return frac_mat;
	}
	
	// Returns a ref to the corresponding row. Does basic bounds checking.
	std::vector<T>& operator[](const std::size_t index)
	{
		assert(index <= col_size_ - 1);
		return vectors_[index];
	}

	// Returns a const-ref to the corresponding row. Basic bounds checking
	// is performed.
	const std::vector<T>& operator[](const std::size_t index) const
	{
		assert(index <= col_size_ - 1);
		return vectors_[index];
	}
	
	/*Fills the matrix according to the fill_type
	 * Min and max can be specified with set_rand_limits() or set_rand_min()/
	 * set_rand_max(). For char-types printable characters are in range
	 * [41, 126]
	 */
	Matrix& fill(fill_type fill_type);


	// Arithmetic operations. Declaring operations friend allows implicit
	// conversions both ways, here it has nothing to do with access-specifying.
	// See Helpers -> VectorOperations for the precise implementations.

	friend Matrix& operator*=(Matrix& lhs, const Matrix& rhs)
	{
		assert(lhs.size() == rhs.size());

		// TODO: Optimize *=
		// Lazy way:
		Matrix<T> result = lhs * rhs;
		lhs.vectors_ = result.vectors_;

		return lhs;
	}
	
	friend Matrix& operator+=(Matrix& lhs, const Matrix& rhs)
	{
		using namespace VectorOperations;
		assert(lhs.size() == rhs.size());

		lhs.vectors_ += rhs.vectors_;
		return lhs;
	}
	
	friend Matrix& operator-=(Matrix& lhs, const Matrix& rhs)
	{
		static_assert(std::is_signed<T>() || std::is_class<T>(),
			"subtraction is not defined for unsigned integral type");
		
		using namespace VectorOperations;
		assert(lhs.size() == rhs.size());

		lhs.vectors_ -= rhs.vectors_;
		return lhs;
	}
	
	friend Matrix operator+(const Matrix& lhs, const Matrix& rhs)
	{
		using namespace VectorOperations;
		assert(lhs.size() == rhs.size());

		// New Matrix is constructed from the rvalue-expression
		return lhs.vectors_ + rhs.vectors_;
	}

	friend Matrix operator-(const Matrix& lhs, const Matrix& rhs)
	{
		static_assert(std::is_signed<T>() || std::is_class<T>(),
			"subtraction is not defined for unsigned integral type");
		
		using namespace VectorOperations;
		assert(lhs.size() == rhs.size());

		// See above
		return lhs.vectors_ - rhs.vectors_;
	}

	// Matrix multiplication
	friend Matrix<T> operator*<T>(const Matrix<T>& lhs, const Matrix<T>& rhs);
	
	// Scalar multiplication
	friend Matrix& operator*(const T scalar, Matrix<T>& rhs)
	{
		for (auto& vector : rhs.vectors_)
		{
			for (T& element : vector)
			{
				element *= scalar;
			}
		}
		return rhs;
	}

	// Matrix to the power of a positive whole number. Returns a new Matrix.
	Matrix power(const int exponent);

	// Computes the trace of the matrix
	T trace()
	{
		// Square matrices only
		assert(col_size_ == row_size_);
		
		T result(0);
		for (unsigned i=0, j=0; i < col_size_; ++i, ++j)
		{
			result += vectors_[i][j];
		}
		return result;
	}
	
	// TODO: Linear algebra

	// Transposes the matrix
	Matrix& transpose();

	// Is used to determine the types of LU etc.
	// Fraction for integrals and float, double etc. for floating points

	using LU_T = 
		std::conditional_t<std::is_floating_point_v<T>, T, Fraction>;

	// TODO: Struct somewhere else?
	
	// Struct for holding result of the LU-factorization
	struct LU
	{
		// Constructor initializes L as square identity matrix and U as
		// the matrix we want to calculate LU for
		explicit LU(const Matrix<LU_T>& mat) :
			L(mat.size().first, fill_type::identity),
			U(mat)
		{}

		// L is square and lower triangular
		Matrix<LU_T> L;

		// U is upper triangular
		Matrix<LU_T> U;
	};

	// Floating point and integral type lu() are not the same. SFINAE is used.
	
	/**
	 * \brief Computes LU-factorization
	 * \return LU-struct with members L and U.
	 */
	template <typename U = T>
	[[nodiscard]]
	std::enable_if_t<!std::is_floating_point_v<U>, LU>
	lu() const
	{
		LU lu(static_cast<Matrix<Fraction>>(*this));
		return compute_lu(lu, 1);
	}

	template <typename U = T>
	[[nodiscard]]
	std::enable_if_t<std::is_floating_point_v<U>, LU>
	lu() const
	{
		LU lu(*this);
		return compute_lu(lu, 1);
	}

	// Equality operators.	

	friend bool operator==(const Matrix& lhs, const Matrix& rhs)
	{
		return lhs.vectors_ == rhs.vectors_;
	}

	friend bool operator!=(const Matrix& lhs, const Matrix& rhs)
	{
		return !(lhs == rhs);
	}

	// Stream operator
	friend std::ostream& operator<< <T>(std::ostream& os, const Matrix<T>& obj);

	
	// Used mostly in unit-testing

	// Checks if all of the elements are certain value
	[[nodiscard]] bool all_of(const T predicate) const;

	// Checks the main diagonal for certain value
	[[nodiscard]] bool if_main_diag(const T predicate) const;

	// Check for upper and lower triangular
	[[nodiscard]] bool is_upper_triangular() const;
	[[nodiscard]] bool is_lower_triangular() const;
	
	// Return size of Matrix as pair
	[[nodiscard]] std::pair<std::size_t, std::size_t> size() const noexcept
	{
		return { col_size_, row_size_ };
	}

private:
	// Matrix is represented as a vector
	std::vector<std::vector<T>> vectors_;

	// Matrix's size
	std::size_t col_size_;
	std::size_t row_size_;

	// TODO: mutable RandLimits
	struct RandLimits
	{
		int min{ 0 };
		int max{ 10 };
	};
	inline static RandLimits rand_limits_;

	// Size-checking (initList / vector constructors)
	[[nodiscard]] bool check_matrix_rows() const;
	
	// Fillers methods
	
	// Form a identity matrix or a nxm matrix with main diagonal
	// as ones.
	void fill_identity();

	// Fills matrix with random whole numbers / reals
	void fill_randi();
	void fill_rand();

	// Combines the functionality of the previous two functions
	template<typename Dist>
	void fill_random(const Dist& number_dist);

	// Recursive functions that compute the LU-fact using Doolittle algorithm.
	// These templates are enabled by return type.

	LU& compute_lu(LU& lu, const unsigned n) const;
};

// Alias for LU-struct
template<typename T>
using LU_t = typename Matrix<T>::LU;

// Less clutter from the definitions
#include "matrix_defs.h"