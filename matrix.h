#pragma once

#include "pch.h"
#include "VectorOps.h"


namespace RandLimits
{
	inline int min = 0;
	inline int max = 10;
}

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


template<typename T> 
class Matrix
{
public:

	// Null-matrix
	Matrix() : col_size_(0), row_size_(0) {};

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
	

	/*Fills the matrix according to the fill_type
	 * Min and max can be specified by changing RandLimits::min/max
	 * For char the randi is limited to sensible ASCII chars (41, 126)
	 */
	Matrix& fill(fill_type fill_type);


	// Arithmetic operations. Declaring operations friend allows implicit
	// conversions both ways, here it has nothing to do with access-specifying.
	// See Helpers -> VectorOperations for the precise implementations.

	
	friend Matrix& operator+=(Matrix& lhs, const Matrix& rhs)
	{
		using namespace VectorOperations;
		assert(lhs.size() == rhs.size());

		lhs.vectors_ += rhs.vectors_;
		return lhs;
	}
	
	friend Matrix& operator-=(Matrix& lhs, const Matrix& rhs)
	{
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
		using namespace VectorOperations;
		assert(lhs.size() == rhs.size());

		// See above
		return lhs.vectors_ - rhs.vectors_;
	}

	// Matrix multiplication
	friend Matrix<T> operator*<T>(const Matrix<T>& lhs, const Matrix<T>& rhs);
	
	// Scalar multiplication
	Matrix& operator*(const T scalar)
	{
		for (auto& vector : vectors_)
		{
			for (T& element : vector)
			{
				element *= scalar;
			}
		}
		return *this;
	}
	
	// TODO: Linear algebra


	// Transposes the matrix
	Matrix& transpose()
	{
		// Construct an empty vector (transposed result)
		std::vector<std::vector<T>> t_vector(
			row_size_, std::vector<T>(col_size_));

		for (unsigned i = 0; i < col_size_; ++i)
		{
			for (unsigned j = 0; j < row_size_; ++j)
			{
				t_vector[j][i] += vectors_[i][j];
			}
		}
		// Replace the old vector and swap the sizes
		vectors_ = t_vector;
		std::swap(col_size_, row_size_);
		
		return *this;
	}

	// Relational and equality operators.	

	friend bool operator<(const Matrix& lhs, const Matrix& rhs)
	{
		return lhs.vectors_ < rhs.vectors_;
	}

	friend bool operator<=(const Matrix& lhs, const Matrix& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>(const Matrix& lhs, const Matrix& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const Matrix& lhs, const Matrix& rhs)
	{
		return !(lhs < rhs);
	}

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
};

// Less clutter from the definitions
#include "matrix_defs.h"