//
// Created by meo-dragon on 7/30/2025.
//
#include <stdio.h>
#include <stdlib.h>

#include "grx.h"
#include "surface.h"

struct qrc
{
    SURFACE surface;
    GRX grx;
};

int main()
{
    struct qrc app = {};
    if (init_surface(&app.surface))
    {
        return EXIT_FAILURE;
    }
    if (init_grx(&app.grx))
    {
        return EXIT_FAILURE;
    }

    app.surface.run();

    free_surface(&app.surface);
    free_grx(&app.grx);
    return EXIT_SUCCESS;
}
