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

    printf("Starting BEJ parsing...\n");
    printf("BEJ file: %s\n", bej_filepath);
    printf("Dictionary: %s\n", dictionary_filepath);
    printf("Output JSON: %s\n", json_filepath);

    bool success = bej_parse(bej_filepath, dictionary_filepath, json_filepath);

    if (success) {
        printf("BEJ parsing completed successfully! JSON saved to file %s\n", json_filepath);
        return 0;
    } else {
        fprintf(stderr, "Error: Failed to parse the BEJ file.\n");
        return 1;
    }
    
    return 0;
}