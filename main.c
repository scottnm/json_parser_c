#include <stdio.h>
#include <stdbool.h>

#define LINE_LEN 255

void error(const char* error_str)
{
    printf("%s\n", error_str);
    exit(1);
}

void flush_line(FILE* file)
{
    char line[LINE_LEN + 1] = {0};
    while (fgets(line, LINE_LEN, file) != NULL && strchr(line, '\n') == NULL) {}
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        error("Need at least one argument for the filename");
    }

    const char* filename = argv[1];
    FILE* file = fopen(filename);
    
    char line[LINE_LEN + 1] = {0};
    int success = false;

    while (fgets(line, LINE_LEN, file) != NULL)
    {
        // if line longer than line length, flush rest of buffer
        if (strchr(line, '\n') == NULL) { flush_line(file); }
    }

    fclose(file);
    if (! success)
    {
        error("Failed to find __date__");
    }
}
