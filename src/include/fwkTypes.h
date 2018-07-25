/*
 * $Id:$
 *
 * Header file for standard type definitions for Linux.
 *
 */

/** \file
 This file provides standard type definitions for Linux.
 */

#ifndef FWK_TYPES_H
#define FWK_TYPES_H

#ifdef __KERNEL__
# include <linux/types.h>
#else
# define __STDC_LIMIT_MACROS
# include <stdint.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef uint8_t FwkUint8_t;     /**< An unsigned 8-bit type */
typedef int8_t FwkInt8_t;       /**< A signed 8-bit type */
typedef uint16_t FwkUint16_t;   /**< An unsigned 16-bit type */
typedef int16_t FwkInt16_t;     /**< A signed 16-bit type */
typedef uint32_t FwkUint32_t;   /**< An unsigned 32-bit type */
typedef int32_t FwkInt32_t;     /**< A signed 32-bit type */
typedef uint64_t FwkUint64_t;   /**< An unsigned 64-bit type */
typedef int64_t FwkInt64_t;     /**< A signed 64-bit type */

/* Definitions for integer extents are provided by stdint.h. */

typedef enum FwkBoolean
{
    FWK_FALSE = 0,
    FWK_TRUE = 1
} FwkBoolean_t;

#ifdef __cplusplus
}
#endif

#endif /* FWK_TYPES_H */