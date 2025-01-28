cmake_minimum_required(VERSION 3.23...3.28)

project(project LANGUAGES C)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/dist")
endif()

add_subdirectory(${PROJECT_ROOT}/src/idx)
include(CTest)  # defines BUILD_TESTING
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # I'm the top-most project
    option(IDX_BUILD_EXAMPLES "Whether to build the libidx examples" ON)
    option(IDX_BUILD_TESTING "Whether to build the libidx tests" ${BUILD_TESTING})
    if ((BUILD_TESTING AND NOT IDX_BUILD_TESTING) OR
        (NOT BUILD_TESTING AND IDX_BUILD_TESTING))
        message(WARNING, " will use more specific -DIDX_BUILD_TESTING=${IDX_BUILD_TESTING} over -DBUILD_TESTING=${BUILD_TESTING}")
    endif()
else()
    # I'm not the top-most project
    option(IDX_BUILD_EXAMPLES "Whether to build the libidx examples" OFF)
    option(IDX_BUILD_TESTING "Whether to build the libidx tests" OFF)
endif()
if (IDX_BUILD_EXAMPLES)
    add_subdirectory(${PROJECT_ROOT}/src/example-idx-any-type)
    add_subdirectory(${PROJECT_ROOT}/src/example-idx-uint8)
endif()
if (IDX_BUILD_TESTING)
    add_subdirectory(${PROJECT_ROOT}/test/idx)
endif()
