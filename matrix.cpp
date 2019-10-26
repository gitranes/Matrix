// 
// Most of the Matrix-class defitinions are provided here.
//
// This .cpp is not project file so that the linker doesn't explode from
// multiple definitions. By providing the definitions here and including
// this file at the end of matrix.h, one can get rid of all the definition
// clutter in the header file.
//

#include "pch.h"
#include <algorithm>

#include "matrix.h"
#include "Checks.h"

template <typename T>
Matrix<T>::Matrix(const std::size_t n) :
	vectors_(n, std::vector<T>(n)),
	col_size_(n),
	row_size_(n)
{
	static_assert(std::is_arithmetic<T>());
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const std::size_t m):
	vectors_(n, std::vector<T>(m)),
	col_size_(n),
	row_size_(m)
{
	static_assert(std::is_arithmetic<T>());
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const fill_type fill_type):
	vectors_(n, std::vector<T>(n)),
	col_size_(n),
	row_size_(n)
{
	static_assert(std::is_arithmetic<T>());
	fill(fill_type);
}

template <typename T>
Matrix<T>::Matrix(const std::size_t n, const std::size_t m, fill_type fill_type):
	vectors_(n, std::vector<T>(m)),
	col_size_(n),
	row_size_(m)
{
	static_assert(std::is_arithmetic<T>());
	fill(fill_type);
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>> init_list):
	vectors_(std::move(init_list)),
	col_size_(init_list.size()),
	row_size_(init_list.begin()->size())
{
	static_assert(std::is_arithmetic<T>());

	// Assert that the i-list's sizes are consistent.
	assert(Checks::check_matrix_rows(row_size_, vectors_));
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& obj)
{
	for (const auto& vec : obj.vectors_)
	{
		os << "| ";
		for (const T& element : vec)
		{
			os << element << " ";
		}
		os << "|" << std::endl;
	}
	return os;
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
void Matrix<T>::fill_identity()
{
	unsigned i = 0;
	for (auto& vector : vectors_)
	{
		// Matrices where col_size > row_size
		if (i > row_size_ - 1) break;

		vector[i] = 1;
		++i;
	}
}

template <typename T>
void Matrix<T>::fill_randi()
{
	static std::random_device rd;
	static std::mt19937 rand_eng(rd());
	const int min = RandLimits::min;
	const int max = RandLimits::max;

	// Wish I had concepts

	// Real random numbers
	if constexpr (std::is_same<T, float>() || std::is_same<T, double>())
	{
		const float f_min = static_cast<float>(min);
		const float f_max = static_cast<float>(max);

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
