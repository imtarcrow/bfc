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
    struct Command* command_buffer;
    unsigned long command_count;
    struct JumpTableEntry* jump_table;
    unsigned long jump_table_entry_count;
};

struct JumpTableEntry
{
    unsigned long start;
    unsigned long end;
};

struct ParseResult* parse(char* code, unsigned long size);

struct JumpTableEntry* generate_jumptable(struct Command* command_buffer, unsigned long command_count,
                                          unsigned long* entry_count);
struct Command* generate_command_buffer(char* code, unsigned long size, unsigned long* command_count);

char* strip_non_bf_characters(char* buffer, unsigned long buffersize, unsigned long* character_count);
long get_filesize(FILE* file);
