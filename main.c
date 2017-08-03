#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int numer;
    int denom;
} frac;

typedef struct
{
    int whole_part;
    frac frac_part;
} mixed_frac;

void error(const char* error_str)
{
    printf("%s\n", error_str);
    exit(1);
}

frac parse_frac(char* frac_str)
{
    frac ret_val = {1, 1};

    char* frac_split = strchr(frac_str, '_');
    // not a mixed fraction
    if (frac_split == NULL)
    {
        char* frac_divider;
        int numerator = strtol(frac_str, &frac_divider, 10);

        // if strtol didn't find an invalid character than
        // the frac_str is just a whole number
        if (*frac_divider == '\0')
        {
            return (frac){ numerator, 1 };
        }

        int denominator = strtol(frac_divider + 1, NULL, 10);
        return (frac){ numerator, denominator };
    }

    else
    {
        int whole_number = strtol(frac_str, NULL, 10);
        char* frac_divider;
        int numerator = strtol(frac_split + 1, &frac_divider, 10);
        int denominator = strtol(frac_divider + 1, NULL, 10);
        numerator += whole_number * denominator;
        return (frac){ numerator, denominator };
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        error("Not enough arguments\n");
    }

    char* expression_str = argv[1];
    char* save_ptr;

    // split the arguments from input string into tokens
    // char *strtok_r(char *str, const char *delim, char **saveptr);
    char* op_1_str = strtok_r(expression_str, " ", &save_ptr);
    char* operator_str = strtok_r(NULL, " ", &save_ptr);
    char* op_2_str = strtok_r(NULL, " ", &save_ptr);

    // turn tokens into fraction structs
    // change so both fractions have same denom
    // perform math on fractions
    // reduce
    //      - divide out whole numbers until num is smaller than denom
    //      - save as whole num
    //      - reduce fractional part
    //      - print out result

    frac res = parse_frac(op_1_str);
    frac res2 = parse_frac(op_2_str);
    printf(" %d \n---\n %d \n\n", res.numer, res.denom);
    printf(" %d \n---\n %d \n", res2.numer, res2.denom);
}
