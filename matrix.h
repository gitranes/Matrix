#pragma once
#include "pch.h"

#include "Helpers.h"

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

	
	// Fill operation
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

	// Return size of Matrix as pair
	[[nodiscard]] std::pair<std::size_t, std::size_t> size() const noexcept
	{
		return { col_size_, row_size_ };
	}

	// Const-ref to underlying data
	[[nodiscard]] const std::vector<std::vector<T>>& get_data() const
	{
		return vectors_;
	}

private:
	// Matrix is represented as a vector
	std::vector<std::vector<T>> vectors_;

	// Matrix's size
	std::size_t col_size_;
	std::size_t row_size_;

	
	// Fillers methods
	
	// Form a identity matrix or a nxm matrix with main diagonal
	// as ones.
	void fill_identity();

	// Fills matrix with random whole numbers / reals
	void fill_randi();
};


// Less clutter from the definitions
#include "matrix_defs.h"