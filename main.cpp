#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char_vec.h"
#include "helpers.h"
#include "json.h"

// MACROS
#define NO_CONT assert("no return" && false);
#define ERRORBUF_SIZE 200

// STATIC FUNC DECLARATIONS
static char getnc(FILE* stream);
static void ungetnc(char c, FILE* stream);
static char peeknc(FILE* stream);
static void logerror_and_quit(FILE* stream, const char* err_str);
static void flush_whitespace(FILE* stream);
static json_obj* parse_obj(FILE* stream);
static char* parse_key(FILE* stream);
static json_value parse_val(FILE* stream);
static double parse_num(FILE* stream);
static char* parse_str(FILE* stream);
static json_arr* parse_arr(FILE* stream);
static void print_obj(FILE* stream, const json_obj& obj, int num_spaces = 0);
static void print_arr(FILE* stream, const json_arr& arr, int num_spaces = 0);

// STATIC VALUES
static char_vec error_buf;

// DEFINITIONS
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

    error_buf = new_char_vec(ERRORBUF_SIZE);
    FILE* in_stream = argc < 2 ? stdin : fopen(argv[1], "r");
    json_obj* obj = parse_obj(in_stream);
    print_obj(in_stream, *obj);
    destroy_char_vec(&error_buf);
    return 0;
}

static void inline space_padding(int spaces)
{
    for (int i = 0; i < spaces; ++i) { printf(" "); }
}

static void print_obj(FILE* stream, const json_obj& obj, int num_spaces)
{
    int init_padding = num_spaces;
    num_spaces += 4;
    space_padding(init_padding);
    printf("{\n");
    for (std::pair<cstr, json_value> it : obj)
    {
        space_padding(num_spaces);
        printf("\"%s\": ", it.first);
        json_value v = it.second;
        switch(v.type)
        {
            case json_vtype::NUM:
                printf("%f\n", v.num);
                break;
            case json_vtype::STR:
                printf("%s\n", v.str);
                break;
            case json_vtype::OBJ:
                printf("\n");
                print_obj(stream, *(v.obj), num_spaces);
                break;
            case json_vtype::ARR:
                printf("\n");
                print_arr(stream, *(v.arr), num_spaces);
                break;
            default:
                logerror_and_quit(stream, "invalid value type");
                break;
        }
    }
    space_padding(init_padding);
    printf("}\n");
}

static void print_arr(FILE* stream, const json_arr& arr, int num_spaces)
{
    int init_padding = num_spaces;
    num_spaces += 4;
    space_padding(init_padding);
    printf("[\n");
    for (const json_value& v : arr)
    {
        space_padding(num_spaces);
        switch(v.type)
        {
            case json_vtype::NUM:
                printf("%f,\n", v.num);
                break;
            case json_vtype::STR:
                printf("%s,\n", v.str);
                break;
            case json_vtype::OBJ:
                print_obj(stream, *(v.obj), num_spaces);
                space_padding(num_spaces);
                printf(",\n");
                break;
            case json_vtype::ARR:
                print_arr(stream, *(v.arr), num_spaces);
                break;
            default:
                logerror_and_quit(stream, "invalid value type");
                break;
        }
    }
    space_padding(init_padding);
    printf("]\n");
}

