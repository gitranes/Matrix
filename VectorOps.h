#pragma once

// std::vector operations

#pragma once
#include <vector>
#include <algorithm>
#include <cassert>


namespace VectorOperations
{	
	// std::plus and std::minus can't find the defined operations so I
	// defined my own
	template<typename T>
	struct Plus
	{
		constexpr T operator()(const T& lhs, const T& rhs)
		{
			return lhs + rhs;
		}
	};

	template<typename T>
	struct Minus
	{
		constexpr T operator()(const T& lhs, const T& rhs)
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

	// See +=, unsigned overflows are not checked.
	template<typename T>
	std::vector<T>& operator-=(std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		assert(lhs.size() == rhs.size());

		std::transform(lhs.begin(), lhs.end(), rhs.cbegin(),
			lhs.begin(), Minus<T>());

		return lhs;
	}

	// See +, unsigned overflows are not checked.
	template<typename T>
	std::vector<T> operator-(
		const std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		assert(lhs.size() == rhs.size());

		std::vector<T> result;
		result.reserve(lhs.size());

		std::transform(lhs.begin(), lhs.end(), rhs.cbegin(),
			std::back_inserter(result), Minus<T>());

		return result;
	};
}