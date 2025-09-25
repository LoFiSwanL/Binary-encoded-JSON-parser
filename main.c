#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bej_parser.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <bej_filepath> <dictionary_filepath> <json_filepath>\n", argv[0]);
        return 1;
    }

    const char* bej_filepath = argv[1];
    const char* dictionary_filepath = argv[2];
    const char* json_filepath = argv[3];

    printf("starting BEJ parsing...\n");
    printf("BEJ file: %s\n", bej_filepath);
    printf("dictionary: %s\n", dictionary_filepath);
    printf("output JSON: %s\n", json_filepath);

    bool success = bej_parse(bej_filepath, dictionary_filepath, json_filepath);

    if (success) {
        printf("successfully! JSON saved to file %s\n", json_filepath);
        return 0;
    } else {
        fprintf(stderr, "failed to parse the BEJ file.\n");
        return 1;
    }
    
    return 0;
}