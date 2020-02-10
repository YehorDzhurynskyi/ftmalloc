#include <cstdlib>
#include <ctime>
#include <cassert>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif

#include "malloctest.h"

#define SIZE 20000
#define MAXSIZE (1024 * 10)

#define MT

#define DECLARE_PARAMS(prefix)\
\
alloc_params prefix##_alloc_params;\
{\
    prefix##_alloc_params.cluster = prefix##_cluster;\
    prefix##_alloc_params.size = prefix##_size;\
    prefix##_alloc_params.min_alloc = prefix##_min;\
    prefix##_alloc_params.max_alloc = prefix##_max;\
}\
\
realloc_params prefix##_realloc_params;\
{\
    prefix##_realloc_params.cluster = prefix##_cluster;\
    prefix##_realloc_params.size = prefix##_size;\
    prefix##_realloc_params.reallocated = prefix##_reallocated;\
}\
\
dealloc_params prefix##_dealloc_params;\
{\
    prefix##_dealloc_params.cluster = prefix##_cluster;\
    prefix##_dealloc_params.size = prefix##_size;\
    prefix##_dealloc_params.reallocated = prefix##_reallocated;\
}\
\

#ifdef MT
#ifdef WIN32

# define DECLARE_WORKERS(prefix)\
\
DECLARE_PARAMS(prefix)\
\
HANDLE prefix##_alloc_thread = (HANDLE)_beginthread(\
    alloc_worker,\
    0,\
    &prefix##_alloc_params\
);\
\
HANDLE prefix##_realloc_thread = (HANDLE)_beginthread(\
    realloc_worker,\
    0,\
    &prefix##_realloc_params\
);\
\
HANDLE prefix##_dealloc_thread = (HANDLE)_beginthread(\
    dealloc_worker,\
    0,\
    &prefix##_dealloc_params\
);\

#else

#include <pthread.h>

typedef pthread_t HANDLE;
# define DECLARE_WORKERS(prefix)\
\
DECLARE_PARAMS(prefix)\
\
HANDLE prefix##_alloc_thread;\
pthread_create(\
	&prefix##_alloc_thread,\
    NULL,\
    (void * _Nullable (* _Nonnull)(void * _Nullable))alloc_worker,\
    &prefix##_alloc_params\
);\
\
HANDLE prefix##_realloc_thread;\
pthread_create(\
	&prefix##_realloc_thread,\
    NULL,\
    (void * _Nullable (* _Nonnull)(void * _Nullable))realloc_worker,\
    &prefix##_realloc_params\
);\
\
HANDLE prefix##_dealloc_thread;\
pthread_create(\
    &prefix##_dealloc_thread,\
    NULL,\
	(void * _Nullable (* _Nonnull)(void * _Nullable))dealloc_worker,\
    &prefix##_dealloc_params\
);\

#endif
#endif

typedef struct
{
    void **cluster;
    size_t *size;
    size_t min_alloc;
    size_t max_alloc;
} alloc_params;

typedef struct
{
    void **cluster;
    size_t *size;
    t_bool *reallocated;
} realloc_params;

typedef struct
{
    void **cluster;
    size_t *size;
    t_bool *reallocated;
} dealloc_params;

static void alloc_worker(void *p)
{
    alloc_params* params = (alloc_params*)p;
    for (volatile int i = 0; i < SIZE; ++i)
    {
        size_t alloc_size = rand() % params->max_alloc;
        alloc_size = alloc_size < params->min_alloc ? params->min_alloc : alloc_size;
        char *alloc = (char*)ftmalloc(alloc_size);
        assert(alloc != NULL);

#if 1
        for (unsigned int j = 0; j < alloc_size; ++j)
        {
            alloc[j] = (char)(rand() % 256);
        }
#else
        char ch = 'A';
        for (unsigned int j = 0; j < alloc_size; ++j)
        {
            alloc[j] = ch;
            ch = (ch + 1) % ('H' + 1);
            if (ch == 0) ch = 'A';
        }
#endif

        params->cluster[i] = alloc;
        params->size[i] = alloc_size;
    }
}

#define NREALLOC 1

static void realloc_worker(void *p)
{
    realloc_params* params = (realloc_params*)p;
    int c = 0;
    while (c < NREALLOC * SIZE)
    {
        volatile int i = rand() % SIZE;
        if (params->cluster[i] != NULL && params->reallocated[i] < NREALLOC)
        {
            void* old = malloc(params->size[i]);
            memcpy(old, params->cluster[i], params->size[i]);

            volatile size_t alloc_size = rand() % MAXSIZE;
            alloc_size = alloc_size > 1 ? alloc_size : 1;

            params->cluster[i] = (char*)ftrealloc(params->cluster[i], alloc_size);
            assert(params->cluster[i] != NULL);

            assert(0 == memcmp(old, params->cluster[i], alloc_size < params->size[i] ? alloc_size : params->size[i]));

#if NDEBUG
            volatile int cmpdiff = memcmp(old, params->cluster[i], alloc_size < params->size[i] ? alloc_size : params->size[i]);
            if (cmpdiff != 0)
            {
                printf("cmpdiff error: %i\n", cmpdiff);
                __debugbreak();
                exit(-1);
            }
#endif
            params->size[i] = alloc_size;
            params->reallocated[i]++;
            ++c;
            free(old);
        }
    }
}

