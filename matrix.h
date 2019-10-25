#pragma once
#include <vector>
#include <utility>

enum class fill_type
{
	zeros,
	ones
};

template<typename T>
class Matrix
{
public:
	// Non-initialising constructors:
	
	// Square Matrix constructor
	explicit Matrix<T>(const std::size_t n) :
		matrix_(n, std::vector<T>(n)),
		row_size_(n),
		col_size_(n)
	{
		static_assert(std::is_arithmetic<T>());
	}

	// Non-square Matrix constructor
	explicit Matrix<T>(const std::size_t n, const std::size_t m) :
		matrix_(n, std::vector<T>(m)),
		row_size_(m),
		col_size_(n)
	{
		static_assert(std::is_arithmetic<T>());
	}

	
	// Filling constructors:

	// Fills a square Matrix with the fill_type
	explicit Matrix<T>(const std::size_t n, fill_type fill_type) :
		matrix_(n, std::vector<T>(n, static_cast<T>(fill_type))),
		row_size_(n),
		col_size_(n)
	{
		static_assert(std::is_arithmetic<T>());
	}

	// Fill a non-square Matrix with the fill_type
	explicit Matrix<T>(
		const std::size_t n, const std::size_t m, fill_type fill_type) :
			matrix_(n, std::vector<T>(m, static_cast<T>(fill_type))),
			row_size_(m),
			col_size_(n)
	{
		static_assert(std::is_arithmetic<T>());
	}


	// std::initializer_list constructors

	// Size and elements are derived from the initializer list
	explicit Matrix<T>(
		std::initializer_list<std::vector<T>> init_list) :
		matrix_(init_list),
		row_size_(init_list.begin()->size()),
		col_size_(init_list.size())
	{
		static_assert(std::is_arithmetic<T>());
	}
	
	// Information about underlying data:
	
	// Return size of Matrix as pair
	[[nodiscard]] std::pair<std::size_t, std::size_t> size() const
	{
		return { row_size_, col_size_ };
	}

	// const-ref to underlying vector for testing 
	const std::vector<std::vector<T>>& return_vec() const
	{
		return matrix_;
	}


	

	

private:
	// Matrix is represented as a vector
	std::vector<std::vector<T>> matrix_;

	// Matrix size
	std::size_t row_size_;
	std::size_t col_size_;
};


