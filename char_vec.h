#ifndef __CHAR_VEC_H__
#define __CHAR_VEC_H__

typedef struct
{
    char* b;
    char* top;
    char* e;
} char_vec;

char_vec
new_char_vec(size_t init_cap);

void
destroy_char_vec(char_vec* buf);

char*
detach_char_vec(char_vec* buf);

void
push_char(char_vec* buf, char c);

void
print_char_vec(FILE* output, char_vec* buf, bool newline);

#endif //__CHAR_VEC_H__
