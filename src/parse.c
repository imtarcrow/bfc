#include "parse.h"

#include <stdio.h>
#include <stdlib.h>

struct ParseResult* parse(char* code, unsigned long size)
{
    unsigned long stripped_size = 0;
    char* code_stripped = strip_non_bf_characters(code, size, &stripped_size);

    // Propagate error upwards
    if (code_stripped == NULL) {
        return NULL;
    }

    struct Command* command_buffer = malloc(sizeof(struct Command) * stripped_size);
    struct Command current_command;
    unsigned long command_buffer_index = 0;

    char last_c = ' ';
    for (unsigned long i = 0; i < stripped_size; i++) {
        char c = code_stripped[i];

        if (c == last_c && c != '[' && c != ']') {
            current_command.data.count++;
            continue;
        }
        else {
            command_buffer[command_buffer_index] = current_command;
            command_buffer_index++;
        }

        switch (c) {
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

    unsigned long command_count = command_buffer_index + 1;

    void* tmp = realloc(command_buffer, command_count * sizeof(struct Command));
    if (tmp == NULL) {
        free(command_buffer);
        return NULL;
    }

    struct ParseResult* result = malloc(sizeof(struct ParseResult));

    result->command_buffer = tmp;
    result->command_count = command_count;

    return result;
}

char* strip_non_bf_characters(char* buffer, unsigned long buffer_size, unsigned long* character_count)
{
    char* code_buffer_trimmed = malloc(buffer_size);

    if (code_buffer_trimmed == NULL) {
        return NULL;
    }

    unsigned long code_buffer_trimmed_index = 0;

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
