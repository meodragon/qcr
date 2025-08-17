//
// Created by meo-dragon on 8/17/2025.
//

#ifndef BYTES_H
#define BYTES_H

#include <stdint.h>

typedef struct byte_buffer {
    char *data;
    uint32_t byte_count;
} byte_buffer;

byte_buffer read_byte_buffer(const char *);
void free_byte_buffer(byte_buffer);

#endif //BYTES_H
