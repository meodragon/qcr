//
// Created by meo-dragon on 8/17/2025.
//
#include "bytes.h"


#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>

byte_buffer read_byte_buffer(const char* path) {
    printf("[%s:%d] read byte buffer from %s\n", __func__, __LINE__, path);
    byte_buffer b_buffer = {};
    b_buffer.byte_count = 0;
    b_buffer.data = NULL;
    int fh, bytes_read;
    errno_t err = _sopen_s(&fh, path, _O_RDONLY | _O_BINARY, _SH_DENYNO, 0);
    if (err != 0)
    {
        printf("cannot open file %s, errno %d", path, err);
        exit(EXIT_FAILURE);
    }

    long file_len = _lseek(fh, 0, SEEK_END);
    printf("file length: %ld\n", file_len);
    char* buffer = calloc(file_len, sizeof(char));
    _lseek(fh, 0, SEEK_SET);
    if ((bytes_read = _read(fh, buffer, file_len)) <= 0) {
        printf("problem reading file %s (read %d bytes)\n", path, bytes_read);
        exit(EXIT_FAILURE);
    }
    printf("read %d bytes\n", bytes_read);
    _close(fh);

    b_buffer.byte_count = bytes_read;
    b_buffer.data = buffer;

    return b_buffer;
}


void free_byte_buffer(byte_buffer buff) {
    free(buff.data);
    buff.data = NULL;
}