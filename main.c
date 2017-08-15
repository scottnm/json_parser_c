#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
error(
    const char* error_str
    )
{
    printf("%s\n", error_str);
    exit(1);
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
        c = getc(stream);
        reading_whitespace = isspace(c);
    }
    ungetc(c, stream);
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
    }
    else
    {
        error("Error, bad obj");
    }
}
