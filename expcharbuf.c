#include <stdio.h>
#include <stdlib.h>
#include "expcharbuf.h"
#include "helpers.h"

expcharbuf
new_expcharbuf(
    int init_cap
    )
{
    char* buf = (char*)calloc(init_cap + 1, sizeof(char)); // extra 1 for nullterm
    return (expcharbuf){buf, buf, buf + init_cap};
}

void
destroy_expcharbuf(
    expcharbuf* buf
	)
{
    free(buf->b);
    buf->b = NULL;
    buf->top = NULL;
    buf->e = NULL;
}

char*
detach_expcharbuf(
    expcharbuf* buf
    )
{
    char* str = buf->b;
    *(buf->top) = '\0';
    buf->b = NULL;
    buf->top = NULL;
    buf->e = NULL;
    return str;
}

void
expandbuf(
    expcharbuf* buf
    )
{
    printf("--expcharbuf called--\n");
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

void
pushback_char(
    expcharbuf* buf,
    char c
    )
{
    if (buf->top == buf->e)
    {
        expandbuf(buf);
    }
    *(buf->top++) = c;
}

void
print_expcharbuf(
    expcharbuf* buf
    )
{
    char* b = buf->b;
    char* top = buf->top;
    while (b != top)
    {
        printf("%c", *(b++)); 
    }
    printf("\n");
}
