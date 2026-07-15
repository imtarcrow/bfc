#include "token.h"

#include <stdio.h>
#include <stdlib.h>

struct TokenizationResult tokenize(char* code, size_t length, bool use_debug_tokens)
{
    struct TokenizationResult result;
    enum Token* token_buffer = malloc(sizeof(enum Token) * length);
    if (token_buffer == NULL) {
        result.tokens = NULL;
        result.token_count = 0;
        return result;
    }

    size_t token_buffer_index = 0;

    for (size_t i = 0; i < length; i++) {
        char command = code[i];

        switch (command) {
        case '>':
            token_buffer[token_buffer_index++] = IncreasePointer;
            break;
        case '<':
            token_buffer[token_buffer_index++] = DecreasePointer;
            break;
        case '+':
            token_buffer[token_buffer_index++] = Add;
            break;
        case '-':
            token_buffer[token_buffer_index++] = Subtract;
            break;
        case '.':
            token_buffer[token_buffer_index++] = OutputChar;
            break;
        case ',':
            token_buffer[token_buffer_index++] = InputChar;
            break;
        case '[':
            token_buffer[token_buffer_index++] = LoopStart;
            break;
        case ']':
            token_buffer[token_buffer_index++] = LoopEnd;
            break;
        case '@':
            if (use_debug_tokens) {
                token_buffer[token_buffer_index++] = Breakpoint;
            }
            break;
        case '#':
            if (use_debug_tokens) {
                token_buffer[token_buffer_index++] = PrintDebugState;
            }
            break;
        default:
            break;
        }
    }

    void* tmp = realloc(token_buffer, token_buffer_index * sizeof(enum Token));
    if (tmp == NULL) {
        free(token_buffer);
        result.tokens = NULL;
        result.token_count = 0;
        return result;
    }

    result.tokens = tmp;
    result.token_count = token_buffer_index;

    return result;
}

struct TokenizationResult tokenize_file(char* filename, bool use_debug_tokens)
{

    struct TokenizationResult result;
    result.tokens = NULL;
    result.token_count = 0;

    FILE* code = fopen(filename, "r");
    if (code == NULL) {
        printf("File could not be opened!\n");
        return result;
    }

    size_t filesize = get_filesize(code);
    printf("file: %s | %p | size: %ld\n", filename, (void*)code, filesize);

    char* code_buffer = malloc(filesize);
    if (code_buffer == NULL) {
        fclose(code);
        return result;
    }

    fread(code_buffer, 1, filesize, code);
    fclose(code);

    printf("\n====================\n\n");
    for (size_t i = 0; i < filesize; i++) {
        printf("%c", code_buffer[i]);
    }

    printf("\n====================\n\n");

    result = tokenize(code_buffer, filesize, use_debug_tokens);

    free(code_buffer);

    printf("\n====================\n\n");
    printf("Token count: %zu\n", result.token_count);
    for (size_t i = 0; i < result.token_count; i++) {
        printf("%s\n", token_to_str(result.tokens[i]));
    }

    printf("\n====================\n\n");

    return result;
}

size_t get_filesize(FILE* file)
{
    long current_position = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current_position, SEEK_SET);

    return size;
}

const char* token_to_str(enum Token token)
{
    switch (token) {
    case IncreasePointer:
        return "IncreasePointer";
    case DecreasePointer:
        return "DecreasePointer";
    case Add:
        return "Add";
    case Subtract:
        return "Subtract";
    case OutputChar:
        return "OutputChar";
    case InputChar:
        return "InputChar";
    case LoopStart:
        return "LoopStart";
    case LoopEnd:
        return "LoopEnd";
    case Breakpoint:
        return "Breakpoint";
    case PrintDebugState:
        return "PrintDebugState";
    }
    return "Unknown";
}
