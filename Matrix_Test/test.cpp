#include "pch.h"
#include <algorithm>
#include "../matrix.h"

// Matrix sizes as Google Test's constructor can't take arguments
inline const std::size_t N_SIZE = 3;
inline const std::size_t M_SIZE = 4;



template<typename T>
class MatrixTest : public ::testing::Test
{
public:
	MatrixTest() :
		square_(N_SIZE),
		non_square_(N_SIZE, M_SIZE),
		zf_square_(N_SIZE, fill_type::zeros),
		of_non_square_(N_SIZE, M_SIZE, fill_type::ones)
	{}

protected:
	Matrix<T> square_;
	Matrix<T> non_square_;

	// Zero and one filled matrices
	Matrix<T> zf_square_;
	Matrix<T> of_non_square_;
};

using MyTypes = testing::Types<char, int, float>;
TYPED_TEST_CASE(MatrixTest, MyTypes);


TYPED_TEST(MatrixTest, SizeTest)
{
	const auto& square_implement = this->square_.return_vec();
	const auto& non_square_implement = this->non_square_.return_vec();

	// Lambda for checking all rows
	static auto check_row_sizes = [](const std::vector<TypeParam>& row)
	{
		return row.size() == M_SIZE;
	};
	
	// Square
	EXPECT_EQ(square_implement.size(), N_SIZE);
	ASSERT_TRUE(std::all_of(
		square_implement.cbegin(), square_implement.cend(), check_row_sizes));
	
	// Non-square
	EXPECT_EQ(non_square_implement.size(), N_SIZE);
	ASSERT_TRUE(std::all_of(
		non_square_implement.cbegin(), 
		non_square_implement.cend(), 
		check_row_sizes)
	);
}

TYPED_TEST(MatrixTest, FillTest)
{
	// Underlying data type
	using data_type = std::vector<std::vector<TypeParam>>;

	// Lambda that tests all of the elements

	auto static test_all_elements = [](
		const int predicate, const data_type& vec_of_vectors)
	{
		return std::all_of(
			vec_of_vectors.cbegin(), vec_of_vectors.cend(),
			[predicate](const std::vector<TypeParam>& vector)
			{
				return std::all_of(vector.cbegin(), vector.cend(),
					[predicate](const TypeParam element)
					{
						return element == predicate;
					});
			});
	};
	
	// Square
	const auto& zf_vec_vectors = this->zf_square_.return_vec();
	const auto& of_vec_vectors = this->of_non_square_.return_vec();
	
	ASSERT_TRUE(test_all_elements(0, zf_vec_vectors));
	ASSERT_TRUE(test_all_elements(1, of_vec_vectors));
}