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

void print_frac(frac f)
{
    printf("[%d / %d]\n", f.numer, f.denom);
}

void print_mixed_frac(mixed_frac mf)
{
    printf("%d ", mf.whole_part);
    print_frac(mf.frac_part);
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

void morph_fracs_into_common_form(frac* f1, frac* f2)
{
    f1->numer *= f2->denom;
    f2->numer *= f1->denom;
    f1->denom *= f2->denom;
    f2->denom = f1->denom;
}

frac addf(frac f1, frac f2)
{
    morph_fracs_into_common_form(&f1, &f2);
    return (frac) {f1.numer + f2.numer, f1.denom};
}

frac subf(frac f1, frac f2)
{
    morph_fracs_into_common_form(&f1, &f2);
    return (frac) {f1.numer - f2.numer, f1.denom};
}

frac mulf(frac f1, frac f2)
{
    return (frac) {f1.numer * f2.numer, f1.denom * f2.denom};
}

frac divf(frac f1, frac f2)
{
    return (frac) {f1.numer * f2.denom, f1.denom * f2.numer};
}

frac simplify_frac(frac f)
{
    int n = f.numer;
    int d = f.denom;
    int gcd = 1;
    for (int i = 2; i <= n && i <= d; ++i)
    {
        if (n % i == 0 && d % i == 0)
        {
            gcd = i;
        }
    }
    return (frac) {f.numer / gcd, f.denom / gcd};
}

mixed_frac frac_to_mixed_frac(frac f)
{
    int wholenum = f.numer / f.denom;
    int numer = f.numer % f.denom;
    return (mixed_frac) { f.numer / f.denom, (frac) {f.numer % f.denom, f.denom}};
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
    char operator_chr = *strtok_r(NULL, " ", &save_ptr);
    char* op_2_str = strtok_r(NULL, " ", &save_ptr);

    // turn tokens into fraction structs
    frac op1 = parse_frac(op_1_str);
    frac op2 = parse_frac(op_2_str);
    // perform math on fractions
    frac res;
    switch(operator_chr)
    {
        case '+':
            res = addf(op1, op2);
            break;
        case '-':
            res = subf(op1, op2);
            break;
        case '*':
            res = mulf(op1, op2);
            break;
        case '/':
            res = divf(op1, op2);
            break;
        default:
            error("Operation not supported");
            break;
    }
    res = simplify_frac(res);
    print_frac(res);
    mixed_frac mixed_form_res = frac_to_mixed_frac(res);
    print_mixed_frac(mixed_form_res);
}
