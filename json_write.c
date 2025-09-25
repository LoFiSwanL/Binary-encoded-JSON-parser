#include "json_write.h"
#include "bej_parser.h"
#include "dictionary_read.h"
#include <stdio.h>
#include <string.h>

static void json_write_comma(JsonWriterState *state)
{
    if(!state->is_first_item){
        fprintf(state->file, ",");
    }
    state->is_first_item = false;
}

static void json_write_key(JsonWriterState *state, const char *key_name)
{
    fprintf(state->file, "\"%s\":", key_name);
}

JsonWriterState json_begin(const char *filepath)
{
    JsonWriterState state;
    state.file = fopen(filepath, "w");
    state.is_first_item = true;

    if(!state.file){
        fprintf(stderr, "cant open json file for write\n");
    }
    return state;
}

void json_begin_object(JsonWriterState *state)
{
    if(!state->file){
        return;
    }
    fprintf(state->file, "{");
    state->is_first_item = true;
}

void json_end_object(JsonWriterState *state)
{
    if(!state->file){
        return;
    }
    fprintf(state->file, "}");
    state->is_first_item = false;
}

void json_end(JsonWriterState *state)
{
    if(state->file){
        fclose(state->file);
    }
}

void json_write_key_element(JsonWriterState *state, const char *key_name, const BejElement *element, const Dictionary *dict) {
    if (!state->file) return;

    json_write_comma(state);
    json_write_key(state, key_name);

    switch (element->type) {
        case BEJ_TYPE_SET: {
            BejParserState nested_state = {
                .data = (const unsigned char*)element->value,
                .size = element->length,
                .curr_pos = 0
            };
            json_begin_object(state);
            bej_parse_recursive(&nested_state, state, dict);
            json_end_object(state);
            break;
        }
        case BEJ_TYPE_INTEGER: {
            uint64_t integer_value = 0;
            switch (element->length) {
                case 1: integer_value = *(uint8_t*)element->value; break;
                case 2: integer_value = *(uint16_t*)element->value; break;
                case 4: integer_value = *(uint32_t*)element->value; break;
                case 8: integer_value = *(uint64_t*)element->value; break;
                default: integer_value = 0; break;
            }
            fprintf(state->file, "%llu", (unsigned long long)integer_value);
            break;
        }
        case BEJ_TYPE_STRING: {
            fprintf(state->file, "\"%.*s\"", (int)element->length, (char*)element->value);
            break;
        }
        case BEJ_TYPE_UNKNOWN:
        default: {
            fprintf(state->file, "\"<unknown>\"");
            break;
        }
    }
}