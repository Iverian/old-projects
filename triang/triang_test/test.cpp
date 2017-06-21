#include <gtest/gtest.h>

#include <cls_triangle.h>

struct Delone_test : testing::Test {
};

struct Triangle_test : testing::Test
{
	
};

TEST_F(Triangle_test, TestCreate)
{
	cls_triangle x = { { 155,303 },{ 143, 291 },{ 166, 314 } };
	cls_triangle contr = { {0, 0} , {0, 1}, {1, 0} };
	ASSERT_NE(x.is_exist(), contr.is_exist());
}

TEST_F(Triangle_test, TestDotBelong)
{
	cls_triangle x = { {172, 274}, {143, 291}, {166, 314} };
	cls_dot p = { 155, 303 };
	auto flag = x.dot_belong(p);
	ASSERT_NE(flag, enu_belong::INSIDE);
	ASSERT_NE(flag, enu_belong::OUTSIDE);
}

TEST_F(Delone_test, TestDeloneCheck1)
{
    cls_triangle tri({ 262, 442 }, { 91, 306 }, { 192, 323 });
    cls_dot x{ 436, 323 };
    ASSERT_TRUE(tri.delone_check_dot(x));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
