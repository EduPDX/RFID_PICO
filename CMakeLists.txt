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
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(RFID_PICO C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(
    RFID_PICO
    RFID_PICO.c
    src/mqtt/mqtt.c
    src/MFRC522/mfrc522.c
    src/ssd1306/ssd1306_i2c.c
    src/display/display.c
    )

pico_set_program_name(RFID_PICO "main")
pico_set_program_version(RFID_PICO "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(RFID_PICO 0)
pico_enable_stdio_usb(RFID_PICO 1)

# Add the standard library to the build
target_link_libraries(RFID_PICO
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mqtt
        pico_lwip_iperf
        hardware_i2c
        hardware_spi
        hardware_pwm)

# Add the standard include files to the build
target_include_directories(RFID_PICO PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(
    RFID_PICO
    pico_stdlib
)

pico_add_extra_outputs(RFID_PICO)


