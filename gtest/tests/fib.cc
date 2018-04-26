#include "gtest/gtest.h"
#include "kgen.hpp"
#include "fib.hpp"

TEST(Fib, BasicOperation) {
	fib_gen g;
	EXPECT_EQ(1, *g);
	EXPECT_EQ(1, *++g);
	EXPECT_EQ(2, *++g);
	EXPECT_EQ(3, *++g);
	EXPECT_EQ(5, *++g);
	EXPECT_EQ(8, *++g);
	EXPECT_EQ(13, *++g);
	EXPECT_EQ(21, *++g);
	EXPECT_EQ(34, *++g);
	EXPECT_EQ(55, *++g);
	EXPECT_EQ(89, *++g);
	EXPECT_EQ(144, *++g);
	EXPECT_EQ(233, *++g);
}
