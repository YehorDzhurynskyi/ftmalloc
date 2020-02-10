#include "malloctest.h"

namespace
{

bool is_scribbled(char* a, size_t size, int at, char tag)
{
    bool scribbled = true;
    for (int i = at; i < size; ++i)
    {
        if (a[i] != tag)
        {
            scribbled = false;
            break;
        }
    }
    return scribbled;
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleMalloc)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    void* b = ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_NE(b, nullptr);

    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);

    ftfree(a);

    EXPECT_EQ(is_scribbled(a, size, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleReallocGrowNoExtend)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    void* b = ftmalloc(size);
    EXPECT_NE(b, nullptr);
    EXPECT_NE(a, nullptr);

    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);

    char* aa = (char*)ftrealloc(a, size * 2);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_EQ(is_scribbled(aa, size * 2, 0, 0xfa), true);

    ftfree(aa);

    EXPECT_EQ(is_scribbled(aa, size * 2, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleReallocGrowNoExtendContent)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);
    void* b = ftmalloc(size);
    EXPECT_NE(b, nullptr);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, size, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, size * 2);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, size * 2, sizeof(text), 0xfa), true);

    ftfree(aa);

    EXPECT_EQ(is_scribbled(aa, size * 2, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleReallocGrowExtend)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);
    char* next = (char*)ftmalloc(size);
    EXPECT_NE(next, nullptr);
    void* b = ftmalloc(size);
    EXPECT_NE(b, nullptr);

    ftfree(next);

    char* aa = (char*)ftrealloc(a, size * 2);
    EXPECT_EQ(aa, a);
    EXPECT_EQ(is_scribbled(aa, size * 2, 0, 0xfa), true);

    ftfree(aa);

    EXPECT_EQ(is_scribbled(aa, size * 2, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleReallocGrowExtendContent)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);
    char* next = (char*)ftmalloc(size);
    EXPECT_NE(next, nullptr);
    void* b = ftmalloc(size);
    EXPECT_NE(b, nullptr);

    ftfree(next);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, size, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, size * 2);
    EXPECT_EQ(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, size * 2, sizeof(text), 0xfa), true);

    ftfree(aa);

    EXPECT_EQ(is_scribbled(aa, size * 2, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleSmallBucketUpperBoundExceedContent)
{
    char* b = (char*)ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL);

    char* a = (char*)ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_CHUNK_SZ);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_CHUNK_SZ, 0, 0xfa), true);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_CHUNK_SZ, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL, sizeof(text), 0xfa), true);

    ftfree(aa);

    // EXPECT_EQ(is_scribbled(aa, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleMediumBucketUpperBoundExceedContent)
{
    char* b = (char*)ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM);

    char* a = (char*)ftmalloc(FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_CHUNK_SZ);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_CHUNK_SZ, 0, 0xfa), true);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_CHUNK_SZ, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_STREQ(aa, text);
    // EXPECT_EQ(is_scribbled(a, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL, FTMALLOC_MIN_SZ, 0xfd), true);
    EXPECT_EQ(is_scribbled(aa, FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM, sizeof(text), 0xfa), true);

    ftfree(aa);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleLargeBucketUpperBoundExceedContent)
{
    char* a = (char*)ftmalloc(getpagesize() - FTMALLOC_CHUNK_SZ - FTMALLOC_BIN_SZ);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, getpagesize() - FTMALLOC_CHUNK_SZ - FTMALLOC_BIN_SZ, 0, 0xfa), true);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, getpagesize() - FTMALLOC_CHUNK_SZ - FTMALLOC_BIN_SZ, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, getpagesize() - FTMALLOC_CHUNK_SZ - FTMALLOC_BIN_SZ + 1);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, getpagesize() - FTMALLOC_CHUNK_SZ - FTMALLOC_BIN_SZ + 1, sizeof(text), 0xfa), true);

    ftfree(aa);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleReallocShrink)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);
    void* b = ftmalloc(size);
    EXPECT_NE(b, nullptr);

    char* aa = (char*)ftrealloc(a, size / 2);
    EXPECT_EQ(aa, a);
    EXPECT_EQ(is_scribbled(aa, size / 2, 0, 0xfa), true);
    t_mem_chunk* freed = chunk_adj_next(chunk_mem2chunk((t_byte*)aa));
    size_t freed_sz = FTMALLOC_MEM_ALGN_UP(size - freed->prev_size - FTMALLOC_CHUNK_SZ);
    EXPECT_EQ(is_scribbled((char*)chunk_chunk2mem(freed), freed_sz, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(aa);

    EXPECT_EQ(is_scribbled(aa, size, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleReallocShrinkContent)
{
    const int size = 365;
    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);
    char* next = (char*)ftmalloc(size);
    EXPECT_NE(next, nullptr);
    void* b = ftmalloc(size);
    EXPECT_NE(b, nullptr);

    ftfree(next);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, size, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, size / 2);
    EXPECT_EQ(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, size / 2, sizeof(text), 0xfa), true);
    t_mem_chunk* freed = chunk_adj_next(chunk_mem2chunk((t_byte*)aa));
    size_t freed_sz = FTMALLOC_MEM_ALGN_UP(size - freed->prev_size - FTMALLOC_CHUNK_SZ);
    EXPECT_EQ(is_scribbled((char*)chunk_chunk2mem(freed), freed_sz, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(aa);

    EXPECT_EQ(is_scribbled(aa, size, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
}

TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleMediumBucketLowerBoundExceedContent)
{
    const int size = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL + 1;
    char* b = (char*)ftmalloc(size / 2);
    char* bb = (char*)ftmalloc(size);

    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, size, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, size / 2);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, size / 2, sizeof(text), 0xfa), true);

    // t_mem_chunk* freed = chunk_mem2chunk((t_byte*)a);
    // size_t freed_sz = chunk_size_get(freed);

    // EXPECT_EQ(is_scribbled((char*)chunk_chunk2mem(freed), freed_sz, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(aa);

    // EXPECT_EQ(is_scribbled(aa, size, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
    ftfree(bb);
}
TEST_F(FTMallocEnvScribbleTest, EnvMallocScribbleLargeBucketLowerBoundExceedContent)
{
    const int size = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM + 1;
    char* b = (char*)ftmalloc(size / 2);
    char* bb = (char*)ftmalloc(size);

    char* a = (char*)ftmalloc(size);
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(is_scribbled(a, size, 0, 0xfa), true);

    const char text[] = "ROC1ROC2ROC3, my world";
    memcpy(a, text, sizeof(text));
    EXPECT_STREQ(a, text);
    EXPECT_EQ(is_scribbled(a, size, sizeof(text), 0xfa), true);

    char* aa = (char*)ftrealloc(a, size / 2);
    EXPECT_NE(aa, nullptr);
    EXPECT_NE(aa, a);
    EXPECT_STREQ(aa, text);
    EXPECT_EQ(is_scribbled(aa, size / 2, sizeof(text), 0xfa), true);

    // t_mem_chunk* freed = chunk_mem2chunk((t_byte*)a);
    // size_t freed_sz = chunk_size_get(freed);
    // EXPECT_EQ(is_scribbled((char*)chunk_chunk2mem(freed), freed_sz, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(aa);

    // EXPECT_EQ(is_scribbled(aa, size, FTMALLOC_MIN_SZ, 0xfd), true);

    ftfree(b);
    ftfree(bb);
}

}
