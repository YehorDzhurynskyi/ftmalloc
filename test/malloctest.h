extern "C"
{
#include "ftmalloc_internal.h"
#include "ftmalloc.h"
}

#ifdef FTMALLOC_POSIX_API
# define ftmalloc malloc
# define ftcalloc calloc
# define ftrealloc realloc
# define ftfree free
#endif // FTMALLOC_POSIX_API

#include "gtest/gtest.h"

class FTMallocTest : public ::testing::Test
{
protected:
	void TearDown() override
	{
		EXPECT_EQ(g_ftmalloc_state.bin_list_small, nullptr);
		EXPECT_EQ(g_ftmalloc_state.bin_list_medium, nullptr);
		EXPECT_EQ(g_ftmalloc_state.bin_list_large, nullptr);

        auto verifybucket = [](t_mem_bin* binlist)
        {
            t_mem_bin* bin = binlist;
            while (bin)
            {
                size_t occupied = 0;
                size_t user = 0;
                size_t available = 0;

                t_mem_chunk* chunk = bin_adj(bin);
                while (!chunk_is_prev_top(chunk))
                {
                    occupied += (chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0) + FTMALLOC_MEM_CHUNK_SZ;
                    user += chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0;
                    available += chunk_in_use_get(chunk) ? 0 : chunk_size_get(chunk);

                    chunk = chunk_adj_prev(chunk);
                }
                occupied += (chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0) + FTMALLOC_MEM_CHUNK_SZ;
                user += chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0;
                available += chunk_in_use_get(chunk) ? 0 : chunk_size_get(chunk);

                EXPECT_EQ(occupied, bin->mem_allocated);
                EXPECT_EQ(user, bin->mem_user);
                EXPECT_EQ(available, bin->mem_allocated - bin->mem_occupied);

                bin = bin->next;
            }
        };

        verifybucket(g_ftmalloc_state.bin_list_small);
        verifybucket(g_ftmalloc_state.bin_list_medium);
        verifybucket(g_ftmalloc_state.bin_list_large);

# ifdef FTMALLOC_DEBUG
		EXPECT_EQ(g_ftmalloc_state.usage_alloc, g_ftmalloc_state.usage_dealloc);
		EXPECT_EQ(g_ftmalloc_state.total_mmap, g_ftmalloc_state.total_munmap);
		EXPECT_EQ(g_ftmalloc_state.total_alloc, g_ftmalloc_state.total_dealloc);
		EXPECT_LE(g_ftmalloc_state.total_realloc_hits, g_ftmalloc_state.usage_realloc);
# endif

		if (HasFailure())
		{
			ftmalloc_show_mem_ex();
		}
	}
};

class FTMallocEnvScribbleTest : public FTMallocTest
{
protected:
    void SetUp() override
    {
        setenv(FTMALLOC_ENV_SCRIBBLE, "1", 1);
        assert(getenv(FTMALLOC_ENV_SCRIBBLE));
    }

    void TearDown() override
    {
        unsetenv(FTMALLOC_ENV_SCRIBBLE);
        assert(!getenv(FTMALLOC_ENV_SCRIBBLE));
        FTMallocTest::TearDown();
    }
};

class FTMallocEnvCheckHeapRelaxedTest : public FTMallocTest
{
protected:
    void SetUp() override
    {
        setenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED, "1", 1);
        assert(getenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED));
    }

    void TearDown() override
    {
        unsetenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED);
        assert(!getenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED));
        FTMallocTest::TearDown();
    }
};

class FTMallocEnvCheckHeapFullyTest : public FTMallocTest
{
protected:
    void SetUp() override
    {
        setenv(FTMALLOC_ENV_CHECK_HEAP_FULLY, "1", 1);
        assert(getenv(FTMALLOC_ENV_CHECK_HEAP_FULLY));
    }

    void TearDown() override
    {
        unsetenv(FTMALLOC_ENV_CHECK_HEAP_FULLY);
        assert(!getenv(FTMALLOC_ENV_CHECK_HEAP_FULLY));
        FTMallocTest::TearDown();
    }
};
