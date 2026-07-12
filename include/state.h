#include <stdint.h>

struct InterpreterState
{
    uint8_t* tape;
    unsigned long tape_size;
    unsigned long tape_position;
    unsigned long code_position;
};

int state_init(struct InterpreterState* state, unsigned long cell_count);
void state_delete(struct InterpreterState* state);
