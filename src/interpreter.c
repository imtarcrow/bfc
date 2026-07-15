#include "interpreter.h"

#include <stdlib.h>

int interpreter_init(struct InterpreterState* state, size_t tape_length)
{
    state->should_exit = false;
    state->code_position = 0;
    state->tape.cursor_position = 0;
    state->tape.tape_length = tape_length;

    state->tape.buffer = calloc(tape_length, 1);

    if (state->tape.buffer == NULL) {
        return 1;
    }

    return 0;
}

void interpreter_delete(struct InterpreterState* state)
{
    if (state->tape.buffer != NULL) {
        free(state->tape.buffer);
    }
}
