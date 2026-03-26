add_executable(tgt_exe_example_uint8_1d)

set_property(TARGET tgt_exe_example_uint8_1d PROPERTY OUTPUT_NAME example-uint8-1d)

target_compile_definitions(
    tgt_exe_example_uint8_1d
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_example_uint8_1d
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_example_uint8_1d
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<BOOL:${EXAMPLE_UINT8_1D_WITH_ASAN}>:-fsanitize=address>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_example_uint8_1d
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_example_uint8_1d
    PRIVATE
        tgt_lib_idx
)
 
target_link_options(
    tgt_exe_example_uint8_1d
    PRIVATE
        -pie
        $<$<BOOL:${EXAMPLE_UINT8_1D_WITH_ASAN}>:-fsanitize=address>
)

target_sources(
    tgt_exe_example_uint8_1d
    PRIVATE
        main.c
)

install(TARGETS tgt_exe_example_uint8_1d)
