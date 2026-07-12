struct InterpreterState
{
    void* tape;
    int tape_size;
    int tape_position;
};

int state_init(struct InterpreterState* state, long cell_count);
void state_delete(struct InterpreterState* state);
