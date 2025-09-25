#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char* binary_read(const char *filepath, size_t *size_out) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "could not open file for reading\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        fprintf(stderr, "memory allocation fail\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, size, file);
    fclose(file);

    if(bytes_read != size){
        fprintf(stderr, "cant read file\n");
        free(buffer);
        return NULL;
    }

    *size_out = size;
    return buffer;
}