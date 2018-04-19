#include "gtest/gtest.h"
#include "kgen.hpp"
#include "prime.hpp"

TEST(Prime, BasicOperation) {
	prime_gen g;
	EXPECT_EQ(2, *g);
	EXPECT_EQ(3, *++g);
	EXPECT_EQ(5, *++g);
	EXPECT_EQ(7, *++g);
	EXPECT_EQ(11, *++g);
	EXPECT_EQ(13, *++g);
}
