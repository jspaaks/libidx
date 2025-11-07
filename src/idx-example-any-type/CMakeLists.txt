add_executable(tgt_exe_idx_example_any_type)

set_property(TARGET tgt_exe_idx_example_any_type PROPERTY OUTPUT_NAME idx-example-any-type)

target_compile_definitions(
    tgt_exe_idx_example_any_type
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_idx_example_any_type
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_idx_example_any_type
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<BOOL:${IDX_EXAMPLE_ANY_TYPE_WITH_ASAN}>:-fsanitize=address>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_idx_example_any_type
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_idx_example_any_type
    PRIVATE
        tgt_lib_idx
)

target_link_options(
    tgt_exe_idx_example_any_type
    PRIVATE
        -pie
        $<$<BOOL:${IDX_EXAMPLE_ANY_TYPE_WITH_ASAN}>:-fsanitize=address>
)

target_sources(
    tgt_exe_idx_example_any_type
    PRIVATE
        main.c
        strategies.c
)

install(TARGETS tgt_exe_idx_example_any_type)
