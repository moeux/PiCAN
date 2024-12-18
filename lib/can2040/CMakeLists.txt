FILE(GLOB_RECURSE can2040_sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
add_library(can2040 INTERFACE)
target_sources(can2040 INTERFACE ${can2040_sources})

target_include_directories(can2040 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(can2040 INTERFACE pico_base_headers hardware_pio hardware_dma cmsis_core)

pico_generate_pio_header(can2040 ${CMAKE_CURRENT_LIST_DIR}/pio/can2040.pio)