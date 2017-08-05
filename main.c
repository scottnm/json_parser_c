#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char* error_str)
{
    printf("%s\n", error_str);
    exit(1);
}

char* sieve; // use a character pointer so each test only takes up a byte

int main(int argc, char** argv)
{
    char input_buffer[11];
    while (fgets(input_buffer, 11, stdin) != NULL)
    {
        int next_value = strtol(input_buffer, NULL, 10);
        printf("echo: %d\n", next_value);
    }
    printf("done.\n");
}
