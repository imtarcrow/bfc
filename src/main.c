#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "state.h"

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

    printf("command executed: ");
    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    unsigned long filesize = (unsigned long)get_filesize(code);
    printf("file: %s | %p | size: %ld\n", argv[1], (void*)code, filesize);

    char* code_buffer = malloc(filesize);
    fread(code_buffer, 1, filesize, code);
    fclose(code);

    printf("\n####################\n\n");
    for (unsigned long i = 0; i < filesize; i++) {
        printf("%c", code_buffer[i]);
    }

    printf("\n####################\n\n");

    struct InterpreterState state;
    state_init(&state, UINT16_MAX + 1);

    struct ParseResult* parse_result = parse(code_buffer, filesize);

    if (parse_result == NULL) {
        printf("ERROR NULL\n");
        exit(100);
    }

    free(parse_result->command_buffer);
    free(parse_result);

    while (1) {
        if (state.code_position >= filesize) {
            break;
        }

        uint8_t command = code_buffer[state.code_position];

        switch (command) {
        case '>':
            state.tape_position++;
            break;
        case '<':
            state.tape_position--;
            break;
        case '+':
            state.tape[state.tape_position]++;
            break;
        case '-':
            state.tape[state.tape_position]--;
            break;
        case '.':
            printf("%c", state.tape[state.tape_position]);
            break;
        case ',': {
            int ch = getchar();
            state.tape[state.tape_position] = (ch == EOF) ? 0 : (uint8_t)ch;
            break;
        }
        case '[':
            if (state.tape[state.tape_position] == 0) {
                int depth = 1;
                while (depth != 0) {
                    state.code_position++;
                    if (code_buffer[state.code_position] == '[') {
                        depth++;
                    }
                    if (code_buffer[state.code_position] == ']') {
                        depth--;
                    }
                }
            }
            break;
        case ']':
            if (state.tape[state.tape_position] != 0) {
                int depth = 1;
                while (depth != 0) {
                    state.code_position--;
                    if (code_buffer[state.code_position] == ']') {
                        depth++;
                    }
                    if (code_buffer[state.code_position] == '[') {
                        depth--;
                    }
                }
            }
            break;
        }

        state.code_position += 1;
    }

    state_delete(&state);
    free(code_buffer);

    return 0;
}
