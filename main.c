// start time: 6:49
// end time  : 7:19
//
//
// plan
// * read directory name from cmdargs
// * read all files from current directory
// * append date-created to all file names in dir

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>

void error(
    const char* error_str
    )
{
    printf("%s\n", error_str);
    exit(1);
}

int RenameFiles(
    const char* fileName,
    const struct stat* fileStat,
    int fileType
    )
{
    // read the time created
    int timeCreated = 0;

    // convert to string
    const char* timeCreatedStr = "0";
    int timeCreatedStrLen = strlen(timeCreatedStr);

    // append to beginning of filename
    static char fileNameBuffer[150];

    // copy over the creation string
    strncpy(fileNameBuffer, timeCreatedStr, timeCreatedStrLen);

    // add the underscore
    fileNameBuffer[timeCreatedStrLen] = '_';

    // add the rest of the filename
    strncpy(fileNameBuffer + timeCreatedStrLen + 1, fileName, strlen(fileName));
    
    printf("New file: %s\n", fileNameBuffer);

    return 0;
}

int main(
    int argc,
    char** argv
    )
{
    if (argc < 2)
    {
        error("Need an argument for the directory to use: ./main <dir>");
    }

    const char* dirName = argv[1];
    RenameFiles("file1.txt", (const struct stat*) NULL, 0);
    RenameFiles("file2.txt", (const struct stat*) NULL, 0);
    RenameFiles("file3.txt", (const struct stat*) NULL, 0);
    RenameFiles("file4.txt", (const struct stat*) NULL, 0);
}
