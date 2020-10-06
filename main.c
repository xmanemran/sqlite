#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
} Statement;

typedef struct {
    char *buffer;
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer(void) {
    InputBuffer *input_buffer = (InputBuffer *) malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt() {
    printf("db > ");
}

void read_line(InputBuffer *input_buffer) {
    ssize_t byte_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (byte_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = byte_read - 1;
    input_buffer->buffer[input_buffer->input_length] = 0;
}

void close_buffer(InputBuffer *input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_buffer(input_buffer);
        exit(EXIT_SUCCESS);
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* inputBuffer, Statement* statement){
    if(strncmp(inputBuffer->buffer, "insert", 6) == 0){
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strcmp(inputBuffer->buffer, "select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return  PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){
    switch (statement->type) {
        case STATEMENT_SELECT:
            printf("This is where we would a select\n");
            break;
        case STATEMENT_INSERT:
            printf("This is where we would a insert\n");
            break;
    }
}

int main() {
    InputBuffer *input_buffer = new_input_buffer();
    while (1) {
        print_prompt();
        read_line(input_buffer);

        if(input_buffer->buffer[0] == '.'){
            switch (do_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'.\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
}
