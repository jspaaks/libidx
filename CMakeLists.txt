cmake_minimum_required(VERSION 3.23...3.28 FATAL_ERROR)

project(project LANGUAGES C)

option(EXAMPLE_INT16_2D_WITH_ASAN "Whether to enable address sanitizing for executable 'example-int16-2d'" OFF)
option(EXAMPLE_UINT8_1D_WITH_ASAN "Whether to enable address sanitizing for executable 'example-uint8-1d'" OFF)
option(EXAMPLE_IDXREAD_WITH_ASAN "Whether to enable address sanitizing for executable 'idxread'" OFF)
option(IDX_WITH_ASAN "Whether to enable address sanitizing for library 'libidx.so'" OFF)

include(CTest)
include(GenerateExportHeader)
include(GNUInstallDirs)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

if (APPLE)
    list(APPEND CMAKE_INSTALL_RPATH @loader_path/../lib)
elseif(UNIX)
    list(APPEND CMAKE_INSTALL_RPATH $ORIGIN/../lib)
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # I'm the top-most project, set IDX_BUILD_TESTING like BUILD_TESTING by default
    option(IDX_BUILD_TESTING "Whether to build the libidx tests" ${BUILD_TESTING})
    option(IDX_BUILD_EXAMPLES "Whether to build the libidx examples" ON)
    option(IDX_BUILD_IDXREAD "Whether to build the idxread executable" ON)
else()
    # I'm not the top-most project, set IDX_BUILD_TESTING default OFF
    option(IDX_BUILD_TESTING "Whether to build the libidx tests" OFF)
    option(IDX_BUILD_EXAMPLES "Whether to build the libidx examples" OFF)
    option(IDX_BUILD_IDXREAD "Whether to build the idxread executable" OFF)
endif()

add_subdirectory(src/idx)

if (IDX_BUILD_IDXREAD)
    add_subdirectory(src/idxread)
endif()
if (IDX_BUILD_EXAMPLES)
    add_subdirectory(src/example_uint8_1d)
    add_subdirectory(src/example_int16_2d)
endif()
if (IDX_BUILD_TESTING)
    add_subdirectory(test/idx)
endif()
