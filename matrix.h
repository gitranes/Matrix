#pragma once

#include "pch.h"
#include "VectorOps.h"


namespace RandLimits
{
	inline int min = 0;
	inline int max = 10;
}

enum class fill_type
{
	zeros,
	ones,
	identity,
	randi
};


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


	// std::initializer_list and vector constructors. Vector and initList
	// constructors are used only for rvalues. Implicit conversions are allowed.
	// Size and elements are derived from the initializer list / vectors. 

	Matrix(std::initializer_list<std::vector<T>>&& init_list);
	Matrix(std::vector<std::vector<T>>&& vectors);

	// Destructor and copy and move operations are implicit

	/*
	 * -- Matrix Fill operation --
	 * Fill types are
	 *  > zeros - fills matrix with zeros
	 *  > ones - fills matrix with ones
	 *  > identity - fills main diagonal with ones
	 *  > randi - fills with random integers/reals
	 *    1 one can specify min and max by changing RandLimits::min/max
	 *    2 for char the randi is limited to sensible ASCII chars (41,126)
	 *      the rest is garbage (seperators, null, backspace)
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
	
	// Matrix multiplication
	friend Matrix operator*(const Matrix& lhs, const Matrix& rhs)
	{
		// Matrix multiplication is defined for:
		assert(lhs.row_size_ == rhs.col_size_);

		const auto& lhs_data = lhs.vectors_;
		const auto& rhs_data = rhs.vectors_;

		// New matrix size : NxM * MxP = NxP.
		const auto new_col_size = lhs.col_size_;
		const auto new_row_size = rhs.row_size_;

		// result is initialized to zero.
		std::vector<std::vector<T>> result
			(new_col_size, std::vector<T>(new_col_size, 0));

		// Matrix multiplication
		for (unsigned i = 0; i < new_col_size; ++i)
		{
			for (unsigned j = 0; j < new_row_size; ++j)
			{
				for (unsigned k = 0; k < lhs.row_size_; ++k)
				{
					result[i][j] += lhs_data[i][k] * rhs_data[k][j];
				}
			}
		}
		return result;
	}

	// TODO: Matrix operations (Linear Algebra)
	
	// Relational operations

	friend bool operator==(const Matrix& lhs, const Matrix& rhs)
	{
		return lhs.vectors_ == rhs.vectors_;
	}

	friend bool operator!=(const Matrix& lhs, const Matrix& rhs)
	{
		return !(lhs == rhs);
	}

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

	// Stream operator
	friend std::ostream& operator<<(std::ostream& os, const Matrix& obj)
	{
		bool float_format = false;
		// Change state of the stream
		if constexpr (std::is_floating_point<T>())
		{
			os << std::fixed << std::setprecision(2) << std::setfill(' ');
			float_format = true;
		} else {
			os << std::setfill(' ');
		}
		
		for (const auto& vec : obj.vectors_)
		{
			os << std::endl << "|";
			for (const T& element : vec)
			{
				if (float_format)
				{
					os << " " << std::setw(5) << element << " " << std::setw(2);
				}
				else {
					os << std::setw(4) << element << std::setw(4);
				}
			}
			os << " |" << std::endl;
		}
		return os;
	}

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
};

// Less clutter from the definitions
#include "matrix_defs.h"