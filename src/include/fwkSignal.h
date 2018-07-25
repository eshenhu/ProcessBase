/*
 * $Id:$
 *
 * Header file for signal handling definitions.
 */

/** \file
 This file provides definitions and functions for signal handling.
*/

#ifndef FWK_SIGNAL_H
#define FWK_SIGNAL_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * An enumeration for common unix signals.
 */
enum FwkSigVal
{
    FWK_SIG_FIRST =0,
    FWK_SIG_USR1  =1,
    FWK_SIG_INT   =2,
    FWK_SIG_TERM  =3,
    FWK_SIG_HUP   =4,
    FWK_SIG_USR2  =5,
    FWK_SIG_WINCH =6,
    FWK_SIG_LAST
};

/**
 * The signal handler type
 */
typedef void (*FwkSignalHandler_t)(int);

/**
 * The signal handler error code
 */
#define FWK_SIG_ERR ((FwkSignalHandler_t) -1)

/**
 * Masks all signals for a thread
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkSignalMaskThreadSignals(void);

/**
 * Unmasks all signals for a thread
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkSignalUnmaskThreadSignals(void);

/**
 * Masks a single for a thread
 *
 * @param sigId - The library defined signal id.
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkSignalMaskThreadSignal(
    int sigId);

/**
 * Unmasks a single signal for a thread
 *
 * @param sigId - The library defined signal id.
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkSignalUnmaskThreadSignal(
    int sigId);

/**
 * Gets the library defined signal id from actual signal number
 *
 * @param sig - The actual unix signal number.
 *
 * @return Returns the id on success and -1 on failure
 */
int
fwkSignalGetId(
    int sig);

/**
 * Gets the actual signal number from the library defined signal id.
 *
 * @param sigId - the id.
 *
 * @return Returns the actual unix signal number on success and -1 on failure
 */
int
fwkSignalGetSignum(
    int sigId);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* FWK_SIGNAL_H */
