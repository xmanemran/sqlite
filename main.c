#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;


typedef struct {
    StatementType type;
} Statement;

InputBuffer* new_input_buffer();
void print_prompt();
void read_input(InputBuffer*);
void close_input_buffer(InputBuffer*);
MetaCommandResult do_meta_command(InputBuffer*);
PrepareResult prepare_statement(InputBuffer*, Statement *);
void execute_statement(Statement*);

int main() {
    InputBuffer* input_buffer = new_input_buffer();

    while (true){
        print_prompt();
        read_input(input_buffer);

        if(input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command %s\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of %s\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");

    }
}

void execute_statement(Statement* statement){
    switch (statement->type) {
        case STATEMENT_INSERT:
            printf("This will be insert\n");
            break;
        case STATEMENT_SELECT:
            printf("This will be insert\n");
            break;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    if(strncmp(input_buffer->buffer, "insert", 6) == 0){
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }

    if(strncmp(input_buffer->buffer, "select", 6) == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if(strcmp(input_buffer->buffer, ".exit") == 0){
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}

void read_input(InputBuffer* input_buffer){
    ssize_t byte_read = getline(&input_buffer->buffer, &input_buffer->buffer_length, stdin);

    if(byte_read <= 0){
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing new line
    input_buffer->input_length = byte_read-1;
    input_buffer->buffer[byte_read-1] = 0;
}

InputBuffer* new_input_buffer(){
    InputBuffer* inputBuffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
    return inputBuffer;
}

void print_prompt(){
    printf("db > ");
}
#pragma clang diagnostic pop