/*
 * $Id:$
 *
 * Source file for file operations.
 *
 */

#include "fwkFile.h"
#include "sys/stat.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
int
fwkFileGetSize(
    const char* fileName,
    FwkUint32_t* size)
{
    struct stat sb;
    if (stat (fileName, &sb) < 0)
    {
        if (size != 0)
        {
            *size = 0;
        }
        return -1;
    }

    if (size != 0)
    {
        *size = sb.st_size;
    }
    return 0;
}


FwkDirHandle*
fwkFileOpenDir(
    const char* dirName)
{
    return opendir(dirName);
}


int
fwkFileCloseDir(
    FwkDirHandle* dirHandle)
{
    return closedir(dirHandle);
}


int
fwkFileReadDir(
    FwkDirHandle* dirHandle,
    int fileNameMax,
    char* fileName)
{
    struct dirent entry;
    struct dirent* dirp;

    if (readdir_r(
            dirHandle,
            &entry,
            &dirp) < 0)
    {
        return -1;
    }

    if (dirp == 0)
    {
        // end-of-file
        return 1;
    }
    else
    {
        strncpy(fileName, entry.d_name, fileNameMax);
	fileName[fileNameMax - 1] = '\0'; 
    }

    return 0;
}

char*
fwkFileReadHandle(
    char* s,
    int size,
    FwkFileHandle handle)
{
    return(fgets(s, size, handle));
}

char*
fwkFileGetCwd(
    char* buf,
    int size)
{
    if (!buf)
    {
        return NULL;
    }

    return getcwd(buf, size);
}

int
fwkFileExists(
    const char* filepath)
{
    struct stat statbuf;
    return (stat(filepath, &statbuf) == 0);
}

int
fwkSymlinkFile(
    const char* source,
    const char* dest)
{
    return symlink(source, dest);
}

int
fwkUnlinkFile(
    const char* filepath)
{
    return unlink(filepath);
}

int
getFileWriteLock(
    const char* filepath)
{
    if(filepath == NULL)
        return -1;

    int fdLock = -1;
    if((fdLock = open(filepath, O_RDWR|O_CREAT|O_EXCL, 0666)) == -1)
        return -1;

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 1;
    if(fcntl(fdLock, F_SETLK, &fl) == -1)
    {
        close(fdLock);
        return -1;
    }

    return fdLock;
}

