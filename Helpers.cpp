#include "pch.h"
#include "Helpers.h"



bool Checks::check_matrix_size(
	const std::pair<std::size_t, std::size_t>& size_pair, 
	const std::vector<std::vector<int>>& matrix_data
)
{;
	const auto [col_size, row_size] = size_pair;

	const bool col_size_check = matrix_data.size() == col_size;
	const bool row_size_check = check_matrix_rows(row_size, matrix_data);

	return col_size_check && row_size_check;
}

