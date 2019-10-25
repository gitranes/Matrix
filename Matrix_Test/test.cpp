#include "pch.h"
#include <algorithm>
#include "../matrix.h"

// Matrix sizes as Google Test's constructor can't take arguments
inline const std::size_t N_SIZE = 3;
inline const std::size_t M_SIZE = 4;

// initializer-list for test case (2x2 matrix I)
std::initializer_list<std::vector<int>> I_LIST = { {1, 0}, {0, 1} };

// Test class
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

// No need to test Size for all types
TYPED_TEST_CASE_P(MatrixTest);

// Rest of the unit-tests are tested with plenty of types
using MyTypes = testing::Types<unsigned, int, char, float>;
TYPED_TEST_CASE(MatrixTest, MyTypes);


TYPED_TEST_P(MatrixTest, SizeTest)
{
	// Underlying data types
	using size_pair = std::pair<std::size_t, std::size_t>;
	using data_type = std::vector<std::vector<int>>;
	
	const auto& square_implement = this->square_.return_vec();
	const auto& non_square_implement = this->non_square_.return_vec();

	// Lambda for checking all rows
	static auto check_row_sizes = [](
		const std::size_t predicate, const data_type& implementation)
	{
		return std::all_of(
			implementation.cbegin(), implementation.cend(),
			[predicate](const std::vector<int>& row)
		{
				return row.size() == predicate;
		});
	};
	
	// Square
	size_pair sq_size_pair = this->square_.size();
	
	EXPECT_EQ(sq_size_pair.first, N_SIZE);
	ASSERT_TRUE(check_row_sizes(
		sq_size_pair.second, square_implement));
	
	// Non-square
	size_pair nsq_size_pair = this->non_square_.size();
	
	EXPECT_EQ(nsq_size_pair.first, N_SIZE);
	ASSERT_TRUE(check_row_sizes(
		nsq_size_pair.second, non_square_implement));
}

TYPED_TEST_P(MatrixTest, InitListTest)
{
	// Underlying data types
	using size_pair = std::pair<std::size_t, std::size_t>;
	using data_type = std::vector<std::vector<int>>;

	const auto& square_implement = this->square_.return_vec();
	const auto& non_square_implement = this->non_square_.return_vec();

	// Lambda for checking all rows
	static auto check_row_sizes = [](
		const std::size_t predicate, const data_type& implementation)
	{
		return std::all_of(
			implementation.cbegin(), implementation.cend(),
			[predicate](const std::vector<int>& row)
			{
				return row.size() == predicate;
			});
	};

	// Square
	size_pair sq_size_pair = this->square_.size();

	EXPECT_EQ(sq_size_pair.first, N_SIZE);
	ASSERT_TRUE(check_row_sizes(
		sq_size_pair.second, square_implement));

	// Non-square
	size_pair nsq_size_pair = this->non_square_.size();

	EXPECT_EQ(nsq_size_pair.first, N_SIZE);
	ASSERT_TRUE(check_row_sizes(
		nsq_size_pair.second, non_square_implement));
}

// Instantiate SizeTest and InitListTest with int
REGISTER_TYPED_TEST_CASE_P(MatrixTest, SizeTest, InitListTest);
INSTANTIATE_TYPED_TEST_CASE_P(MatrixIntTests, MatrixTest, int);

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