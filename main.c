#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char* error_str)
{
    printf("%s\n", error_str);
    exit(1);
}

#define SIEVE_SIZE 10000
char* sieve;


int is_prime(int num)
{
    return !sieve[num - 1];
}

void fill_sieve()
{
    sieve = calloc(SIEVE_SIZE, 1);
    sieve[0] = true;
    for (int i = 2; i <= SIEVE_SIZE / 2; ++i)
    {
        for (int factor = 2; i * factor <= SIEVE_SIZE; ++factor)
        {
            sieve[(i * factor) - 1] = true;
        }
    }
}

int main(int argc, char** argv)
{
    fill_sieve();

    char input_buffer[11];
    while (fgets(input_buffer, 11, stdin) != NULL)
    {
        int next_value = strtol(input_buffer, NULL, 10);
        printf("%d is %s\n", next_value, is_prime(next_value) ? "prime" : "not prime");
    }
    printf("done.\n");
}
