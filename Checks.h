// Helper functions that are used by Matrix's initList-constructor
// and SizeTest unit-test.

#pragma once
#include <vector>
#include <algorithm>

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
			[row_predicate](const std::vector<int>& row_vec)
			{
				return row_vec.size() == row_predicate;
			}
		);
	}
};
