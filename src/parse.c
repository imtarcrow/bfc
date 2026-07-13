#include "parse.h"

#include <stdio.h>
#include <stdlib.h>

// TODO: jump table calculations

struct ParseResult* parse(char* code, unsigned long size)
{

    // strip all non brainfuck characters out of the code buffer
    unsigned long stripped_size = 0;
    char* code_stripped = strip_non_bf_characters(code, size, &stripped_size);
    if (code_stripped == NULL) {
        return NULL;
    }

    // turn the brainfuck code into keywords (command enum)
    unsigned long command_count = 0;
    struct Command* command_buffer = generate_command_buffer(code_stripped, stripped_size, &command_count);
    if (command_buffer == NULL) {
        return NULL;
    }

    unsigned long entry_count = 0;
    struct JumpTableEntry* jump_table = generate_jumptable(command_buffer, command_count, &entry_count);

    free(code_stripped);

    // parse result struct to be returned to caller
    struct ParseResult* result = malloc(sizeof(struct ParseResult));

    result->command_buffer = command_buffer;
    result->command_count = command_count;
    result->jump_table = jump_table;
    result->jump_table_entry_count = entry_count;

    return result;
}

struct Command* generate_command_buffer(char* code, unsigned long size, unsigned long* command_count)
{
    // buffer used to store all the parsed commands
    struct Command* command_buffer = malloc(sizeof(struct Command) * size);
    unsigned long command_index = 0;

    // loops over all of the characters in the stripped buffer
    // if last command is same as before (for add, sub, pointer left & right) the amount is increased
    struct Command current_command;
    char last_command_char = ' ';
    for (unsigned long i = 0; i < size; i++) {

        char command_char = code[i];

        if (command_char == last_command_char) {
            if (command_char != '[' && command_char != ']') {
                current_command.data.count++;
                continue;
            }
        }

        // if its a new command, move the old one into the buffer
        // except if it is the first command (i = 0) then it should not be pushed
        if (i != 0) {
            command_buffer[command_index] = current_command;
            command_index++;
        }

        last_command_char = command_char;
        current_command.data.count = 1;

        // switch case is only reached if it is a new command
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

    if (size != 0) {
        command_buffer[command_index] = current_command;
        command_index++;
    }

    *command_count = command_index;

    // realloc to trim the command_buffer to only the size it needs
    void* tmp = realloc(command_buffer, command_index * sizeof(struct Command));
    if (tmp == NULL) {
        free(command_buffer);
        return NULL;
    }

    return tmp;
}

struct JumpTableEntry* generate_jumptable(struct Command* command_buffer, unsigned long command_count,
                                          unsigned long* entry_count)
{
    *entry_count = 0;
    struct JumpTableEntry* entries = malloc(sizeof(struct JumpTableEntry) * (command_count / 2) + 1);
    if (entries == NULL) {
        return NULL;
    }
    printf("Command count: %ld\n", command_count);

    for (unsigned long i = 0; i < command_count; i++) {
        if (command_buffer[i].kind == LoopStart) {
            unsigned int depth = 1;
            unsigned long j = i;
            while (depth != 0) {
                j++;
                if (j >= command_count) {
                    break; // no matching loop end command found!
                }

                if (command_buffer[j].kind == LoopStart) {
                    depth++;
                }
                else if (command_buffer[j].kind == LoopEnd) {
                    depth--;
                }
            }

            if (depth != 0) {
                free(entries);
                return NULL;
            }

            struct JumpTableEntry entry;

            entry.start = i;
            entry.end = j;
            entries[*entry_count] = entry;

            command_buffer[i].data.index = *entry_count;
            command_buffer[j].data.index = *entry_count;

            (*entry_count)++;
        }
    }

    // realloc to trim the command_buffer to only the size it needs
    void* tmp = realloc(entries, *entry_count * sizeof(struct JumpTableEntry));
    if (tmp == NULL) {
        free(entries);
        return NULL;
    }

    return tmp;
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
