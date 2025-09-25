#ifndef JSON_WRITE_H
#define JSON_WRITE_H

#include <stdio.h>
#include <stdbool.h>

// Forward declarations instead of includes
struct BejElement;
struct Dictionary;

typedef struct JsonWriterState {
    FILE *file;
    bool is_first_item;
} JsonWriterState;

JsonWriterState json_begin(const char *filepath);
void json_begin_object(JsonWriterState *state);
void json_end_object(JsonWriterState *state);
void json_end(JsonWriterState *state);
void json_write_key_element(JsonWriterState *state, const char *key_name, const struct BejElement *element, const struct Dictionary *dict);

#endif
