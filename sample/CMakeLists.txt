cmake_minimum_required (VERSION 2.6)

add_subdirectory(./../lib ccsh_lib)

include_directories(${SHARED_LIB_INCLUDE_DIR})
include_directories(${WRAPPERS_INCLUDE_DIR})

file(GLOB_RECURSE wrappers ../wrappers/*.hpp)

add_executable(ccsh_sample main.cpp ${wrappers})
target_link_libraries( ccsh_sample PUBLIC ccsh_lib ${filesystem_lib})
