// Google test file

#include "pch.h"

#include "../matrix.h"
#include "../Helpers.h"


namespace MatrixTests
{
	// Matrix sizes as Google Test's constructor can't take arguments
	inline const std::size_t N_SIZE = 3;
	inline const std::size_t M_SIZE = 5;

	// initializer-list for test case (5x5 matrix I)
	inline const std::initializer_list<std::vector<int>>
		I_LIST = {
					{1, 0, 0, 0, 0},
					{0, 1, 0, 0, 0},
					{0, 0, 1, 0, 0},
					{0, 0, 0, 1, 0},
					{0, 0, 0, 0, 1}
	};

	// Helper templates for unit-tests.
	
	template<typename T>
	bool check_mat_size_help(const Matrix<T>& mat)
	{
		const auto& mat_data = mat.get_data();
		const auto& size_pair = mat.size();
		
		return Checks::check_matrix_size(size_pair, mat_data);
	}

	template<typename T>
	bool test_all_elements(const T predicate, const Matrix<T>& mat)
	{
		const auto& mat_data = mat.get_data();
		
		return std::all_of(
			mat_data.cbegin(), mat_data.cend(),
			[predicate](const std::vector<T>& vector)
			{
				return std::all_of(vector.cbegin(), vector.cend(),
					[predicate](const T element)
					{
						return element == predicate;
					});
			}
		);
	};

	// Tests for main diagonal for the predicate
	template<typename T>
	auto static test_main_diag (
		const T predicate, const Matrix<T>& mat)
	{
		const auto& mat_data = mat.get_data();

		unsigned i = 0;
		for (const auto& vector : mat_data)
		{
			// vector.size() is row_size
			if (i > vector.size() - 1) break;

			if (vector[i] != predicate) return false;
			++i;
		}
		return true;
	};
	
	
	// Test class
	template<typename T>
	class MatrixTest : public ::testing::Test
	{
	public:
		MatrixTest() :
			square_(N_SIZE),
			non_square_(N_SIZE, M_SIZE),
			non_square2_(M_SIZE, N_SIZE),
			i_list_mat_(I_LIST)
		{}

	protected:
		Matrix<T> square_;
		Matrix<T> non_square_;
		Matrix<T> non_square2_;

		// initList initialized Matrix
		Matrix<int> i_list_mat_;
	};

	// SizeTest and RelationalTest is ran with int only.
	TYPED_TEST_CASE_P(MatrixTest);

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

		// Death tests with erroneous initLists;
		ASSERT_DEATH(Matrix<int> bad_init({{1, 2},{1, 2, 3}}),
			"^Assertion failed");
		
