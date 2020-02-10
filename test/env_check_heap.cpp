#include "malloctest.h"

namespace
{

TEST_F(FTMallocEnvCheckHeapRelaxedTest, EnvMallocCheckHeap)
{
    void* arr[10];
    for (int i = 0; i < 10; ++i)
    {
        arr[i] = ftmalloc(1);
        EXPECT_NE(arr[i], nullptr);
    }

    EXPECT_EQ(ftmalloc_check_heap_relaxed(), true);

    for (int i = 0; i < 10; ++i)
    {
        if (i % 2 != 0)
        {
            ftfree(arr[i]);
        }
    }

    EXPECT_EQ(ftmalloc_check_heap_relaxed(), true);

    for (int i = 0; i < 10; ++i)
    {
        if (i % 2 == 0)
        {
            ftfree(arr[i]);
        }
    }

    EXPECT_EQ(ftmalloc_check_heap_relaxed(), true);
}

TEST_F(FTMallocEnvCheckHeapFullyTest, EnvMallocCheckHeap)
{
    void* arr[10];
    for (int i = 0; i < 10; ++i)
    {
        arr[i] = ftmalloc(1);
        EXPECT_NE(arr[i], nullptr);
    }

    EXPECT_EQ(ftmalloc_check_heap_fully(), true);

    for (int i = 0; i < 10; ++i)
    {
        if (i % 2 != 0)
        {
            ftfree(arr[i]);
        }
    }

    EXPECT_EQ(ftmalloc_check_heap_fully(), true);

    for (int i = 0; i < 10; ++i)
    {
        if (i % 2 == 0)
        {
            ftfree(arr[i]);
        }
    }

    EXPECT_EQ(ftmalloc_check_heap_fully(), true);
}

}
