// start time: 6:49
// end time  : 7:19
//
// start time: 10:40pm
// end time: 11:00pm
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
    static char newNameBuf[150];

    // - find the last '/' if there is one
    char* trailingDirSeparator = strrchr(fileName, '/');
    if (trailingDirSeparator == NULL)
    {
        // copy over the date-creation string
        strncpy(newNameBuf, timeCreatedStr, timeCreatedStrLen);

        // add the underscore
        newNameBuf[timeCreatedStrLen] = '_';

        // add the rest of the filename
        strncpy(newNameBuf + timeCreatedStrLen + 1, fileName, strlen(fileName));
        
    }
    else
    {
        // - copy upto and including that / into the buffer
        int leadingDirsLen = trailingDirSeparator - fileName + 1;
        strncpy(newNameBuf, fileName, leadingDirsLen);
        // - copy the date
        strncpy(newNameBuf + leadingDirsLen, timeCreatedStr, timeCreatedStrLen); 
        newNameBuf[leadingDirsLen + timeCreatedStrLen] = '_';
        // - copy the rest of the filename
        strncpy(newNameBuf + leadingDirsLen + timeCreatedStrLen + 1, fileName + leadingDirsLen, strlen(fileName) - leadingDirsLen);
    }

    int renameRes = rename(fileName, newNameBuf);
    if (renameRes == -1)
    {
        error("Rename failed");
    }

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
    RenameFiles("1/file1.txt", (const struct stat*) NULL, 0);
    RenameFiles("1/2/file2.txt", (const struct stat*) NULL, 0);
    RenameFiles("1/2/3/file3.txt", (const struct stat*) NULL, 0);
    RenameFiles("1/2/3/4/file4.txt", (const struct stat*) NULL, 0);
}