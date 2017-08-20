#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expcharbuf.h"
#include "helpers.h"
#include "json.h"

#define NO_CONT assert("no return" && false);

#define ERRORBUF_SIZE 200
static expcharbuf error_buf;
char getnc(FILE* stream)
{
    char c = getc(stream);
    pushback_char(&error_buf, c);
    return c;
}

void ungetnc(char c, FILE* stream)
{
    ungetc(c, stream);
    --error_buf.top;
}

void logerror_and_quit(FILE* stream, const char* err_str)
{
    fprintf(stderr, "errbuf: ");
    print_expcharbuf(stderr, &error_buf, false);
    fprintf(stderr, "\x1b[33m*\x1b[0m");
    // print the next 10 chars in the file stream
    if (stream != NULL)
    {
        char nc = ' ';
        for (int i = 0; i < 10 && nc != EOF; ++i)
        {
            nc = getc(stream); // use stream because we just don't need to backup to the error buf
            fprintf(stderr, "%c", nc);
        }
        fprintf(stderr, "\n");
    }
    error(err_str);
}

void flush_whitespace(FILE* stream)
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

char* parse_key(FILE* stream)
{
    flush_whitespace(stream);
    char c = getnc(stream);
    if (c != '"')
    {
        logerror_and_quit(stream, "When trying to parse a key, a value other than an opening quote was found");
    }

    expcharbuf key_buf = new_expcharbuf(15);
    for(char next = getnc(stream); next != '"'; next = getnc(stream))
    {
        pushback_char(&key_buf, next);
    }

    flush_whitespace(stream);
    if (getnc(stream) != ':')
    {
        logerror_and_quit(stream, "Missing ':' after key");
    }

    return detach_expcharbuf(&key_buf);
}

double parse_num(FILE* stream)
{
    expcharbuf numbuf = new_expcharbuf(15);

    {
        char nc;
        for(nc = getnc(stream); nc != ',' && nc != '}' && !isspace(nc); nc = getnc(stream))
        {
            pushback_char(&numbuf, nc);
        }
        ungetnc(nc, stream); // put back final , or } for the obj_parser to interpret
    }

    char* end_of_numstr = numbuf.top;
    char* numstr = detach_expcharbuf(&numbuf);
    char* endptr;
    double res = strtod(numstr, &endptr);
    // TODO: handle the case where the number is too large or small (errno case)
    if (endptr != end_of_numstr) // str couldn't be converted because it wasn't a number
    {
        logerror_and_quit(stream, "Invalid number could not be parsed");
    }
    free(numstr);
    flush_whitespace(stream);
    return res;
}

value parse_val(FILE* stream)
{
    flush_whitespace(stream);

    char lookahead = getnc(stream);
    ungetnc(lookahead, stream);

    value parsed_val;

    switch (lookahead)
    {
        case '"': // str val
            logerror_and_quit(stream, "cannot parse strings yet");
            break;
        case '{': // obj
            logerror_and_quit(stream, "cannot parse objects yet");
            break;
        case '[': // arr
            logerror_and_quit(stream, "cannot parse arrays yet");
            break;
        default:  // number
            if (isdigit(lookahead) || lookahead == '+' || lookahead == '-')
            {
                parsed_val.type = V_NUM;
                parsed_val.num = parse_num(stream);
                break;
            }
            logerror_and_quit(stream, "invalid character parsed");
            NO_CONT
    }

    return parsed_val;
}

void parse_obj(FILE* stream)
{
    int parsing = true;
    while (parsing)
    {
        char* key = parse_key(stream);
        printf("next key: %s\n", key);
        free(key);
        value v = parse_val(stream);
        switch(v.type)
        {
            case V_NUM:
                printf("next val: %f\n", v.num);
                break;
            default:
                logerror_and_quit(stream, "invalid value type");
        }
        flush_whitespace(stream);
        char end_of_kv = getnc(stream);
        if (end_of_kv == '}')
        {
            parsing = false;
        }
        else if (end_of_kv != ',')
        {
            if (end_of_kv == EOF)
            {
                ungetnc(EOF, stream);
                logerror_and_quit(stream, "Unexpected EOF");
            }
            logerror_and_quit(stream, "End of keyvalue pair was not followed by a comma or a closing brace");
        }
    }
}

int main(int argc, char** argv)
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
        parse_obj(input_src);
    }
    else
    {
        logerror_and_quit(input_src, "Error, bad obj syntax");
    }

    destroy_expcharbuf(&error_buf);
    return 0;
}
