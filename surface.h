//
// Created by meo-dragon on 7/30/2025.
//

#ifndef SURFACE_H
#define SURFACE_H
#include <stdint.h>

#include <windows.h>
#include "resource.h"

typedef struct surface
{
    HINSTANCE h_instance;
    HWND hwnd;
    uint32_t width;
    uint32_t height;
    int (*run)();
} SURFACE;

int init_surface(SURFACE*);
void free_surface(SURFACE*);
#endif //SURFACE_H
