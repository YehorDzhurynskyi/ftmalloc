#include "malloctest.h"

namespace
{

using FTMallocEvalTest = FTMallocTest;

TEST_F(FTMallocEvalTest, Test0)
{
    int     i;
    char    *addr;

    i = 0;
    while (i < 1024)
    {
        ++i;
    }
}

TEST_F(FTMallocEvalTest, Test1)
{
    int     i;
    char    *addr[1024];

    i = 0;
    while (i < 1024)
    {
        addr[i] = (char*)ftmalloc(1024);
        EXPECT_NE(addr[i], nullptr);
        addr[i][0] = 42;
        i++;
    }
    i = 0;
    while (i < 1024)
    {
        ftfree(addr[i++]);
    }
}

TEST_F(FTMallocEvalTest, Test2)
{
    int     i;
    char    *addr;

    i = 0;
    while (i < 1024)
    {
        addr = (char*)ftmalloc(1024);
        EXPECT_NE(addr, nullptr);
        addr[0] = 42;
        ftfree(addr);
        i++;
    }
}

TEST_F(FTMallocEvalTest, Test3)
{
    const int M = 1024 * 1024;
    char* addr1;
    char* addr3;

    const char text[] = "Bonjours\n";
    addr1 = (char*)ftmalloc(16 * M);
    EXPECT_NE(addr1, nullptr);
    strcpy(addr1, text);
    EXPECT_STREQ(addr1, text);
    addr3 = (char*)ftrealloc(addr1, 128 * M);
    EXPECT_NE(addr3, nullptr);
    EXPECT_NE(addr3, addr1);
    addr3[127 * M] = 42;
    EXPECT_STREQ(addr3, text);
    ftfree(addr3);
}

TEST_F(FTMallocEvalTest, Test3plus)
{
    const int M = 1024 * 1024;
    char* addr1;
    char* addr2;
    char* addr3;

    const char text[] = "Bonjours\n";
    addr1 = (char*)ftmalloc(16 * M);
    EXPECT_NE(addr1, nullptr);
    strcpy(addr1, text);
    EXPECT_STREQ(addr1, text);
    addr2 = (char*)ftmalloc(16 * M);
    EXPECT_NE(addr2, nullptr);
    addr3 = (char*)ftrealloc(addr1, 128 * M);
    EXPECT_NE(addr3, nullptr);
    EXPECT_NE(addr3, addr1);
    addr3[127 * M] = 42;
    EXPECT_STREQ(addr3, text);
    ftfree(addr3);
    ftfree(addr2);
}

TEST_F(FTMallocEvalTest, Test5)
{
    void *a = ftmalloc(1024);
    void *b = ftmalloc(1024 * 32);
    void *c = ftmalloc(1024 * 1024);
    void *d = ftmalloc(1024 * 1024 * 16);
    void *e = ftmalloc(1024 * 1024 * 128);

    EXPECT_NE(a, nullptr);
    EXPECT_NE(b, nullptr);
    EXPECT_NE(c, nullptr);
    EXPECT_NE(d, nullptr);
    EXPECT_NE(e, nullptr);

    show_alloc_mem();

    ftfree(a);
    ftfree(b);
    ftfree(c);
    ftfree(d);
    ftfree(e);
}

}
