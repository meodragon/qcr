//
// Created by meo-dragon on 8/15/2025.
//

// Tribute to all Management Associates I met

#ifndef MA_H
#define MA_H

typedef struct mat4
{
    float data[4][4];
} mat4;

mat4 qcr_rotate();
mat4 qcr_lookat();
mat4 qcr_perspective();

#endif //MA_H
