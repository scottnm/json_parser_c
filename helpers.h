#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdio.h>
#include <stdlib.h>

void
error(
    const char* error_str
    )
{
    printf("%s\n", error_str);
    exit(1);
}

#endif //__HELPERS_H__
