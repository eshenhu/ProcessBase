/*
 * $Id:$
 *
 * Header file for process creation and monitoring definitions.
 *
 */

/** \file
 This file provides definitions and functions for signal handling.
*/

#ifndef FWK_PROCESS_UTILS_H
#define FWK_PROCESS_UTILS_H

#include <sys/types.h>
#include "fwkTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define FWK_WAIT_ERROR     0xffffffff
#define FWK_WAIT_NO_CHILD  0xfffffffe
#define FWK_WAIT_ABNORMAL  0xfffffffd

#define FWK_SID_TRUE  1
#define FWK_SID_FALSE 0

/**
 * Runs the application specified without spawning a new process.
 *
 * @param appName - the application name (full path)
 * @param sigIntQuitPtr - Filled in with true if process got SIGINT or SIGQUIT.
 *
 * @return Returns the exit code of the application
 */
int
fwkProcessCall(
    const char* appName,
    FwkUint16_t* sigIntQuitPtr
                );

/**
 * Starts a new instance of the process specified.
 *
 * @param appName - the application name (full path)
 * @param av - the command-line arguments to the process
 * @param priority - the application priority (lower is greather priority)
 * @param setSid - call setsid() in the child if set
 *
 * @return Returns the process ID on success and -1 on failure
 */
int
fwkProcessStart(
    const char* const appName,
    const char* av[],
    FwkUint32_t priority,
    int setSid);

/**
 * Sends a terminate signal to the process.  If this does not work, then the
 * process may need to be killed (see fwkPrcessKill).
 *
 * @param processId - the process ID to terminate
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkProcessTerminate(
    int processId);

/**
 * Sends a specified signal to a specific process.
 *
 * @param processId - the process ID to send signal to.
 * @param signal - the signal to be sent.
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkSignalProcess(
    int processId,
    int signal);

/**
 * Sends a kill signal to the process.  This cannot be ignored.
 *
 * @param processId - the process ID to terminate
 *
 * @return Returns 0 on success and -1 on failure
 */
int
fwkProcessKill(
    int processId);

/**
 * Waits for the specified process to terminate.  If the specified process ID
 * is -1, then it will wait for any process to terminate.
 *
 * @param processId - the process ID to wait for or -1 to wait for any process
 * @param block - if non-zero then the call will block until a child exits
 *
 * @return Returns the process exit code on success or:
 * \li FWK_WAIT_NO_CHILD - no child process exited
 * \li FWK_WAIT_ERROR - unable to get child termination info
 * \li FWK_WAIT_ABNORMAL - the child process exited abnormally
 */
int
fwkProcessWait(
    int* processId,
    int block);

/**
 * Returns the current process's process ID.
 *
 * @return Returns the process ID on success and -1 on failure
 */
int
fwkProcessGetPid(void);

/**
 * Returns the current thread's thread ID.  For a single-threaded process, this
 * always returns the process ID.
 *
 * @return Returns the thread ID on success and -1 on failure
 */
int
fwkProcessGetThreadId(void);

/**
 * Close all descriptors except stdin, stdout and stderr
 *
 * Typically used on child process start to close
 * all inherited descriptors
 */
void
fwkProcessCloseDescriptors(void);

/**
 * Given pid, get process's exe path
 *
 * @param pid - the process ID
 * @param path - filled with the path information by this function
 * @param maxLen - the max size of path's memory
 *
 * @return Returns 0 on success, -1 on failure.
 */
ssize_t
getProcessExePath(
    int pid,
    char *path,
    size_t maxLen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FWK_PROCESS_UTILS_H */
