#ifndef FILE_READER_H
#define FILE_READER_H

#include <stddef.h>
#include <stdint.h>

unsigned char* binary_read(const char *filepath, size_t *size_out);

#endif