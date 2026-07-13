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

    printf("command executed: ");
    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");

    FILE* code = fopen(argv[1], "r");
    if (code == NULL) {
        printf("File could not be opened!\n");
        return 2;
    }

    unsigned long filesize = (unsigned long)get_filesize(code);
    printf("file: %s | %p | size: %ld\n", argv[1], (void*)code, filesize);

    char* code_buffer = malloc(filesize);
    fread(code_buffer, 1, filesize, code);
    fclose(code);

    printf("\n====================\n\n");
    for (unsigned long i = 0; i < filesize; i++) {
        printf("%c", code_buffer[i]);
    }

    printf("\n====================\n\n");

    struct InterpreterState state;
    state_init(&state, UINT16_MAX + 1);

    struct ParseResult* parse_result = parse(code_buffer, filesize);
    free(code_buffer);

    for (unsigned long i = 0; i < parse_result->command_count; i++) {
        switch (parse_result->command_buffer[i].kind) {
        case IncreasePointer:
            printf("> %d\n", parse_result->command_buffer[i].data.count);
            break;
        case DecreasePointer:
            printf("< %d\n", parse_result->command_buffer[i].data.count);
            break;
        case Add:
            printf("+ %d\n", parse_result->command_buffer[i].data.count);
            break;
        case Subtract:
            printf("- %d\n", parse_result->command_buffer[i].data.count);
            break;
        case OutputChar:
            printf(". %d\n", parse_result->command_buffer[i].data.count);
            break;
        case InputChar:
            printf(", %d\n", parse_result->command_buffer[i].data.count);
            break;
        case LoopStart:
            printf("Loop Start %d\n", parse_result->command_buffer[i].data.index);
            break;
        case LoopEnd:
            printf("Loop End %d\n", parse_result->command_buffer[i].data.index);
            break;
        default:
            printf("INVALID COMMAND! %d tried to execute!", parse_result->command_buffer[i].kind);
            break;
        }
    }

    if (parse_result == NULL) {
        printf("parsing failed\n");
    }

    while (1) {
        if (state.code_position >= parse_result->command_count) {
            break;
        }

        struct Command cmd = parse_result->command_buffer[state.code_position];

        switch (cmd.kind) {
        case IncreasePointer:
            state.tape_position += cmd.data.count;
            break;
        case DecreasePointer:
            state.tape_position -= cmd.data.count;
            break;
        case Add:
            state.tape[state.tape_position] += cmd.data.count;
            break;
        case Subtract:
            state.tape[state.tape_position] -= cmd.data.count;
            break;
        case OutputChar:
            for (unsigned int i = 0; i < cmd.data.count; i++) {
                printf("%c", state.tape[state.tape_position]);
            }
            break;
        case InputChar:
            // TODO: use input buffer instead of stdin
            for (unsigned int i = 0; i < cmd.data.count; i++) {
                int ch = getchar();
                state.tape[state.tape_position] = (ch == EOF) ? 0 : (uint8_t)ch;
            }
            break;
        case LoopStart:

            if (state.tape[state.tape_position] == 0) {
                state.code_position = parse_result->jump_table[cmd.data.index].end;
            }
            break;
        case LoopEnd:
            if (state.tape[state.tape_position] != 0) {
                state.code_position = parse_result->jump_table[cmd.data.index].start;
            }
            break;
        default:
            printf("INVALID COMMAND! %d tried to execute!", cmd.kind);
            break;
        }

        state.code_position++;
    }

    free(parse_result->jump_table);
    free(parse_result->command_buffer);
    free(parse_result);

    state_delete(&state);

    return 0;
}
