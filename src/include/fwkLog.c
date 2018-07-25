/*
 * $Id:$
 *
 * Source file for the os-specific logging things
 *
 */
#include "fwkLog.h"
#include <string.h>
#include <unistd.h>

enum FwkLogLevel
fwkLogLevelFromStr(
    const char *inputString
    ) {
    if ( strcmp(inputString, "ERROR") == 0 ) {
	return FWK_LOG_LEVEL_ERROR;
    }
    if ( strcmp(inputString, "WARNING") == 0 ) {
	return FWK_LOG_LEVEL_WARNING;
    }
    if ( strcmp(inputString, "INFO") == 0 ) {
	return FWK_LOG_LEVEL_INFO;
    }
    if ( strcmp(inputString, "DEBUG") == 0 ) {
	return FWK_LOG_LEVEL_DEBUG;
    }
    if ( strcmp(inputString, "DEVEL") == 0 ) {
	return FWK_LOG_LEVEL_DEVEL;
    }
    return FWK_LOG_LEVEL_INVALID;
}

