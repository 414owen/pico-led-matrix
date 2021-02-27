cmake_minimum_required(VERSION 3.12)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# make common .h files visible
include_directories(lib)

include(language.cmake)

# turn all reasonable gcc warnings
include(warnings.cmake)

# initialize the Pico SDK
pico_sdk_init()

# set project name from directory
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} C CXX ASM)

# rest of your project
add_executable(main
    main.c
    ../lib/bootsel.c
    ../lib/brightness.c
    ../lib/char.c
    ../lib/countdown.c
    ../lib/font.c
    ../lib/matrix.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(main pico_stdlib pico_multicore)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(main)
