cmake_minimum_required(VERSION 3.1)

project(malloc)

add_library(malloc STATIC
    src/ftmalloc_malloc.c
    src/ftmalloc_realloc.c
    src/ftmalloc_free.c
    src/ftmalloc_bin.c
    src/ftmalloc_buddy.c
    src/ftmalloc_chunk.c
    src/ftmalloc_init.c
    src/ftmalloc_reserve_mem.c
    src/ftmalloc_show_mem.c

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_compile_definitions(malloc
    PUBLIC
        FTMALLOC_THREADSAFE
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
