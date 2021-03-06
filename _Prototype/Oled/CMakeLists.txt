# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/csowa/Desktop/PicoDev/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Oled C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Oled 
              Oled.c
              OLED_Driver.c
              OLED_GFX.c
              )

pico_set_program_name(Oled "Oled")
pico_set_program_version(Oled "0.1")

pico_enable_stdio_uart(Oled 1)
pico_enable_stdio_usb(Oled 0)

# Add the standard library to the build
target_link_libraries(Oled pico_stdlib)

# Add any user requested libraries
target_link_libraries(Oled
        hardware_spi
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

pico_add_extra_outputs(Oled)

