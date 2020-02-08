#include "malloctest.h"

namespace
{

TEST_F(FTMallocTest, ReallocNullPtrZeroSize)
{
    EXPECT_EQ(ftrealloc(nullptr, 0), nullptr);
}

TEST_F(FTMallocTest, ReallocNullPtr)
{
    void *a = ftrealloc(nullptr, 1);
    EXPECT_NE(a, nullptr);
    ftfree(a);
}

TEST_F(FTMallocTest, ReallocAllocDealloc)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    ftrealloc(a, 0);
}

TEST_F(FTMallocTest, Realloc)
{
    void *a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void *b = ftrealloc(a, 2);
    EXPECT_EQ(b, a);

    ftfree(b);
}

TEST_F(FTMallocTest, ReallocFreeNextExtend1)
{
    void *a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(1);
    EXPECT_NE(b, nullptr);
    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(b);
    void* aa = ftrealloc(a, FTMALLOC_MEM_CHUNK_SZ + 1);
    EXPECT_EQ(a, aa);

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreePrevExtend1)
{
    void *a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(1);
    EXPECT_NE(b, nullptr);
    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, FTMALLOC_MEM_CHUNK_SZ + 1);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreeNextExtend2)
{
    void* a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void* b = ftmalloc(1);
    EXPECT_NE(b, nullptr);
    void* c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(b);
    void* aa = ftrealloc(a, FTMALLOC_MEM_CHUNK_SZ * 2 + 1);
    EXPECT_EQ(a, aa);

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreePrevExtend2)
{
    void *a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(1);
    EXPECT_NE(b, nullptr);
    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, FTMALLOC_MEM_CHUNK_SZ * 2 + 1);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreeNextEdgeExtendSmall)
{
    void* a = ftmalloc(96);
    EXPECT_NE(a, nullptr);
    void* b = ftmalloc(16);
    EXPECT_NE(b, nullptr);
    void* c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(b);
    void* aa = ftrealloc(a, 112);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);

    ftfree(aa);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreeNextEdgeExtendLarge)
{
    void* a = ftmalloc(8144);
    EXPECT_NE(a, nullptr);
    void* b = ftmalloc(208);
    EXPECT_NE(b, nullptr);
    void* c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(b);
    void* aa = ftrealloc(a, 8352);
    EXPECT_EQ(a, aa);

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocPageSizeExceed)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(getpagesize());
    EXPECT_NE(b, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, getpagesize() + 1);
    EXPECT_EQ(bb, b);

    ftfree(bb);
}

TEST_F(FTMallocTest, ReallocContentExtend)
{
    char *a = (char*)ftrealloc(NULL, FTMALLOC_MEM_CHUNK_SZ);
    EXPECT_NE(a, nullptr);
    memcpy(a, "ROC1ROC2ROC3", sizeof("ROC1ROC2ROC3") + 1);
    EXPECT_STREQ(a, "ROC1ROC2ROC3");

    char *b = (char*)ftmalloc(48);
    EXPECT_NE(b, nullptr);
    memcpy(b, "HeLLo!", sizeof("HeLLo!") + 1);
    EXPECT_STREQ(b, "HeLLo!");

    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);

    ftfree(b);
    a = (char*)ftrealloc(a, 32);
    EXPECT_NE(a, nullptr);
    strcat(a, ", my world");
    EXPECT_STREQ(a, "ROC1ROC2ROC3, my world");

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocSmallBucketUpperBoundExceed)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ);
    EXPECT_NE(b, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
}

TEST_F(FTMallocTest, ReallocMediumBucketUpperBoundExceed)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_MEM_CHUNK_SZ);
    EXPECT_NE(b, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
}

TEST_F(FTMallocTest, ReallocLargeBucketUpperBoundExceed)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(getpagesize() - FTMALLOC_MEM_CHUNK_SZ - FTMALLOC_MEM_BIN_SZ);
    EXPECT_NE(b, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, getpagesize() - FTMALLOC_MEM_CHUNK_SZ - FTMALLOC_MEM_BIN_SZ + 1);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
}

TEST_F(FTMallocTest, ReallocFreeNextShrink1)
{
    void *a = ftrealloc(NULL, FTMALLOC_MEM_CHUNK_SZ + 1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(1);
    EXPECT_NE(b, nullptr);
    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(b);
    void* aa = ftrealloc(a, 1);
    EXPECT_EQ(a, aa);

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreePrevShrink1)
{
    void *a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(FTMALLOC_MEM_CHUNK_SZ + 1);
    EXPECT_NE(b, nullptr);
    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, 1);
    EXPECT_NE(bb, nullptr);
    EXPECT_EQ(bb, b);

    ftfree(bb);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreeNextShrink2)
{
    void* a = ftrealloc(NULL, FTMALLOC_MEM_CHUNK_SZ * 2 + 1);
    EXPECT_NE(a, nullptr);
    void* b = ftmalloc(1);
    EXPECT_NE(b, nullptr);
    void* c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(b);
    void* aa = ftrealloc(a, 1);
    EXPECT_EQ(a, aa);

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocFreePrevShrink2)
{
    void *a = ftrealloc(NULL, 1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(FTMALLOC_MEM_CHUNK_SZ * 2 + 1);
    EXPECT_NE(b, nullptr);
    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, 1);
    EXPECT_NE(bb, nullptr);
    EXPECT_EQ(bb, b);

    ftfree(bb);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocContentShrink)
{
    char *a = (char*)ftrealloc(NULL, 48);
    EXPECT_NE(a, nullptr);
    memcpy(a, "ROC1ROC2ROC3, my world", sizeof("ROC1ROC2ROC3, my world") + 1);
    EXPECT_STREQ(a, "ROC1ROC2ROC3, my world");

    char *b = (char*)ftmalloc(48);
    EXPECT_NE(b, nullptr);
    memcpy(b, "HeLLo!", sizeof("HeLLo!") + 1);
    EXPECT_STREQ(b, "HeLLo!");

    void *c = ftmalloc(1);
    EXPECT_NE(c, nullptr);

    ftfree(b);
    a = (char*)ftrealloc(a, FTMALLOC_MEM_CHUNK_SZ);
    EXPECT_NE(a, nullptr);
    a[FTMALLOC_MEM_CHUNK_SZ - 1] = '\0';
    EXPECT_STREQ(a, "ROC1ROC2ROC3, m");

    ftfree(a);
    ftfree(c);
}

TEST_F(FTMallocTest, ReallocMediumBucketLowerBoundExceed)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL + 1);
    EXPECT_NE(b, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, 1);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
}

TEST_F(FTMallocTest, ReallocLargeBucketLowerBoundExceed)
{
    void *a = ftmalloc(1);
    EXPECT_NE(a, nullptr);
    void *b = ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM + 1);
    EXPECT_NE(b, nullptr);
    ftfree(a);
    void *bb = ftrealloc(b, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ);
    EXPECT_NE(bb, nullptr);
    EXPECT_NE(bb, b);

    ftfree(bb);
}

}
