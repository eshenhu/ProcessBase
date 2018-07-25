/*
 * $Id:$
 *
 * Header file for the os-specific logging things
 *
 */

#ifndef FWK_LOG_H
#define FWK_LOG_H

/*
 * Size of the char array used to construct a log message.
 * This value is based on the limit being used by syslog.
 */
#define MAX_LOG_BUFFER_SIZE 2048

#include "fwkTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*  The Fwk log level values. */
    enum FwkLogLevel
    {
        FWK_LOG_LEVEL_INVALID  = 0,
        FWK_LOG_LEVEL_ERROR    = 1, /**< Error log message */
        FWK_LOG_LEVEL_WARNING  = 2, /**< Warning log message */
        FWK_LOG_LEVEL_INFO     = 3, /**< Informational log message */
        FWK_LOG_LEVEL_DEBUG    = 4, /**< Debug log message */
        FWK_LOG_LEVEL_DEVEL    = 5, /**< Developer log message */
        FWK_LOG_LEVEL_LAST
    };

    /**
     * Converts a printable ASCII string to a log level
     @ return log level, FWK_LOG_LEVEL_INVALID if invalid string
    */
    enum FwkLogLevel
    fwkLogLevelFromStr(
        const char* inputString
    );

#ifdef __cplusplus
}
#endif

#endif /* FWK_LOG_H */
