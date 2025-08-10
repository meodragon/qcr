//
// Created by meo-dragon on 7/30/2025.
//

#ifndef SURFACE_H
#define SURFACE_H
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#include "resource.h"
#else
#endif

typedef struct surface
{
#ifdef _WIN32
    HINSTANCE h_instance;
    HWND hwnd;
#else
#endif
    uint32_t width;
    uint32_t height;
    int (*run)();
} SURFACE;

int init_surface(SURFACE*);
void free_surface(SURFACE*);
#endif //SURFACE_H
