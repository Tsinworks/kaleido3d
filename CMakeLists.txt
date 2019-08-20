cmake_minimum_required (VERSION 3.6)
project (kaleido3d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${kaleido3d_SOURCE_DIR}/scripts/cmake")

option(BUILD_SHARED "build shared libraries" ON)
option(BUILD_WITH_UNIT_TEST "build with unit test" OFF)

include(common)

if(WIN32)
    include(generate_winres)
endif()

include(targets)

include_directories(.build/third_party/include)
# include(.build/third_party)

add_subdirectory(src)