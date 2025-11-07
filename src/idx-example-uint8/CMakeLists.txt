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
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<BOOL:${EXAMPLE_IDX_UINT8_WITH_ASAN}>:-fsanitize=address>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_example_idx_uint8
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_example_idx_uint8
    PRIVATE
        tgt_lib_idx
)
 
target_link_options(
    tgt_exe_example_idx_uint8
    PRIVATE
        -pie
        $<$<BOOL:${EXAMPLE_IDX_UINT8_WITH_ASAN}>:-fsanitize=address>
)

target_sources(
    tgt_exe_example_idx_uint8
    PRIVATE
        main.c
)

install(TARGETS tgt_exe_example_idx_uint8)
