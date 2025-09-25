cmake_minimum_required(VERSION 3.31)
project(Lab1)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab1 
    main.c
    bej_parser.c
    json_write.c
    dictionary_read.c
    file_reader.c
)

