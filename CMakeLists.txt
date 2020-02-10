cmake_minimum_required(VERSION 3.1)

project(malloc)

add_library(malloc STATIC
    src/ftmalloc.c
    src/ftrealloc.c
    src/ftfree.c
    src/ftmalloc_show_mem.c
    src/ftmalloc_check_heap.c

	src/bin/bin_adj.c
	src/bin/bin_init.c
	src/bin/bin_mem.c
	src/bin/bin_of.c
	src/bin/bin_verify.c

	src/buddy/buddy_deoccupy.c
	src/buddy/buddy_merge.c
	src/buddy/buddy_occupy.c

	src/chunk/chunk_adj.c
	src/chunk/chunk_arrange.c
	src/chunk/chunk_bin_of.c
	src/chunk/chunk_freed.c
	src/chunk/chunk_in_use.c
	src/chunk/chunk_mem.c
	src/chunk/chunk_prev_in_use.c
	src/chunk/chunk_size.c
	src/chunk/chunk_verify.c

	src/mem/mem_allocate.c
	src/mem/mem_deallocate.c
	src/mem/mem_find.c
	src/mem/mem_reserve.c

	src/realloc/realloc_try_grow.c
	src/realloc/realloc_try_shrink.c

	src/show_mem/show_mem_chunk.c
	src/show_mem/show_mem_chunk_ex.c

	include/ftmalloc.h
    src/ftmalloc_internal.h
)

if (WIN32)
	target_sources(malloc
		PUBLIC
			src/ftmalloc_windows_adaptor.h
			src/ftmalloc_windows_adaptor.c
	)
endif ()

target_include_directories(malloc
    PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
	PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src
)

target_compile_definitions(malloc
    PUBLIC
		FTMALLOC_THREADSAFE
		FTMALLOC_DEBUG
)

set_target_properties(malloc
	PROPERTIES
		LINKER_LANGUAGE "C"
)


if (UNIX)
	message(STATUS "linking libft")

	add_subdirectory(libft)

	target_link_libraries(malloc
		PUBLIC
			ft
	)
	
	target_compile_options(malloc
		PRIVATE
			-Wall -Werror -Wunused
	)
endif ()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

add_executable(malloctest
    test/malloctest.h

    test/main.cpp
    test/malloc.cpp
    test/realloc.cpp
    test/mt.cpp
    test/mt_realloc.cpp
    test/env_scribble.cpp
    test/env_check_heap.cpp
)

target_include_directories(malloctest
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src
)

target_link_libraries(malloctest PUBLIC malloc gtest)
set_target_properties(malloctest
	PROPERTIES
		LINKER_LANGUAGE "CXX"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_test(NAME malloc_test COMMAND malloctest)
