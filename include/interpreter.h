#ifndef BFC_INTERPRETER_H
#define BFC_INTERPRETER_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct Tape
{
    uint8_t* buffer;
    uint32_t tape_length;
    uint32_t cursor_position;
};

struct InterpreterState
{
    struct Tape tape;
    uint32_t code_position;
    bool should_exit;
};

int interpreter_init(struct InterpreterState* state, size_t tape_length);
void interpreter_delete(struct InterpreterState* state);

#endif // BFC_INTERPRETER_H
