#include <cstdlib>
#include <ctime>
#include <cassert>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif

#include "malloctest.h"

#define SIZE 20000

#define MT

#define DECLARE_PARAMS(prefix)\
\
alloc_params prefix##_alloc_params;\
{\
    prefix##_alloc_params.cluster = prefix##_cluster;\
    prefix##_alloc_params.min_alloc = prefix##_min;\
    prefix##_alloc_params.max_alloc = prefix##_max;\
}\
\
dealloc_params prefix##_dealloc_params;\
{\
    prefix##_dealloc_params.cluster = prefix##_cluster;\
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
    size_t min_alloc;
    size_t max_alloc;
} alloc_params;

typedef struct
{
    void **cluster;
} dealloc_params;

static void alloc_worker(void *p)
{
    alloc_params* params = (alloc_params*)p;
    for (int i = 0; i < SIZE; ++i)
    {
        size_t alloc_size = rand() % params->max_alloc;
        alloc_size = alloc_size < params->min_alloc ? params->min_alloc : alloc_size;
        char *alloc = (char*)ftmalloc(alloc_size);
        assert(alloc != NULL);

        for (unsigned int j = 0; j < alloc_size; ++j)
        {
            alloc[j] = (char)(rand() % 256);
        }

        params->cluster[i] = alloc;
    }
}

static void dealloc_worker(void *p)
{
    dealloc_params* params = (dealloc_params*)p;
    int c = 0;
    while (c < SIZE)
    {
        int i = rand() % SIZE;
        if (params->cluster[i] != NULL)
        {
            ftfree(params->cluster[i]);
            params->cluster[i] = NULL;
            ++c;
        }
    }
}

namespace
{

TEST_F(FTMallocTest, MTMallocFree)
{
    srand(time(NULL));

    void **sm_cluster = (void**)calloc(SIZE, sizeof(void*));
    void **md_cluster = (void**)calloc(SIZE, sizeof(void*));
    void **lg_cluster = (void**)calloc(SIZE, sizeof(void*));

    const size_t sm_min = 1;
    const size_t sm_max = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ + 1;

    const size_t md_min = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ + 1;
    const size_t md_max = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_MEM_CHUNK_SZ + 1;

    const size_t lg_min = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_MEM_CHUNK_SZ + 1;
    const size_t lg_max = 1024 * 10;

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
        md_alloc_thread,
        md_dealloc_thread,
        lg_alloc_thread,
        lg_dealloc_thread
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
}

}
