cmake_minimum_required(VERSION 3.31)
project(qcr C)

set(CMAKE_C_STANDARD 23)

if(WIN32)
    include_directories("$ENV{VULKAN_SDK}/Include")
    link_directories("$ENV{VULKAN_SDK}/Lib")
else()
    include_directories("$ENV{VULKAN_SDK}/include")
    link_directories("$ENV{VULKAN_SDK}/lib")
endif()

add_executable(qcr main.c icon.rc
        surface.c
        surface.h
        grx.c
        grx.h
        ma.h)

if (WIN32)
    target_link_libraries(qcr vulkan-1.lib)
else()
    target_link_libraries(qcr libvulkan.so)
endif()