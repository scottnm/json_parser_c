#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expcharbuf.h"

void
flush_whitespace(
    FILE* stream
    )
{
    char c;
    int reading_whitespace = true;
    while (reading_whitespace)
    {
        c = getc(stream);
        reading_whitespace = isspace(c);
    }
    ungetc(c, stream);
}

const char*
parse_key(
    FILE* stream
    )
{
}

void
parse_obj(
    FILE* stream
    )
{
    while (true)
    {
        const char* key = parse_key(stream);
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

    FILE* input_src = argc < 2 ? stdin : fopen(argv[1], "r");
    flush_whitespace(input_src);
    if (getc(input_src) == '{')
    {
        printf("Start processing obj\n");
        auto res = parse_obj(input_src);
        print_obj(res);
    }
    else
    {
        error("Error, bad obj");
    }
    return 0;
}
