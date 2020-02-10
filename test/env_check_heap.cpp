#include "malloctest.h"

namespace
{

TEST_F(FTMallocEnvCheckHeapRelaxedTest, EnvMallocCheckHeapMalloc)
{
    void* arr[10];
    for (int i = 0; i < 10; ++i)
    {
        arr[i] = ftmalloc(1);
        EXPECT_NE(arr[i], nullptr);
    }

    for (int i = 0; i < 10; ++i)
    {
        ftfree(arr[i]);
    }
}

}
