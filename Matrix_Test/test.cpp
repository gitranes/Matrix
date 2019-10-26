#include "pch.h"
#include <algorithm>
#include "../matrix.h"
#include "../Checks.h"


namespace MatrixTests
{
	// Matrix sizes as Google Test's constructor can't take arguments
	inline const std::size_t N_SIZE = 3;
	inline const std::size_t M_SIZE = 4;

	// initializer-list for test case (5x5 matrix I)
	inline const std::initializer_list<std::vector<int>>
		I_LIST = {
					{1, 0, 0, 0, 0},
					{0, 1, 0, 0, 0},
					{0, 0, 1, 0, 0},
					{0, 0, 0, 1, 0},
					{0, 0, 0, 0, 1}
	};

	template<typename T>
	bool check_mat_size_help(const Matrix<T>& mat)
	{
		const auto& mat_data = mat.get_data();
		const auto& size_pair = mat.size();
		
		return Checks::check_matrix_size(size_pair, mat_data);
	}

	// Test class
	template<typename T>
	class MatrixTest : public ::testing::Test
	{
	public:
		MatrixTest() :
			square_(N_SIZE),
			non_square_(N_SIZE, M_SIZE),
			zf_square_(N_SIZE, fill_type::zeros),
			of_non_square_(N_SIZE, M_SIZE, fill_type::ones),
			i_list_mat_(I_LIST)
		{}

	protected:
		Matrix<T> square_;
		Matrix<T> non_square_;

		// Zero and one filled matrices
		Matrix<T> zf_square_;
		Matrix<T> of_non_square_;

		// I-LIST initialized Matrix
		Matrix<int> i_list_mat_;
	};

	// No need to test Size for all types
	TYPED_TEST_CASE_P(MatrixTest);

	// Rest of the unit-tests are tested with plenty of types
	using MyTypes = testing::Types<unsigned, int, char, float>;
	TYPED_TEST_CASE(MatrixTest, MyTypes);


	TYPED_TEST_P(MatrixTest, SizeTest)
	{
		// Matrix objects
		const auto& sq_mat = this->square_;
		const auto& nsq_mat = this->non_square_;
		const auto& i_list_mat = this->i_list_mat_;

		// Assert with a help of template and Checks-function
		ASSERT_TRUE(check_mat_size_help(sq_mat));
		ASSERT_TRUE(check_mat_size_help(nsq_mat));
		ASSERT_TRUE(check_mat_size_help(i_list_mat));
	}

	// Instantiate SizeTest with int
	REGISTER_TYPED_TEST_CASE_P(MatrixTest, SizeTest);
	INSTANTIATE_TYPED_TEST_CASE_P(MatrixIntTests, MatrixTest, int);

	TYPED_TEST(MatrixTest, FillTest)
	{
		// Underlying data type
		using data_type = std::vector<std::vector<TypeParam>>;

		// Lambda that tests all of the elements

		auto static test_all_elements = [](
			const auto predicate, const data_type& vec_of_vectors)
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
		const auto& zf_vec_vectors = this->zf_square_.get_data();
		const auto& of_vec_vectors = this->of_non_square_.get_data();

		ASSERT_TRUE(test_all_elements(0, zf_vec_vectors));
		ASSERT_TRUE(test_all_elements(1, of_vec_vectors));
	}
}