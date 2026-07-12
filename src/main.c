#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "state.h"

long get_filesize(FILE* file)
{
    long current_position = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current_position, SEEK_SET);

    return size;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: %s <brainfuck file>\n", argc == 0 ? "./bfc" : argv[0]);
        return 1;
    }

    FILE* code = fopen(argv[1], "r");

    if (code == NULL) {
        printf("File could not be opened!\n");
        return 2;
    }

    printf("file: %p\n", (void*)code);

    long filesize = get_filesize(code);
    char* code_buffer = malloc(filesize);

    printf("filesize: %ld\n", filesize);

    fread(code_buffer, 1, filesize, code);
    fclose(code);

    for (long i = 0; i < filesize; i++) {
        printf("%c", code_buffer[i]);
    }

    struct InterpreterState state;
    state_init(&state, 65536);

    state_delete(&state);
    free(code_buffer);

    return 0;
}
