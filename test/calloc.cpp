#include "malloctest.h"

namespace
{

bool is_nullified(char* a, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (a[i] != 0x0)
        {
            return false;
        }
    }
    return true;
}

TEST_F(FTMallocTest, CallocZeroSize)
{
	EXPECT_EQ(ftcalloc(1, 0), nullptr);
}

TEST_F(FTMallocTest, CallocZeroCount)
{
    EXPECT_EQ(ftcalloc(0, 1), nullptr);
}

TEST_F(FTMallocTest, CallocZeroSizeCount)
{
    EXPECT_EQ(ftcalloc(0, 0), nullptr);
}

TEST_F(FTMallocTest, CallocOverflowSize)
{
	EXPECT_EQ(ftcalloc(-1, 1), nullptr);
}

TEST_F(FTMallocTest, CallocOverflowCount)
{
    EXPECT_EQ(ftcalloc(1, -1), nullptr);
}

TEST_F(FTMallocTest, CallocOverflowSizeCount)
{
    EXPECT_EQ(ftcalloc(-1, -1), nullptr);
}

TEST_F(FTMallocTest, CallocSingleAllocDealloc)
{
	void* a = ftcalloc(1, 1);
	EXPECT_NE(a, nullptr);

    EXPECT_EQ(is_nullified((char*)a, 1), true);

	ftfree(a);
}

TEST_F(FTMallocTest, CallocMultipleAllocDealloc)
{
	void* a = ftcalloc(5, 5);
	EXPECT_NE(a, nullptr);

    void* b = ftcalloc(3, 32);
	EXPECT_NE(b, nullptr);

    EXPECT_EQ(is_nullified((char*)a, 5 * 5), true);
    EXPECT_EQ(is_nullified((char*)b, 3 * 32), true);

    ftfree(a);
	ftfree(b);
}

}