json_obj* parse_obj(FILE* stream)
{
    // assert that the object begins with the { char
    flush_whitespace(stream);
    if (getnc(stream) != '{')
    {
        logerror_and_quit(stream, "Error, bad obj syntax");
    }

    json_obj* obj = new json_obj;
    bool parsing = true;
    while (parsing)
    {
        cstr key = parse_key(stream);
        json_value val = parse_val(stream);
        obj->emplace(key, val);

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
    return obj;
}

json_arr* parse_arr(FILE* stream)
{
    flush_whitespace(stream);
    if (getnc(stream) != '[')
    {
        logerror_and_quit(stream, "When trying to parse an array, a value other than an opening brace was found");
    }

    json_arr* arr = new json_arr;
    bool parsing = true;
    while (parsing)
    {
        arr->push_back(parse_val(stream));
        flush_whitespace(stream);
        char nc = getnc(stream);

        // TODO:
        // write error tests homie
        if (nc == ']')
        {
            parsing = false;
        }
        else if (nc == EOF)
        {
            ungetnc(EOF, stream);
            logerror_and_quit(stream, "Unexpected EOF while parsing arr");
        }
        else if (nc != ',')
        {
            logerror_and_quit(stream, "Unexpected character after parsing value in array");
        }
    }

    return arr;
}

char* parse_key(FILE* stream)
{
    char* key = parse_str(stream);
    if (getnc(stream) != ':')
    {
        logerror_and_quit(stream, "Missing ':' after key");
    }

    return key;
}

char* parse_str(FILE* stream)
{
    flush_whitespace(stream);
    char c = getnc(stream);
    if (c != '"')
    {
        logerror_and_quit(stream, "When trying to parse a string, a value other than an opening quote was found");
    }

    char_vec str_buf = new_char_vec(15);
    for(char next = getnc(stream); next != '"'; next = getnc(stream))
    {
        push_char(&str_buf, next);
    }
    return detach_char_vec(&str_buf);
}

double parse_num(FILE* stream)
{
    char_vec numbuf = new_char_vec(15);

    {
        char nc;
        for(nc = getnc(stream); nc != ',' && nc != '}' && !isspace(nc); nc = getnc(stream))
        {
            push_char(&numbuf, nc);
        }
        ungetnc(nc, stream); // put back final , or } for the obj_parser to interpret
    }

    char* end_of_numstr = numbuf.top;
    cbuf num_str_buf = detach_char_vec(&numbuf);
    char* endptr;
    double res = strtod(num_str_buf, &endptr);
    // TODO: handle the case where the number is too large or small (errno case)
    if (endptr != end_of_numstr) // str couldn't be converted because it wasn't a number
    {
        logerror_and_quit(stream, "Invalid number could not be parsed");
    }
    free(num_str_buf);
    flush_whitespace(stream);
    return res;
}

json_value parse_val(FILE* stream)
{
    flush_whitespace(stream);

    json_value parsed_val;
    char lookahead = peeknc(stream);
    switch (lookahead)
    {
        case '"': // str val
            parsed_val.type = json_vtype::STR;
            parsed_val.str = parse_str(stream);
            break;
        case '{': // obj
            parsed_val.type = json_vtype::OBJ;
            parsed_val.obj = parse_obj(stream);
            break;
        case '[': // arr
            parsed_val.type = json_vtype::ARR;
            parsed_val.arr = parse_arr(stream);
            break;
        default:  // number
            if (isdigit(lookahead) || lookahead == '+' || lookahead == '-')
            {
                parsed_val.type = json_vtype::NUM;
                parsed_val.num = parse_num(stream);
                break;
            }
            logerror_and_quit(stream, "invalid character parsed");
            NO_CONT
    }

    return parsed_val;
}

char getnc(FILE* stream)
{
    char c = getc(stream);
    push_char(&error_buf, c);
    return c;
}

void ungetnc(char c, FILE* stream)
{
    ungetc(c, stream);
    --error_buf.top;
}

char peeknc(FILE* stream)
{
    char c = getc(stream);
    ungetc(c, stream);
    return c;
}

void logerror_and_quit(FILE* stream, const char* err_str)
{
    FILE* outloc = stderr;

    fprintf(outloc, "errbuf: ");
    print_char_vec(outloc, &error_buf, false);
    fprintf(outloc, "\x1b[33m(*)\x1b[0m");
    // print the next 10 chars in the file stream
    if (stream != NULL)
    {
        char nc = getnc(stream);
        for (int i = 0; i < 10 && nc != EOF; ++i)
        {
            fprintf(outloc, "%c", nc);
            nc = getc(stream); // use stream because we just don't need to backup to the error buf
        }
        fprintf(outloc, "\n");
    }
    error(err_str);
}

void flush_whitespace(FILE* stream)
{
    char c;
    bool reading_whitespace = true;
    while (reading_whitespace)
    {
        c = getnc(stream);
        reading_whitespace = isspace(c);
    }
    ungetnc(c, stream);
}
