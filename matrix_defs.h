// 
// Most of the Matrix-class definitions are provided here.
//
// By providing the definitions here and including this file at the end of
// matrix.h, one can get rid of all the definition clutter in the header file.
//

#pragma once

#include <algorithm>
#include "matrix.h"

template <typename T>
Matrix<T>::Matrix(const std::size_t n) :
	vectors_(n, std::vector<T>(n)),
	col_size_(n),
	row_size_(n)
{
	static_assert(std::is_arithmetic<T>(), 
		"Only arithmetic types are supported");
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const std::size_t m) :
	vectors_(n, std::vector<T>(m)),
	col_size_(n),
	row_size_(m)
{
	static_assert(std::is_arithmetic<T>(),
		"Only arithmetic types are supported");
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const fill_type fill_type) :
	vectors_(n, std::vector<T>(n)),
	col_size_(n),
	row_size_(n)
{
	static_assert(std::is_arithmetic<T>(),
		"Only arithmetic types are supported");
	fill(fill_type);
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const std::size_t m, fill_type fill_type) :
	vectors_(n, std::vector<T>(m)),
	col_size_(n),
	row_size_(m)
{
	static_assert(std::is_arithmetic<T>(),
		"Only arithmetic types are supported");
	fill(fill_type);
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>> init_list) :
	vectors_(init_list),
	col_size_(init_list.size()),
	row_size_(init_list.begin()->size())
{
	static_assert(std::is_arithmetic<T>(),
		"Only arithmetic types are supported");

	// Assert that the i-lists' sizes are consistent.
	assert(check_matrix_rows());
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& vectors) :
	vectors_(std::move(vectors)),
	col_size_(vectors_.size()),
	row_size_(vectors_.begin()->size())
{
	static_assert(std::is_arithmetic<T>(),
		"Only arithmetic types are supported");

	// Assert that the vectors' sizes are consistent.
	assert(check_matrix_rows());
}

template <typename T>
Matrix<T>& Matrix<T>::fill(fill_type fill_type)
{
	// 0 and 1 are zero-fill and ones-fill.
	if (fill_type <= fill_type::ones)
	{
		for (auto& vector : vectors_)
		{
			std::fill(vector.begin(), vector.end(),
				static_cast<T>(fill_type));
		}
	}
	else if (fill_type == fill_type::identity)
	{
		fill_identity();
	}
	else if (fill_type == fill_type::randi)
	{
		fill_randi();
	}
	else if (fill_type == fill_type::rand)
	{
		fill_rand();
	}
	return *this;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	// Matrix multiplication is defined for:
	assert(lhs.row_size_ == rhs.col_size_);

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
				result[i][j] += lhs.vectors_[i][k] * rhs.vectors_[k][j];
			}
		}
	}
	return result;
}

template <typename T>
bool Matrix<T>::check_matrix_rows() const 
{
	return std::all_of(
		vectors_.cbegin(), vectors_.cend(),
		[row_size = row_size_](const std::vector<T>& row_vec)
		{
			return row_vec.size() == row_size;
		}
	);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& obj)
{
	bool float_format = false;

	// Change state of the stream
	if constexpr (std::is_floating_point<T>())
	{
		os << std::fixed << std::setprecision(2) << std::setfill(' ');
		float_format = true;
	}
	else
	{
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
			else
			{
				os << std::setw(4) << element << std::setw(4);
			}
		}
		os << " |" << std::endl;
	}
	return os;
}

template <typename T>
bool Matrix<T>::all_of(const T predicate) const
{
	return std::all_of(
		vectors_.cbegin(), vectors_.cend(),
		[predicate](const std::vector<T>& vector)
		{
			return std::all_of(vector.cbegin(), vector.cend(),
			[predicate](const T element)
	        {
				return element == predicate;
	        });
		}
	);
}

template <typename T>
bool Matrix<T>::if_main_diag(const T predicate) const
{
	unsigned i = 0;
	for (const auto& vector : vectors_)
	{
		// vector.size() is row_size
		if (i >= vector.size()) break;

		if (vector[i] != predicate) return false;
		++i;
	}
	return true;
}

template <typename T>
void Matrix<T>::fill_identity()
{
	// TODO: Bench current impl. vs for-loop
	
	// Zero-init a new vectors_
	std::vector<std::vector<T>> result(col_size_, std::vector<T>(row_size_));
	
	unsigned i = 0;
	for (auto& vector : result)
	{
		// Matrices where col_size > row_size
		if (i >= row_size_) break;

		vector[i] = 1;
		++i;
	}
	// Replace old
	vectors_ = result;
}

template <typename T>
template <typename Dist>
void Matrix<T>::fill_random(const Dist& number_dist) 
{
	static std::random_device rd;
	static std::mt19937 rand_eng(rd());
	
	for (auto& vector : vectors_)
	{
		for (T& element : vector)
		{
			element = static_cast<T>(number_dist(rand_eng));
		}
	}
}

template <typename T>
void Matrix<T>::fill_randi()
{
	int min = RandLimits::min;
	int max = RandLimits::max;

	if constexpr (std::is_same<T, char>())
	{
		// Rest of the ASCII range is unprintable
		min = 41;
		max = 126;
	}
	static std::uniform_int_distribution<int> uid(min, max);
	fill_random(uid);
}

template <typename T>
void Matrix<T>::fill_rand()
{
	const auto min = static_cast<float>(RandLimits::min);
	const auto max = static_cast<float>(RandLimits::max);

	static std::uniform_real_distribution<float> uid(min, max);
	fill_random(uid);
}
