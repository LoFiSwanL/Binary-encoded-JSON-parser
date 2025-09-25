#include "bej_parser.h"
#include "dictionary_read.h"
#include "file_reader.h"
#include "json_write.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bej_parser.h"
#include "dictionary_read.h"
#include "file_reader.h"
#include "json_write.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool bej_parse(const char *bej_filepath, const char *dictionary_filepath, const char* json_filepath) {
    size_t bej_size;
    unsigned char *bej_buffer = binary_read(bej_filepath, &bej_size);
    if (!bej_buffer) return false;

    Dictionary dict = dictionary_load(dictionary_filepath);
    if (!dict.buffer) {
        free(bej_buffer);
        return false;
    }

    JsonWriterState json_state = json_begin(json_filepath);
    if (!json_state.file) {
        free(bej_buffer);
        dictionary_free(&dict);
        return false;
    }

    json_begin_object(&json_state);

    BejParserState bej_state = {
        .data = bej_buffer,
        .size = bej_size,
        .curr_pos = 0
    };

    bool success = bej_parse_recursive(&bej_state, &json_state, &dict);

    json_end_object(&json_state);
    json_end(&json_state);
    free(bej_buffer);
    dictionary_free(&dict);

    return success;
}

BejElement bej_parse_element(BejParserState *state) {
    BejElement element = {0};

    uint32_t raw_tag = bej_read_tag(state);
    if (raw_tag == 0 || state->curr_pos >= state->size) return element;

    element.type = (BejDataType)(raw_tag & 0x0F);
    element.tag = (raw_tag >> 4);
    element.length = bej_read_length(state);

    printf("Tag=%u Type=0x%X Length=%llu Pos=%zu\n",
       element.tag, element.type, (unsigned long long)element.length, state->curr_pos);

    if (state->curr_pos + element.length > state->size) {
        fprintf(stderr, "Error: element length exceeds buffer size. Tag=%u Type=0x%X Length=%llu Pos=%zu\n",
                element.tag, element.type, (unsigned long long)element.length, state->curr_pos);
        element.type = BEJ_TYPE_UNKNOWN;
        element.length = 0;
        element.value = NULL;
        return element;
    }

    element.value = (const void*)(state->data + state->curr_pos);
    state->curr_pos += element.length;

    if (element.type != BEJ_TYPE_INTEGER &&
        element.type != BEJ_TYPE_STRING &&
        element.type != BEJ_TYPE_SET) {
        fprintf(stderr, "Unknown data type 0x%X encountered. Tag=%u\n", element.type, element.tag);
        element.type = BEJ_TYPE_UNKNOWN;
    }

    return element;
}

uint32_t bej_read_tag(BejParserState* state) {
    if (state->curr_pos >= state->size) return 0;
    uint8_t first_byte = state->data[state->curr_pos++];
    if (first_byte & 0x80) {
        if (state->curr_pos >= state->size) return 0;
        uint8_t second_byte = state->data[state->curr_pos++];
        return ((uint32_t)(first_byte & 0x7F) | ((uint32_t)second_byte << 7));
    } else {
        return (uint32_t)first_byte;
    }
}

uint64_t bej_read_length(BejParserState *state) {
    if (state->curr_pos >= state->size) return 0;
    uint8_t first_byte = state->data[state->curr_pos++];
    if (first_byte & 0x80) {
        uint8_t bytes_to_read = first_byte & 0x7F;
        if (state->curr_pos + bytes_to_read > state->size) return 0;
        uint64_t length = 0;
        for (int i = 0; i < bytes_to_read; ++i) {
            length = (length << 8) | state->data[state->curr_pos + i];
        }
        state->curr_pos += bytes_to_read;
        return length;
    } else {
        return (uint64_t)first_byte;
    }
}

bool bej_parse_recursive(BejParserState *bej_state, JsonWriterState *json_state, const Dictionary *dict) {
    while (bej_state->curr_pos < bej_state->size) {
        BejElement element = bej_parse_element(bej_state);
        if (element.tag == 0 && element.length == 0) break;

        const char* key_name = dictionary_key(dict, element.tag);
        if (!key_name) key_name = "unknown_tag";

        json_write_key_element(json_state, key_name, &element, dict);
    }
    return true;
}