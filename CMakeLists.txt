cmake_minimum_required(VERSION 3.31)
project(qcr C)

set(CMAKE_C_STANDARD 23)

include_directories("$ENV{VULKAN_SDK}/Include")
link_directories("$ENV{VULKAN_SDK}/Lib")

add_executable(qcr main.c icon.rc
        surface.c
        surface.h
        grx.c
        grx.h)

target_link_libraries(qcr vulkan-1.lib)