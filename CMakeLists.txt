# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

set(PICO_FREERTOS ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

include(${PICO_FREERTOS}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(SimonSays C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(SimonSays 
        SimonSays.c
        include/buzzer/buzzer_modulo.c
        include/joystick/joystick_modulo.c
        include/matriz_led/matriz_led_modulo.c
        include/botao/botao_modulo.c)

pico_set_program_name(SimonSays "SimonSays")
pico_set_program_version(SimonSays "0.1")

# Generate PIO header
pico_generate_pio_header(SimonSays ${CMAKE_CURRENT_LIST_DIR}/include/matriz_led/ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(SimonSays 0)
pico_enable_stdio_usb(SimonSays 1)

# Add the standard library to the build
target_link_libraries(SimonSays
        pico_stdlib
        FreeRTOS-Kernel-Heap4)

# Add the standard include files to the build
target_include_directories(SimonSays PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add any user requested libraries
target_link_libraries(SimonSays
        FreeRTOS-Kernel-Heap4
        hardware_pwm
        hardware_adc
        pico_stdlib
        hardware_clocks
        hardware_pio
        hardware_dma
        )

pico_add_extra_outputs(SimonSays)

