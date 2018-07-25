/*
 *
 * $Id:$
 *
 * Header file for platform dependent file functions.
 *
 */

/** \file
 This file provides definitions for platform dependent file functions.
 */


#ifndef FWK_FILE_H
#define FWK_FILE_H
#include "fwkTypes.h"
#include "fwkFileDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
 * Returns the file size;
 *
 * @param fileName - the file name
 * @param size - a pointer to the variable to hold the result
 *
 * @return Returns 0 on success and -1 on failure
 *
 */
int
fwkFileGetSize(
    const char* fileName,
    FwkUint32_t* size);


/**
 * Opens a directory for reading;
 *
 * @param dirName - the directory name
 *
 * @return Returns a FwkDirHandle* on success and NULL on failure
 *
 */
FwkDirHandle*
fwkFileOpenDir(
    const char* dirName);


/**
 * Closes a previously opened directory;
 *
 * @param dirHandle - the directory handle returned from fwkFileOpenDir
 *
 * @return Returns 0 on success, -1 on failure
 *
 */
int
fwkFileCloseDir(
    FwkDirHandle* dirHandle);


/**
 * Returns the next file name in the specified directory;
 *
 * @param dirHandle - the directory handle returned from fwkFileOpenDir
 * @param fileNameMax - the size of fileName
 * @param fileName - a pointer to memory to hold the output file name
 *
 * @return Returns 0 on success, -1 on failure, and 1 on end-of-file
 *         (no filenames left)
 *
 */
int
fwkFileReadDir(
    FwkDirHandle* dirHandle,
    int fileNameMax,
    char* fileName);

/**
 * Reads one less than size characters from the file handle.
 * Reading stops on encountering an EOF or newline.
 *
 * @param s - the buffer to store the characters read.
 * @param size - the number of characters to read from stream.
 * @param handle - file handle (pointer to stream)
 *
 * @return Returns the pointer to buffer for success and NULL for failure
 *
 */
char*
fwkFileReadHandle(
    char* s,
    int size,
    FwkFileHandle handle);

/**
 * Returns the current working directory.
 *
 * @param buf - the buffer to store the returned directory
 * @param size - the size of the buffer
 *
 * @return Returns the pointer to buffer for success and NULL for failure
 *
 */
char*
fwkFileGetCwd(
    char* buf,
    int size);


/**
 * Checks if a file exists.
 *
 * @param filepath - the full pathname of the file
 *
 * @return Returns 1 if it exists, 0 otherwise.
 *
 */
int
fwkFileExists(
    const char* filepath);


/**
 * Makes a symlink to a file
 *
 * @param source - the full pathname of the source file
 * @param dest - the full pathname of the dest file
 *
 * @return Returns 1 if it exists, 0 otherwise.
 *
 */
int
fwkSymlinkFile(
    const char* source,
    const char* dest);


/**
 * Remove a file.
 *
 * @param filepath - the full pathname of the file to remove
 *
 * @return Returns 0 on success, -1 on failure.
 *
 */
int
fwkUnlinkFile(
    const char* filepath);

/**
 * get a file write lock.
 *
 * @param filepath - the full pathname of the file
 *
 * @return Returns 0 on success, -1 on failure.
 *
 */
int
getFileWriteLock(
    const char* filepath);

#ifdef __cplusplus
}
#endif

#endif /* FWK_FILE_H */
