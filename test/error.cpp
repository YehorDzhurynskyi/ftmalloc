#include "malloctest.h"

namespace
{

using FTMallocErrorTest = FTMallocTest;

TEST_F(FTMallocErrorTest, Test0)
{
    char    *addr;

    addr = (char*)ftmalloc(16);
	EXPECT_NE(addr, nullptr);
    ftfree(NULL);
    ftfree((char *)addr + 5);
	EXPECT_EQ(ftrealloc((char *)addr + 5, 10), nullptr);
	ftfree(addr);
}

}
