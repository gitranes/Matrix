// Google test file

#include "pch.h"
#include "../matrix.h"

// TODO: Test vectors

namespace MatrixTests
{
	// Matrix sizes as Google Test's constructor can't take arguments
	inline const std::size_t N_SIZE = 3;
	inline const std::size_t M_SIZE = 5;

	// initializer-list for test case (5x5 matrix I)
	inline const std::initializer_list<std::initializer_list<int>>
		I_LIST = {
					{1, 0, 0, 0, 0},
					{0, 1, 0, 0, 0},
					{0, 0, 1, 0, 0},
					{0, 0, 0, 1, 0},
					{0, 0, 0, 0, 1}
	};
	
	// Test class
	template<typename T>
	class MatrixGTest : public ::testing::Test
	{
	public:
		MatrixGTest() :
			square_(N_SIZE),
			nsq_3by5_(N_SIZE, M_SIZE),
			nsq_5by3_(M_SIZE, N_SIZE),
			i_list_mat_(I_LIST)
		{}

	protected:
		Matrix<T> square_;
		Matrix<T> nsq_3by5_;
		Matrix<T> nsq_5by3_;

		// initList initialized Matrix
		Matrix<int> i_list_mat_;
	};

	// Only using works for TEST_F-cases MatrixGTest<int> doesn't... why
	using int_typed = MatrixGTest<int>;

