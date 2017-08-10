// start time: 6:49
// end time  : 7:19
//
// start time: 10:40pm
// end time: 11:00pm
//
// start time: 6:45
// end time: 7:05
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
#include <time.h>

void error(
    const char* error_str
    )
{
    perror(error_str);
    exit(1);
}

int RenameFiles(
    const char* fileName,
    const struct stat* fileStat,
    int fileType
    )
{
    if (fileType == FTW_D)
    {
        // don't do anything for directories
        return 0;
    }

    // convert to string
    //
    //
    // TODO: cleanup. get rid of any unnecessary junk
    //
    //
    int timeCreatedStrLen = 10; // %F = %Y-%m-%d = 10 chars
    char timeCreatedStr[150];
    struct tm* _tm = localtime(&fileStat->st_ctime);
    int time_ret = strftime(timeCreatedStr, 150, "%F", _tm);
    char lastChar = timeCreatedStr[timeCreatedStrLen - 1];

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
    *(newNameBuf + timeCreatedStrLen + 1 + strlen(fileName)) = '\0';        

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
    int res = ftw(dirName, RenameFiles, 10);
    if (res != 0)
    {
        error("dir walk failed");
    }
}
