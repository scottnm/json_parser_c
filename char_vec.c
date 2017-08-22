#include <stdio.h>
#include <stdlib.h>
#include "char_vec.h"
#include "helpers.h"

static
void
expand_char_vec(char_vec* buf)
{
    printf("--expand char_vec called--\n");
    int size = buf->top - buf->b;
    int cap = buf->e - buf->b;
    buf->b = realloc(buf->b, sizeof(char) * 2 * cap + 1); // extra 1 for nullterm
    if (buf->b == NULL)
    {
        error("Resizing expandable char buf failed");
    }
    buf->top = size + buf->b;
    buf->e = 2 * cap + buf->b;
}


char_vec
new_char_vec(int init_cap)
{
    char* buf = (char*)calloc(init_cap + 1, sizeof(char)); // extra 1 for nullterm
    return (char_vec){buf, buf, buf + init_cap};
}

void
destroy_char_vec(char_vec* buf)
{
    free(buf->b);
    buf->b = NULL;
    buf->top = NULL;
    buf->e = NULL;
}

char*
detach_char_vec(char_vec* buf)
{
    char* str = buf->b;
    *(buf->top) = '\0';
    buf->b = NULL;
    buf->top = NULL;
    buf->e = NULL;
    return str;
}

void
push_char(char_vec* buf, char c)
{
    if (buf->top == buf->e)
    {
        expand_char_vec(buf);
    }
    *(buf->top++) = c;
}

void
print_char_vec(FILE* output, char_vec* buf, int newline)
{
    char* b = buf->b;
    char* top = buf->top;
    while (b != top)
    {
        fprintf(output, "%c", *(b++)); 
    }
    if (newline)
    {
        fprintf(output, "\n");
    }
}
