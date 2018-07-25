/*
 *
 * $Id:$
 *
 * Header file for platform dependent file definitions.
 *
 */

/** \file
 This file provides definitions for platform dependent file definitions.
 */


#ifndef FWK_FILE_DEFS_H
#define FWK_FILE_DEFS_H

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


typedef DIR FwkDirHandle;
typedef FILE* FwkFileHandle;

#ifdef __cplusplus
}
#endif

#endif /* FWK_FILE_DEFS_H */