	TEST(MatrixGTest, BadSizeTests)
	{
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
	
	TEST_F(int_typed, PowerTest)
	{
		auto& sq_of = square_.fill(fill_type::ones);
		auto sq_id = sq_of;
		sq_id.fill(fill_type::identity);

		// 3x3 ones ^ 2 = 3x3 threes
		auto sq_of_pw2 = sq_of.power(2);
		ASSERT_TRUE(sq_of_pw2.all_of(3));

		// 3x3 id ^ n = id
		auto sq_id_pw4 = sq_id.power(4);
		auto sq_of_pw0 = sq_of.power(0);
		ASSERT_EQ(sq_id, sq_id_pw4);
		ASSERT_EQ(sq_of_pw0, sq_id);

		ASSERT_DEATH(sq_id.power(-4), "^Assertion failed");
	}

	TEST_F(int_typed, TraceTest)
	{
		auto& sq_of = square_.fill(fill_type::ones);
		auto sq_id = sq_of;
		sq_id.fill(fill_type::identity);

		ASSERT_EQ(sq_of.trace(), sq_id.trace());
		ASSERT_EQ(sq_id.trace(), static_cast<int>(sq_id.size().first));
	}
	
	// First tests are ran with signed types
	TYPED_TEST_CASE_P(MatrixGTest);
	

	TYPED_TEST_P(MatrixGTest, EqualityTest)
	{
		using matrix_type = Matrix<TypeParam>;

		matrix_type& sq_mat_of = this->square_.fill(fill_type::ones);
		matrix_type& nsq_mat = this->nsq_3by5_.fill(fill_type::ones);

		// Matrix copies
		auto sq_mat_null = sq_mat_of;
		auto nsq_mat_zf = nsq_mat;

		sq_mat_null.fill(fill_type::zeros);
		nsq_mat_zf.fill(fill_type::zeros);

		// Test == and !=
		ASSERT_EQ(sq_mat_null, sq_mat_null);
		ASSERT_EQ(nsq_mat, nsq_mat);
		ASSERT_NE(sq_mat_null, nsq_mat_zf);
	}

	TYPED_TEST_P(MatrixGTest, SubtractionTest)
	{
		using matrix_type = Matrix<TypeParam>;

		TypeParam null = 0;
		
		// Get square object and create copies
		matrix_type& sq_mat_null = this->square_.fill(fill_type::zeros);
		matrix_type sq_mat_id = sq_mat_null;
		sq_mat_id.fill(fill_type::identity);
		
		// zero - zero = zero
		matrix_type zero_mat2 = sq_mat_null - sq_mat_null;
		ASSERT_TRUE(zero_mat2.all_of(null));

		// matrix - itself = zero
		sq_mat_id -= sq_mat_id;
		ASSERT_TRUE(sq_mat_id.all_of(null));
	}
	
	TYPED_TEST_P(MatrixGTest, LUFactTest)
	{
		using matrix_type = Matrix<TypeParam>;

		matrix_type sq_id(3, fill_type::identity);
		const auto [L, U] = sq_id.lu();
		ASSERT_EQ(L * U, sq_id);
		
		matrix_type mat = { {1, 1, 0},{1, 1, 0},{0, 0, 1} };
		const auto [L2, U2] = mat.lu();
		ASSERT_EQ(L2 * U2, mat);

		matrix_type sq_of(3, fill_type::randi);
		const auto [L3, U3] = sq_of.lu();
		ASSERT_EQ(L3 * U3, sq_of);

		std::cout << L3 << U3 << std::endl;
	}
	
	using SignedTypes = testing::Types<int, double>;
	REGISTER_TYPED_TEST_CASE_P(
		MatrixGTest,
		EqualityTest, SubtractionTest, LUFactTest
	);
	INSTANTIATE_TYPED_TEST_CASE_P(MatrixIntTests, MatrixGTest, SignedTypes);

	
	// Rest of the unit-tests both signed and unsigned types
	using TestTypes = testing::Types<unsigned, int, double>;
	TYPED_TEST_CASE(MatrixGTest, TestTypes);
	
	
	TYPED_TEST(MatrixGTest, FillTest)
	{
		// Types
		using matrix_type = Matrix<TypeParam>;

		TypeParam one = 1;

		// null = zero-filled
		// of = one-filled
		// id = identity

		// MatrixTest objects
		matrix_type& sq_mat_null = this->square_;
		matrix_type & nsq_mat_of = this->nsq_3by5_;
		matrix_type& nsq_mat_id = this->nsq_5by3_;

		// Fill objects
		sq_mat_null.fill(fill_type::zeros);
		nsq_mat_of.fill(fill_type::ones);
		nsq_mat_id.fill(fill_type::identity);

		// Create a copies. These matrices will be identity filled.
		matrix_type sq_mat_id = sq_mat_null;
		matrix_type nsq_mat_id2 = nsq_mat_of;
		
		// Fill the identity matrices
		sq_mat_id.fill(fill_type::identity);
		nsq_mat_id.fill(fill_type::identity);
		nsq_mat_id2.fill(fill_type::identity);

		ASSERT_TRUE(sq_mat_null.all_of(TypeParam(0)));
		ASSERT_TRUE(nsq_mat_of.all_of(one));
		ASSERT_TRUE(sq_mat_id.if_main_diag(one));
		ASSERT_TRUE(nsq_mat_id.if_main_diag(one));
		ASSERT_TRUE(nsq_mat_id2.if_main_diag(one));
	}

	
	TYPED_TEST(MatrixGTest, SumTest)
	{
		// Types
		using matrix_type = Matrix<TypeParam>;

		// Scalar (to prevent ambiguous template parameters)
		TypeParam two = 2;

		// null = zero-filled
		// of = one-filled
		// id = identity

		// Matrix objects
		matrix_type& sq_mat_zf = this->square_.fill(fill_type::zeros);
		matrix_type& nsq_mat_of = this->nsq_3by5_.fill(fill_type::ones);
		matrix_type& nsq_mat_id = this->nsq_5by3_.fill(fill_type::identity);

		// Create square identity
		auto sq_mat_id = sq_mat_zf;
		sq_mat_id.fill(fill_type::identity);

		// Sum of zero-matrices is zero-matrix
		matrix_type zero_mat = sq_mat_zf + sq_mat_zf;
		ASSERT_TRUE(zero_mat.all_of(TypeParam(0)));
		
		// null_mat + identity = identity
		matrix_type identity = sq_mat_zf + sq_mat_id;
		ASSERT_EQ(identity, sq_mat_id);
		
		// Sum of identities / main diagonal 1's = main diagonal 2's
		sq_mat_id += sq_mat_id;
		nsq_mat_id += nsq_mat_id;
		ASSERT_TRUE(sq_mat_id.if_main_diag(two));
		ASSERT_TRUE(sq_mat_id.if_main_diag(two));

		// of_mat + of_mat = two-filled mat
		nsq_mat_of += nsq_mat_of;
		ASSERT_TRUE(nsq_mat_of.all_of(two));
		
		// Matrices with erroneous sizes.
		ASSERT_DEATH(sq_mat_id += nsq_mat_id, "^Assertion failed");
		ASSERT_DEATH(sq_mat_id + nsq_mat_id, "^Assertion failed");
	}

	TYPED_TEST(MatrixGTest, ScalarMultiplicationTest)
	{
		using matrix_type = Matrix<TypeParam>;
		TypeParam null = 0;

		// Create null and randi matrix
		matrix_type sq_mat_null = this->square_;

		auto copy_sq = sq_mat_null;
		copy_sq.fill(fill_type::randi);
		
		// scalar * null_mat = null_mat
		TypeParam(4) * sq_mat_null;
		ASSERT_TRUE(sq_mat_null.all_of(null));

		// null(scalar) * matrix = null_mat
		null * copy_sq;
		ASSERT_TRUE(copy_sq.all_of(null));
	}

	TYPED_TEST(MatrixGTest, MatMultiplicationTest)
	{
		using matrix_type = Matrix<TypeParam>;

		// Scalars
		TypeParam null = 0;
		TypeParam one = 1;
		
		// null = zero-filled
		// of = one-filled
		// id = identity

		// Matrix objects
		matrix_type& sq_mat_null = this->square_.fill(fill_type::zeros);

		matrix_type sq_mat_id = sq_mat_null;
		sq_mat_id.fill(fill_type::identity);

		matrix_type sq_mat_randi = sq_mat_null;
		sq_mat_randi.fill(fill_type::randi);

		matrix_type& nsq_mat_of = this->nsq_3by5_.fill(fill_type::ones);
		matrix_type& nsq_mat_id = this->nsq_5by3_.fill(fill_type::identity);

		// 3x5 (ones) x 5x3 (identity) = 3x3 (ones)
		auto sq_ones = nsq_mat_of * nsq_mat_id;
		ASSERT_TRUE(sq_ones.all_of(one));

		// any matrix x identity = same matrix
		auto sq_mat_randi2 = sq_mat_randi * sq_mat_id;
		ASSERT_EQ(sq_mat_randi2, sq_mat_randi);

		// any matrix x null matrix = null
		auto sq_mat_null2 = sq_mat_randi * sq_mat_null;
		ASSERT_TRUE(sq_mat_null2.all_of(null));
		
		// Incompatible matrices
		ASSERT_DEATH(nsq_mat_id * nsq_mat_id, "^Assertion failed");
		ASSERT_DEATH(nsq_mat_of * sq_mat_null, "^Assertion failed");
	}

	TYPED_TEST(MatrixGTest, TransposeTest)
	{
		using matrix_type = Matrix<TypeParam>;

		matrix_type& sq_mat_id = this->square_.fill(fill_type::identity);
		matrix_type& nsq_mat1 = this->nsq_3by5_;
		matrix_type& nsq_mat2 = this->nsq_5by3_;

		// Identity transposed is identity
		auto sq_mat_id2 = sq_mat_id;
		ASSERT_EQ(sq_mat_id, sq_mat_id2);

		// 3x5 transposed -> 5x3
		nsq_mat1.transpose();
		ASSERT_EQ(nsq_mat1.size(), nsq_mat2.size());

		/*
		// Test visually
		nsq_mat1.fill(fill_type::randi);
		std::cout << nsq_mat1 << std::endl;
		
		nsq_mat1.transpose();
		std::cout << nsq_mat1 << std::endl;
		*/
	}
	
	// Commented out because the test clutters Google-test screen

	/*
	// This test can only be graded visually
	TYPED_TEST(MatrixGTest, OutputRandTest)
	{
		using matrix_type = Matrix<TypeParam>;
		
		matrix_type& sq_mat = this->square_;
		matrix_type& nsq_mat = this->nsq_3by5_;

		sq_mat.fill(fill_type::randi);
		nsq_mat.fill(fill_type::rand);

		auto sq_mat_of = sq_mat;
		sq_mat_of.fill(fill_type::ones);
		sq_mat_of += sq_mat_of;

		std::cout << sq_mat << '\n' << nsq_mat
				  << '\n'<< sq_mat_of << '\n' << std::endl;
	}
	*/

}