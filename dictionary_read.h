#ifndef DICTIONARY_READ_H
#define DICTIONARY_READ_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct DictionaryEntry {
    uint32_t tag;
    char *key;
} DictionaryEntry;

typedef struct Dictionary {
    unsigned char* buffer;
    size_t buffer_size;
    DictionaryEntry *entries;
    size_t num_entries;
} Dictionary;

Dictionary dictionary_load(const char *filepath);
void dictionary_free(Dictionary *dictionary);
const char* dictionary_key(const Dictionary* dict, uint32_t tag);

#endif
