#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "token.h"

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

    struct TokenizationResult result = tokenize_file(argv[1], false);
    if (result.tokens == NULL && result.token_count == 0) {
        printf("tokenization failed\n");
    }

    struct InterpreterState state;
    interpreter_init(&state, UINT16_MAX + 1);

    while (1) {
        if (state.code_position >= result.token_count) {
            break;
        }

        enum Token token = result.tokens[state.code_position];

        switch (token) {
        case IncreasePointer:
            state.tape.cursor_position += 1;
            break;
        case DecreasePointer:
            state.tape.cursor_position -= 1;
            break;
        case Add:
            state.tape.buffer[state.tape.cursor_position] += 1;
            break;
        case Subtract:
            state.tape.buffer[state.tape.cursor_position] -= 1;
            break;
        case OutputChar:
            printf("%c", state.tape.buffer[state.tape.cursor_position]);
            break;
        case InputChar: {
            // TODO: use input buffer instead of stdin
            int ch = getchar();
            state.tape.buffer[state.tape.cursor_position] = (ch == EOF) ? 0 : (uint8_t)ch;
            break;
        }
        case LoopStart:
            if (state.tape.buffer[state.tape.cursor_position] == 0) {
                int depth = 1;
                while (depth != 0) {
                    state.code_position++;
                    if (result.tokens[state.code_position] == LoopStart) {
                        depth++;
                    }
                    if (result.tokens[state.code_position] == LoopEnd) {
                        depth--;
                    }
                }
            }
            break;
        case LoopEnd:
            if (state.tape.buffer[state.tape.cursor_position] != 0) {
                int depth = 1;
                while (depth != 0) {
                    state.code_position--;
                    if (result.tokens[state.code_position] == LoopEnd) {
                        depth++;
                    }
                    if (result.tokens[state.code_position] == LoopStart) {
                        depth--;
                    }
                }
            }
            break;
        default:
            printf("Unimplemented Token: %s", token_to_str(token));
            break;
        }

        state.code_position++;
    }

    free(result.tokens);

    interpreter_delete(&state);

    return 0;
}
