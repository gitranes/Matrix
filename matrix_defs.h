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
	static_assert(std::is_arithmetic<T>());
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const std::size_t m) :
	vectors_(n, std::vector<T>(m)),
	col_size_(n),
	row_size_(m)
{
	static_assert(std::is_arithmetic<T>());
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const fill_type fill_type) :
	vectors_(n, std::vector<T>(n)),
	col_size_(n),
	row_size_(n)
{
	static_assert(std::is_arithmetic<T>());
	fill(fill_type);
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const std::size_t m, fill_type fill_type) :
	vectors_(n, std::vector<T>(m)),
	col_size_(n),
	row_size_(m)
{
	static_assert(std::is_arithmetic<T>());
	fill(fill_type);
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>>&& init_list) :
	vectors_(init_list),
	col_size_(init_list.size()),
	row_size_(init_list.begin()->size())
{
	static_assert(std::is_arithmetic<T>());

	// Assert that the i-lists' sizes are consistent.
	assert(check_matrix_rows());
}

template <typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& vectors) :
	vectors_(vectors),
	col_size_(vectors.size()),
	row_size_(vectors.begin()->size())
{
	static_assert(std::is_arithmetic<T>());

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
	return *this;
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
	unsigned i = 0;
	for (auto& vector : vectors_)
	{
		// Matrices where col_size > row_size
		if (i >= row_size_) break;

		vector[i] = 1;
		++i;
	}
}

template <typename T>
void Matrix<T>::fill_randi()
{
	static std::random_device rd;
	static std::mt19937 rand_eng(rd());
	int min = RandLimits::min;
	int max = RandLimits::max;

	if constexpr (std::is_same<T, char>())
	{
		// Rest of the ASCII range is unprintable garbage
		min = 41;
		max = 126;
	}
	// Wish I had concepts

	// Real random numbers
	if constexpr (std::is_floating_point<T>())
	{
		const auto f_min = static_cast<float>(min);
		const auto f_max = static_cast<float>(max);

		using u_dist = std::uniform_real_distribution<float>;
		static u_dist uid(f_min, f_max);

		for (auto& vector : vectors_)
		{
			for (auto& element : vector)
			{
				element = uid(rand_eng);
			}
		}
	}
	else // Whole random numbers
	{
		using u_dist = std::uniform_int_distribution<int>;
		static u_dist uid(min, max);
		for (auto& vector : vectors_)
		{
			for (auto& element : vector)
			{
				element = uid(rand_eng);
			}
		}
	}
}