static void dealloc_worker(void *p)
{
    dealloc_params* params = (dealloc_params*)p;
    int c = 0;
    while (c < SIZE)
    {
        volatile int i = rand() % SIZE;
        assert(params->reallocated[i] <= NREALLOC);
        if (params->cluster[i] != NULL && params->reallocated[i] == NREALLOC)
        {
            ftfree(params->cluster[i]);
            params->cluster[i] = NULL;
            ++c;
        }
    }
}

namespace
{

TEST_F(FTMallocTest, MTMallocReallocFree)
{
    setenv(FTMALLOC_ENV_SCRIBBLE, "1", 1);
    setenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED, "1", 1);
    setenv(FTMALLOC_ENV_CHECK_HEAP_FULLY, "1", 1);

    assert(getenv(FTMALLOC_ENV_SCRIBBLE));
    assert(getenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED));
    assert(getenv(FTMALLOC_ENV_CHECK_HEAP_FULLY));

    srand(time(NULL));

    void **sm_cluster = (void**)calloc(SIZE, sizeof(void*));
    void **md_cluster = (void**)calloc(SIZE, sizeof(void*));
    void **lg_cluster = (void**)calloc(SIZE, sizeof(void*));

    t_bool *sm_reallocated = (t_bool*)calloc(SIZE, sizeof(t_bool));
    t_bool *md_reallocated = (t_bool*)calloc(SIZE, sizeof(t_bool));
    t_bool *lg_reallocated = (t_bool*)calloc(SIZE, sizeof(t_bool));

    size_t *sm_size = (size_t*)calloc(SIZE, sizeof(size_t));
    size_t *md_size = (size_t*)calloc(SIZE, sizeof(size_t));
    size_t *lg_size = (size_t*)calloc(SIZE, sizeof(size_t));

    const size_t sm_min = 1;
    const size_t sm_max = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ + 1;

    const size_t md_min = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ + 1;
    const size_t md_max = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_MEM_CHUNK_SZ + 1;

    const size_t lg_min = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_MEM_CHUNK_SZ + 1;
    const size_t lg_max = MAXSIZE;

#ifndef MT
    DECLARE_PARAMS(sm)
    DECLARE_PARAMS(md)
    DECLARE_PARAMS(lg)

    alloc_worker(&sm_alloc_params);
    alloc_worker(&md_alloc_params);
    alloc_worker(&lg_alloc_params);

    dealloc_worker(&sm_dealloc_params);
    dealloc_worker(&md_dealloc_params);
    dealloc_worker(&lg_dealloc_params);

    alloc_worker(&sm_alloc_params);
    dealloc_worker(&sm_dealloc_params);

    alloc_worker(&md_alloc_params);
    dealloc_worker(&md_dealloc_params);

    alloc_worker(&lg_alloc_params);
    dealloc_worker(&lg_dealloc_params);
#else

    DECLARE_WORKERS(sm)
    DECLARE_WORKERS(md)
    DECLARE_WORKERS(lg)

    HANDLE threads[] = {
        sm_alloc_thread,
        sm_dealloc_thread,
        sm_realloc_thread,
        md_alloc_thread,
        md_dealloc_thread,
        md_realloc_thread,
        lg_alloc_thread,
        lg_dealloc_thread,
        lg_realloc_thread
    };
#ifdef WIN32
    WaitForMultipleObjects(sizeof(threads) / sizeof(threads[0]), threads, TRUE, INFINITE);
#else
	for (unsigned int i = 0; i < sizeof(threads) / sizeof(threads[0]); ++i)
	{
		pthread_join(threads[i], NULL);
	}
#endif
#endif

    free(sm_cluster);
    free(md_cluster);
    free(lg_cluster);

    free(sm_reallocated);
    free(md_reallocated);
    free(lg_reallocated);

    free(sm_size);
    free(md_size);
    free(lg_size);

    unsetenv(FTMALLOC_ENV_SCRIBBLE);
    unsetenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED);
    unsetenv(FTMALLOC_ENV_CHECK_HEAP_FULLY);

    assert(!getenv(FTMALLOC_ENV_SCRIBBLE));
    assert(!getenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED));
    assert(!getenv(FTMALLOC_ENV_CHECK_HEAP_FULLY));
}

}
