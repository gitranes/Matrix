#pragma once

// Helper functions: Check-functions and std::vector operations

#pragma once
#include <vector>
#include <algorithm>
#include <cassert>

namespace Checks
{	
	// Used only by SizeTest
	bool check_matrix_size(
		const std::pair<std::size_t, std::size_t>& size_pair,
		const std::vector<std::vector<int>>& matrix_data
	);

	template<typename T>
	bool check_matrix_rows(
		const std::size_t row_predicate,
		const std::vector<std::vector<T>>& matrix_data)
	{
		return std::all_of(
			matrix_data.cbegin(), matrix_data.cend(),
			[row_predicate](const std::vector<T>& row_vec)
			{
				return row_vec.size() == row_predicate;
			}
		);
	}
};


namespace VectorOperations
{
	// std::plus and minus require constexpr operations...

	template<typename T>
	struct Plus
	{
		T operator()(const T& lhs, const T& rhs)
		{
			return lhs + rhs;
		}
	};

	template<typename T>
	struct Minus
	{
		T operator()(const T& lhs, const T& rhs)
		{
			return lhs - rhs;
		}
	};

	// lhs contains the sums of the elements of lhs and rhs 
	template<typename T>
	std::vector<T>& operator+=(std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		assert(lhs.size() == rhs.size());

		std::transform(lhs.begin(), lhs.end(), rhs.cbegin(),
			lhs.begin(), Plus<T>());

		return lhs;
	}

	// result vector contains the sums of the two vectors
	template<typename T>
	std::vector<T> operator+(
		const std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		assert(lhs.size() == rhs.size());

		std::vector<T> result;
		result.reserve(lhs.size());

		std::transform(lhs.begin(), lhs.end(), rhs.cbegin(),
			std::back_inserter(result), Plus<T>());

		return result;
	}

	// See +=, T has to be signed
	template<typename T>
	std::vector<T>& operator-=(std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		static_assert(std::is_signed<T>() || std::is_class<T>());
		assert(lhs.size() == rhs.size());

		std::transform(lhs.begin(), lhs.end(), rhs.cbegin(),
			lhs.begin(), Minus<T>());

		return lhs;
	}

	// See +, T has to be signed
	template<typename T>
	std::vector<T> operator-(
		const std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		static_assert(std::is_signed<T>() || std::is_class<T>());
		assert(lhs.size() == rhs.size());

		std::vector<T> result;
		result.reserve(lhs.size());

		std::transform(lhs.begin(), lhs.end(), rhs.cbegin(),
			std::back_inserter(result), Minus<T>());

		return result;
	};
}

