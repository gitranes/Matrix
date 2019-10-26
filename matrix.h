#pragma once
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>

#include "Checks.h"

enum class fill_type
{
	zeros,
	ones
};


template<typename T> 
class Matrix
{
public:
	// Non-initializing constructors:

	// Square Matrix constructor
	explicit Matrix<T>(const std::size_t n) :
		vector_(n, std::vector<T>(n)),
		col_size_(n),
		row_size_(n)
	{
		static_assert(std::is_arithmetic<T>());
	}

	// Non-square Matrix constructor
	explicit Matrix<T>(const std::size_t n, const std::size_t m) :
		vector_(n, std::vector<T>(m)),
		col_size_(n),
		row_size_(m)
	{
		static_assert(std::is_arithmetic<T>());
	}


	// Filling constructors:

	// Fills a square Matrix with the fill_type
	explicit Matrix<T>(const std::size_t n, fill_type fill_type) :
		vector_(n, std::vector<T>(n, static_cast<T>(fill_type))),
		col_size_(n),
		row_size_(n)
	{
		static_assert(std::is_arithmetic<T>());
	}

	// Fill a non-square Matrix with the fill_type
	explicit Matrix<T>(
		const std::size_t n, const std::size_t m, fill_type fill_type) :
		vector_(n, std::vector<T>(m, static_cast<T>(fill_type))),
		col_size_(n),
		row_size_(m)
	{
		static_assert(std::is_arithmetic<T>());
	}


	// std::initializer_list constructor

	// Size and elements are derived from the initializer list
	explicit Matrix<T>(std::initializer_list<std::vector<T>> init_list) :
		vector_(std::move(init_list)),
		col_size_(init_list.size()),
		row_size_(init_list.begin()->size())
	{
		static_assert(std::is_arithmetic<T>());

		// Assert that the i-list's sizes are consistent.
		//assert(Checks::check_matrix_rows(this));
	}

	// Destructor and copy and move operations are implicit

	// Return size of Matrix as pair
	[[nodiscard]] std::pair<std::size_t, std::size_t> size() const noexcept
	{
		return { col_size_, row_size_ };
	}

	// Const-ref to underlying data
	[[nodiscard]] const std::vector<std::vector<T>>& get_data() const
	{
		return vector_;
	}

private:
	// Matrix is represented as a vector
	std::vector<std::vector<T>> vector_;

	// Matrix's size
	std::size_t col_size_;
	std::size_t row_size_;
};

