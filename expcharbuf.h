#ifndef __EXPCHARBUF_H__
#define __EXPCHARBUF_H__

typedef struct
{
    char* b;
    char* top;
    char* e;
} expcharbuf;

expcharbuf
new_expcharbuf(int init_cap);

void
destroy_expcharbuf(expcharbuf* buf);

char*
detach_expcharbuf(expcharbuf* buf);

void
expandbuf(expcharbuf* buf);

void
pushback_char(expcharbuf* buf, char c);

void
print_expcharbuf(expcharbuf* buf);

#endif //__EXPCHARBUF_H__
