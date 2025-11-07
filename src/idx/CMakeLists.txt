add_library(tgt_lib_idx SHARED)

set_property(TARGET tgt_lib_idx PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET tgt_lib_idx PROPERTY OUTPUT_NAME idx)

generate_export_header(
    tgt_lib_idx
    BASE_NAME idx
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/idx/export.h
)

target_compile_definitions(
    tgt_lib_idx
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_lib_idx
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_lib_idx
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIC
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<BOOL:${IDX_WITH_ASAN}>:-fsanitize=address>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_lib_idx
    PRIVATE
        ../../include
)

target_link_options(
    tgt_lib_idx
    PRIVATE
        $<$<BOOL:${IDX_WITH_ASAN}>:-fsanitize=address>
)

target_sources(
    tgt_lib_idx
    PRIVATE
        idx.c
        protected.c
    PUBLIC
        FILE_SET fset_lib_idx_headers
            TYPE HEADERS
            BASE_DIRS
                ../../include
                ${CMAKE_BINARY_DIR}/include  # contains cmake-generated file
            FILES
                ../../include/idx/idx.h
                ${CMAKE_BINARY_DIR}/include/idx/export.h  # cmake-generated file
)

install(TARGETS tgt_lib_idx
    LIBRARY
    FILE_SET fset_lib_idx_headers
)
