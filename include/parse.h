#include <stdio.h>
enum CommandKind {
    IncreasePointer, // >
    DecreasePointer, // <
    Add, // +
    Subtract, // -
    OutputChar, // .
    InputChar, // ,
    LoopStart, // [
    LoopEnd, // ]
};

struct Command
{
    enum CommandKind kind;
    union {
        unsigned int count; // how often the command repeats in the code ( kind of like rle)
        int index; // index into the jump table (unused)
    } data;
};

struct ParseResult
{
    unsigned long command_count;
    struct Command* command_buffer;
};

struct ParseResult* parse(char* code, unsigned long size);
char* strip_non_bf_characters(char* buffer, unsigned long buffersize, unsigned long* character_count);
long get_filesize(FILE* file);
