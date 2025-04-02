cmake_minimum_required(VERSION 3.10)
project(TestFileIO)

set(CMAKE_BUILD_TYPE Debug)

# ccache support
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "Found ccache: ${CCACHE_FOUND}")
  set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
  set(CMAKE_C_COMPILER_LAUNCHER ccache)
else()
  message(STATUS "ccache not found, proceeding without it.")
endif()

# Define an option to enable trace
option(ENABLE_TRACE "Enable tracing functionality" OFF)
if(ENABLE_TRACE)
    add_compile_definitions(TRACE_ENABLE)
endif()

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(SOURCES
    common/file_utils.cpp
    common/TrieNode.cpp
    common/PathTrie.cpp
    main.cpp
)

add_executable(TestFileIO ${SOURCES})
