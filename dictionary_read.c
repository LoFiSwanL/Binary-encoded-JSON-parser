#include "dictionary_read.h"
#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t dictionary_read_vlq(unsigned char **buffer, size_t *size) {
    uint32_t value = 0;
    int shift = 0;

    while (*size > 0) {
        uint8_t byte = **buffer;
        (*buffer)++;
        (*size)--;

        value |= ((uint32_t)(byte & 0x7F) << shift);
        if (!(byte & 0x80)) {
            break;
        }
        shift += 7;
    }

    return value;
}

Dictionary dictionary_load(const char *filepath)
{
    Dictionary dict = {0};
    unsigned char* raw_buffer = NULL;
    size_t raw_size = 0;

    raw_buffer = binary_read(filepath, &raw_size);
    if (!raw_buffer) return dict;

    dict.buffer = raw_buffer;
    dict.buffer_size = raw_size;

    size_t pos = 0;
    size_t count = 0;

    while (pos + 2 <= raw_size) {
        uint8_t tag = raw_buffer[pos++];
        uint8_t key_len = raw_buffer[pos++];
        if (pos + key_len > raw_size) break;
        pos += key_len;
        count++;
    }

    dict.num_entries = count;
    dict.entries = (DictionaryEntry*)calloc(count, sizeof(DictionaryEntry));
    if (!dict.entries) {
        free(dict.buffer);
        dict.buffer = NULL;
        return dict;
    }

    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t tag = raw_buffer[pos++];
        uint8_t key_len = raw_buffer[pos++];
        if (pos + key_len > raw_size) break;

        char* key = (char*)malloc(key_len + 1);
        memcpy(key, &raw_buffer[pos], key_len);
        key[key_len] = '\0';
        pos += key_len;

        dict.entries[i].tag = tag;
        dict.entries[i].key = key;
    }

    return dict;
}

void dictionary_free(Dictionary *dictionary)
{
    if (dictionary) {
        if (dictionary->entries) {
            for (size_t i = 0; i < dictionary->num_entries; i++) {
                free((void*)dictionary->entries[i].key);
            }
            free(dictionary->entries);
        }
        free(dictionary->buffer);

        dictionary->buffer = NULL;
        dictionary->entries = NULL;
        dictionary->num_entries = 0;
        dictionary->buffer_size = 0;
    }
}

const char* dictionary_key(const Dictionary* dict, uint32_t tag)
{
    for (size_t i = 0; i < dict->num_entries; i++) {
        if (dict->entries[i].tag == tag) {
            return dict->entries[i].key;
        }
    }
    printf("Searching for tag=%u among %zu entries\n", tag, dict->num_entries);
    for (size_t i = 0; i < dict->num_entries; i++) {
    printf("  Entry %zu: tag=%u key=%s\n", i, dict->entries[i].tag, dict->entries[i].key);
}
    return NULL;
}
