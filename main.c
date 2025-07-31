//
// Created by meo-dragon on 7/30/2025.
//
#include <stdlib.h>

#include "surface.h"

struct qrc
{
    SURFACE surface;

};

int main()
{
    struct qrc app = {};
    if (init_surface(&app.surface))
    {
        return EXIT_FAILURE;
    }

    app.surface.run();

    free_surface(&app.surface);
    return EXIT_SUCCESS;
}
