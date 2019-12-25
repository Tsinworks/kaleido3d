cmake_minimum_required (VERSION 3.6)
project (kaleido3d)

set_property(GLOBAL PROPERTY USE_FOLDERS on)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${kaleido3d_SOURCE_DIR}/scripts/cmake")

option(BUILD_SHARED "build shared libraries" ON)
option(BUILD_WITH_UNIT_TEST "build with unit test" OFF)

include(common)

if(WIN32)
    include(generate_winres)
endif()

include(targets)

include_directories(.build/third_party/include)

set(KCORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/Core)
set(KNGFX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/NGFX/Public)
# include(.build/third_party)

add_subdirectory(src)