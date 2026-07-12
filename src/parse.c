#include "parse.h"

#include <stdio.h>
#include <stdlib.h>

// TODO: jump table calculations

struct ParseResult* parse(char* code, unsigned long size)
{
    unsigned long stripped_size = 0;

    // strip all non brainfuck characters out of the code buffer
    char* code_stripped = strip_non_bf_characters(code, size, &stripped_size);

    // propagate error upwards
    if (code_stripped == NULL) {
        return NULL;
    }

    // buffer used to store all the parsed commands
    struct Command* command_buffer = malloc(sizeof(struct Command) * stripped_size);
    unsigned long command_index = 0;

    // loops over all of the characters in the stripped buffer
    // if last command is same as before (for add, sub, pointer left & right) the amount is increased
    struct Command current_command;
    char last_command_char = ' ';
    for (unsigned long i = 0; i < stripped_size; i++) {
        char command_char = code_stripped[i];

        if (command_char == last_command_char) {
            if (command_char != '[' && command_char != ']') {
                current_command.data.count++;
                continue;
            }
        }
        else {
            // if its a new command, move the old one into the buffer
            // except if it is the first command (i = 0) then it should not be pushed
            if (i != 0) {
                command_buffer[command_index] = current_command;
                command_index++;
            }
            last_command_char = command_char;
        }

        current_command.data.count = 1;

        switch (command_char) {
        case '>':
            current_command.kind = IncreasePointer;
            break;
        case '<':
            current_command.kind = DecreasePointer;
            break;
        case '+':
            current_command.kind = Add;
            break;
        case '-':
            current_command.kind = Subtract;
            break;
        case '.':
            current_command.kind = OutputChar;
            break;
        case ',':
            current_command.kind = InputChar;
            break;
        case '[':
            current_command.kind = LoopStart;
            break;
        case ']':
            current_command.kind = LoopEnd;
            break;
        default:
            printf("THIS SHOULD NOT HAPPEN!");
            break;
        }
    }

    free(code_stripped);
    //
    // realloc to trim the command_buffer to only the size it needs
    void* tmp = realloc(command_buffer, command_index * sizeof(struct Command));
    if (tmp == NULL) {
        free(command_buffer);
        return NULL;
    }

    // parse result struct to be returned to caller
    struct ParseResult* result = malloc(sizeof(struct ParseResult));

    result->command_buffer = tmp;
    result->command_count = command_index;

    return result;
}

char* strip_non_bf_characters(char* buffer, unsigned long buffer_size, unsigned long* character_count)
{
    // buffer to contain the trimmed data
    char* code_buffer_trimmed = malloc(buffer_size);

    if (code_buffer_trimmed == NULL) {
        return NULL;
    }

    unsigned long code_buffer_trimmed_index = 0;

    // loop over all the characters, copy bf chars to the trimmed buffer, everything else is skipped
    for (unsigned long i = 0; i < buffer_size; i++) {
        switch (buffer[i]) {
        case '>':
        case '<':
        case '+':
        case '-':
        case '.':
        case ',':
        case '[':
        case ']':
            code_buffer_trimmed[code_buffer_trimmed_index] = buffer[i];
            code_buffer_trimmed_index++;
            break;
        }
    }

    *character_count = code_buffer_trimmed_index;

    // realloc trimmed buffer to only the size that is needed
    void* tmp = realloc(code_buffer_trimmed, code_buffer_trimmed_index);
    if (tmp == NULL) {
        free(code_buffer_trimmed);
        return NULL;
    }

    return tmp;
}

long get_filesize(FILE* file)
{
    long current_position = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current_position, SEEK_SET);

    return size;
}
