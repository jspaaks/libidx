cmake_minimum_required(VERSION 3.23...3.28)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/dist")
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
if (APPLE)
    list(APPEND CMAKE_INSTALL_RPATH @loader_path/../lib)
elseif(UNIX)
    list(APPEND CMAKE_INSTALL_RPATH $ORIGIN/../lib)
endif()

add_executable(tgt_exe_example_idx_uint8)

set_property(TARGET tgt_exe_example_idx_uint8 PROPERTY OUTPUT_NAME example-idx-uint8)

target_compile_definitions(
    tgt_exe_example_idx_uint8
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_example_idx_uint8
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_example_idx_uint8
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_example_idx_uint8
    PRIVATE
        ${PROJECT_ROOT}/include
)

target_link_libraries(
    tgt_exe_example_idx_uint8
    PRIVATE
        tgt_lib_idx
)

target_sources(
    tgt_exe_example_idx_uint8
    PRIVATE
        ${PROJECT_ROOT}/src/example-idx-uint8/main.c
)

install(TARGETS tgt_exe_example_idx_uint8)
