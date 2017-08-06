#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char* error_str)
{
    printf("%s\n", error_str);
    exit(1);
}

struct sieve_node
{
    int val;
    struct sieve_node* next;
};
typedef struct sieve_node sieve_node;

typedef struct
{
    sieve_node* first;
    sieve_node* last;
} sieve_ll;

void add_sieve_node(sieve_ll* list, int val)
{
    sieve_node* new_node = (sieve_node*)malloc(sizeof(sieve_node));
    new_node->val = val;
    new_node->next = NULL;
    if (list->first == NULL)
    {
        list->first = new_node;
        list->last = new_node;
    }
    else
    {
        list->last->next = new_node;
        list->last = new_node;
    }
}

#define SIEVE_SIZE 10000
sieve_ll sieve[SIEVE_SIZE] = {0};

int is_prime(int num)
{
    return sieve[num - 1].first == NULL;
}

void fill_sieve()
{
    // init 1
    add_sieve_node(sieve, 1);
    for (int i = 2; i <= SIEVE_SIZE / 2; ++i)
    {
        for (int factor = 2; i * factor <= SIEVE_SIZE; ++factor)
        {
            add_sieve_node(sieve + (i * factor - 1), i);
        }
    }
}

void print_factors(int val)
{
    sieve_ll list = sieve[val - 1];
    if(list.first == NULL)
    {
        printf("NO FACTORS\n");
    }
    else
    {
        sieve_node* current = list.first;
        while(current->next != NULL)
        {
            printf("%d, ", current->val);
            current = current->next;
        }
        printf("%d\n", current->val);
    }
}

int main(int argc, char** argv)
{
    fill_sieve();

    char input_buffer[11];
    int next_value = 1;
    while (fgets(input_buffer, 11, stdin) != NULL)
    {
        if (input_buffer[0] == 'f')
        {
            print_factors(next_value);
        }
        else
        {
            next_value = strtol(input_buffer, NULL, 10);
            printf("%d is %s\n", next_value, is_prime(next_value) ? "prime" : "not prime");
        }
    }
    printf("done.\n");
}