		ASSERT_DEATH(
			Matrix<int> bad_init({
				{1, 2, 3, 4},
				{1, 2},
				{1, 2, 3},
				{1, 2, 3, 4}
			}), "^Assertion failed"
		);
	}

	TYPED_TEST_P(MatrixTest, RelationalTest)
	{
		using matrixType = Matrix<TypeParam>;

		matrixType& sq_mat_of = this->square_.fill(fill_type::ones);
		matrixType& nsq_mat = this->non_square_.fill(fill_type::ones);

		// Copies and 2x3 matrix
		auto sq_mat_zf = sq_mat_of;
		auto nsq_mat_zf = nsq_mat;
		auto mat2_by3 = matrixType(2, 3, fill_type::zeros);

		sq_mat_zf.fill(fill_type::zeros);
		nsq_mat_zf.fill(fill_type::zeros);

		// Test == and !=
		ASSERT_EQ(sq_mat_zf, sq_mat_zf);
		ASSERT_EQ(nsq_mat, nsq_mat);
		ASSERT_NE(sq_mat_zf, nsq_mat_zf);

		// Test >, >=, < an <=
		ASSERT_GT(sq_mat_of, sq_mat_zf);
		ASSERT_LT(sq_mat_zf, sq_mat_of);
		ASSERT_GE(nsq_mat_zf, sq_mat_zf);
		ASSERT_LE(sq_mat_zf, nsq_mat_zf);

		// Test >
		ASSERT_GT(sq_mat_of, mat2_by3);
		ASSERT_GT(nsq_mat_zf, mat2_by3);
	}

	// Instantiate SizeTest and RelationalTest with int
	REGISTER_TYPED_TEST_CASE_P(MatrixTest, SizeTest, RelationalTest);
	INSTANTIATE_TYPED_TEST_CASE_P(MatrixIntTests, MatrixTest, int);

	// Rest of the unit-tests are tested with plenty of types
	using MyTypes = testing::Types<unsigned, int, char, double>;
	TYPED_TEST_CASE(MatrixTest, MyTypes);

	
	TYPED_TEST(MatrixTest, FillTest)
	{
		// Types
		using matrix_type = Matrix<TypeParam>;

		// zf = zero-filled
		// of = one-filled
		// id = identity

		// Matrix objects
		matrix_type& sq_mat_zf = this->square_.fill(fill_type::zeros);
		matrix_type& nsq_mat_of = this->non_square_.fill(fill_type::ones);
		matrix_type& nsq_mat_id = this->non_square2_;

		// Create a copies. These matrices will be identity filled.
		matrix_type sq_mat_id = sq_mat_zf;
		matrix_type nsq_mat_id2 = nsq_mat_of;
		
		// Fill the identity matrices
		sq_mat_id.fill(fill_type::identity);
		nsq_mat_id.fill(fill_type::identity);
		nsq_mat_id2.fill(fill_type::identity);

		ASSERT_TRUE(test_all_elements(TypeParam(0), sq_mat_zf));
		ASSERT_TRUE(test_all_elements(TypeParam(1), nsq_mat_of));
		ASSERT_TRUE(test_main_diag(TypeParam(1), sq_mat_id));
		ASSERT_TRUE(test_main_diag(TypeParam(1), nsq_mat_id));
		ASSERT_TRUE(test_main_diag(TypeParam(1), nsq_mat_id2));
	}

	
	TYPED_TEST(MatrixTest, ArithmeticTest)
	{
		// Types
		using matrix_type = Matrix<TypeParam>;

		// zf = zero-filled
		// of = one-filled
		// id = identity

		// Matrix objects
		matrix_type& sq_mat_zf = this->square_.fill(fill_type::zeros);
		matrix_type& nsq_mat_of = this->non_square_.fill(fill_type::ones);

		matrix_type& nsq_mat_id = this->non_square2_.fill(fill_type::identity);

		// Create square identity
		auto sq_mat_id = sq_mat_zf;
		sq_mat_id.fill(fill_type::identity);

		// Sum of zero-matrices is zero-matrix
		matrix_type zero_mat = sq_mat_zf + sq_mat_zf;
		ASSERT_TRUE(test_all_elements(TypeParam(0), zero_mat));
		
		// Sum should be identity matrix
		matrix_type identity = sq_mat_zf + sq_mat_id;
		ASSERT_TRUE(test_main_diag(TypeParam(1), identity));
		
		// Sum of identities / main diagonal 1's
		sq_mat_id += sq_mat_id;
		nsq_mat_id += nsq_mat_id;
		ASSERT_TRUE(test_main_diag(TypeParam(2), sq_mat_id));
		ASSERT_TRUE(test_main_diag(TypeParam(2), nsq_mat_id));

		// Should be filled with 2's
		nsq_mat_of += nsq_mat_of;
		ASSERT_TRUE(test_all_elements(TypeParam(2), nsq_mat_of));
		
		// Matrices with erroneous sizes.
		ASSERT_DEATH(sq_mat_id += nsq_mat_id, "^Assertion failed");
		ASSERT_DEATH(sq_mat_id + nsq_mat_id, "^Assertion failed");

		// Test - operation for signed types
		if constexpr(std::is_signed<TypeParam>())
		{
			// matrix - itself = zero
			sq_mat_id -= sq_mat_id;
			ASSERT_TRUE(test_all_elements(TypeParam(0), sq_mat_id));

			// zero - zero = zero
			matrix_type zero_mat2 = sq_mat_zf - sq_mat_zf;
			ASSERT_TRUE(test_all_elements(TypeParam(0), zero_mat2));
		}
	}


	// Commented out for now because the test clutters Google-test screen
	/*
	// This test can only be graded visually
	TYPED_TEST(MatrixTest, OutputRandTest)
	{
		using matrix_type = Matrix<TypeParam>;
		
		matrix_type& sq_mat = this->square_;
		matrix_type& nsq_mat = this->non_square_;

		sq_mat.fill(fill_type::randi);
		nsq_mat.fill(fill_type::randi);

		std::cout << sq_mat << '\n' << nsq_mat << '\n' << std::endl;
	}
	*/
}