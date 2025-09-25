#ifndef BEJ_PARSER_H
#define BEJ_PARSER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declaration
struct JsonWriterState;
struct Dictionary;

typedef enum {
    BEJ_TYPE_INTEGER = 0,
    BEJ_TYPE_SET     = 1,
    BEJ_TYPE_STRING  = 2,
    BEJ_TYPE_UNKNOWN = 255
} BejDataType;

typedef struct {
    const unsigned char* data;
    size_t size;
    size_t curr_pos;
} BejParserState;

typedef struct BejElement {
    uint32_t tag;
    BejDataType type;
    uint64_t length;
    const void *value;
} BejElement;

bool bej_parse(const char *bej_filepath, const char *dictionary_filepath, const char* json_filepath);
BejElement bej_parse_element(BejParserState* state);
uint32_t bej_read_tag(BejParserState* state);
uint64_t bej_read_length(BejParserState* state);
bool bej_parse_recursive(BejParserState *bej_state, struct JsonWriterState *json_state, const struct Dictionary *dict);

#endif
