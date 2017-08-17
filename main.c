#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expcharbuf.h"
#include "helpers.h"

#define ERRORBUF_SIZE 200
static expcharbuf error_buf;
char getnc(
    FILE* stream
    )
{
    char c = getc(stream);
    pushback_char(&error_buf, c);
    return c;
}

void ungetnc(
    char c,
    FILE* stream
    )
{
    ungetc(c, stream);
    --error_buf.top;
}

void
flush_whitespace(
    FILE* stream
    )
{
    char c;
    int reading_whitespace = true;
    while (reading_whitespace)
    {
        c = getnc(stream);
        reading_whitespace = isspace(c);
    }
    ungetnc(c, stream);
}

const char*
parse_key(
    FILE* stream
    )
{
    char c = getnc(stream);    
    if (c != '"')
    {
        print_expcharbuf(&error_buf);
        error("When trying to parse a key, a value other than an opening quote was found");
    }

    expcharbuf key_buf = new_expcharbuf(15);
    for(char next = getnc(stream); next != '"'; next = getnc(stream))
    {
        pushback_char(&key_buf, next);
    }

    return detach_expcharbuf(&key_buf);
}

void
parse_obj(
    FILE* stream
    )
{
    int parsing = true;
    while (parsing)
    {
        flush_whitespace(stream);
        const char* key = parse_key(stream);
        printf("next key: %s\n", key);
        for (char nc = getnc(stream); parsing && nc != ','; nc = getnc(stream))
        {
            parsing = nc != EOF;
        }
    }
}

int
main(
    int argc,
    char** argv
    )
{
    // check for help/usage
    if (argc >= 2)
    {
        for(int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "-h") == 0)
            {
                printf("Usage: ./main <json_file>\n"
                       "Usage: <json_output> | ./main\n");
                return 0;
            }
        }
    }

    error_buf = new_expcharbuf(ERRORBUF_SIZE);

    FILE* input_src = argc < 2 ? stdin : fopen(argv[1], "r");
    flush_whitespace(input_src);
    if (getnc(input_src) == '{')
    {
        printf("Start processing obj\n");
        parse_obj(input_src);
    }
    else
    {
        error("Error, bad obj");
    }

    destroy_expcharbuf(&error_buf);
    return 0;
}
