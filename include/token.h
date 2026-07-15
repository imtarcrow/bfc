#ifndef BFC_TOKEN_H
#define BFC_TOKEN_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

enum Token {
    IncreasePointer, // >
    DecreasePointer, // <
    Add, // +
    Subtract, // -
    OutputChar, // .
    InputChar, // ,
    LoopStart, // [
    LoopEnd, // ]
    Breakpoint,
    PrintDebugState,
};

struct TokenizationResult
{
    enum Token* tokens;
    size_t token_count;
};

struct TokenizationResult tokenize(char* code, size_t length, bool use_debug_tokens);
struct TokenizationResult tokenize_file(char* filename, bool use_debug_tokens);

size_t get_filesize(FILE* file);
const char* token_to_str(enum Token token);

#endif // BFC_TOKEN_H
