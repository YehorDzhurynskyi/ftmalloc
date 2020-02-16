#include "malloctest.h"

namespace
{

TEST_F(FTMallocTest, MallocZeroSize)
{
	EXPECT_EQ(ftmalloc(0), nullptr);
}

TEST_F(FTMallocTest, MallocOverflowSize)
{
	EXPECT_EQ(ftmalloc(-1), nullptr);
}

TEST_F(FTMallocTest, MallocSingleAllocDealloc)
{
	void* a = ftmalloc(1);
	EXPECT_NE(a, nullptr);
	ftfree(a);
}

TEST_F(FTMallocTest, MallocMultipleAllocDealloc)
{
	void* a = ftmalloc(5);
	void* b = ftmalloc(32);
	EXPECT_NE(a, nullptr);
	EXPECT_NE(b, nullptr);
	ftfree(a);
	ftfree(b);
}

}
