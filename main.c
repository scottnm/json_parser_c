#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(const char* error_str)
{
    printf("%s\n", error_str);
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        error("Not enough arguments\n");
    }

    char* input_str = argv[1];
    char* save_ptr;

    // fetch the arguments from the string
    // char *strtok_r(char *str, const char *delim, char **saveptr);
    char* op_1_str = strtok_r(input_str, " ", &save_ptr);
    char* operator_str = strtok_r(NULL, " ", &save_ptr);
    char* op_2_str = strtok_r(NULL, " ", &save_ptr);
    printf("1: %s\n2: %s\n3: %s\n", op_1_str, operator_str, op_2_str);
}
