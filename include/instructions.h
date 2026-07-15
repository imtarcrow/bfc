#ifndef BFC_INSTRUCTION_H
#define BFC_INSTRUCTION_H

#include <stdint.h>

enum InstructionKind {
    Add,
    Subtract,
    IncreasePointer,
    DecreasePointer,
    OutputChar,
    InputChar,
    LoopStart,
    LoopEnd,
    ClearCell,
    Breakpoint,
    PrintDebugState,
};

struct Instruction
{
    enum InstructionKind kind;
    union {
        uint32_t repetition_count;
        uint32_t jump_table_index;
    } data;
};

#endif // BFC_INSTRUCION_H
