#include "state.h"

#include <stdlib.h>

int state_init(struct InterpreterState* state, unsigned long cell_count)
{
    state->tape_size = cell_count;
    state->tape_position = 0;

    state->tape = calloc(cell_count, 1);

    if (state->tape == NULL) {
        return 1;
    }

    return 0;
}

void state_delete(struct InterpreterState* state)
{
    if (state->tape != NULL) {
        free(state->tape);
    }
}
