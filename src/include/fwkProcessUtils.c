/*
 * $Id:$
 *
 * Source file for process definitions.
 *
 */

/** \file
 This file provides source code for process functions.
*/

#include "fwkProcessUtils.h"
#include "fwkSignal.h"
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
extern char **environ;

int
fwkProcessCall(
    const char* appName,
    FwkUint16_t* sigIntQuitPtr )
{
    if ( sigIntQuitPtr == 0 )
    {
        return -1;
    }

    int ret = system(appName);

    if ( WIFSIGNALED(ret) &&
         (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
    {
        /* Let caller know appName was interrupted */
        *sigIntQuitPtr = 1;
    }
    else
    {
        *sigIntQuitPtr = 0;
    }

    if (ret < 0)
    {
        return -1;
    }

    int status = WEXITSTATUS(ret);
    if (status == 127)
    {
        return -1;
    }

    return status;
}

int
fwkProcessStart(
    const char* appName,
    const char* av[],
    FwkUint32_t priority,
    int setSid)
{
    int pid;
    int status;
    struct stat statPtr;

    status = stat(appName, &statPtr);
    if ( status != 0 )
    {
        return -1;
    }

    pid = vfork();
    if (pid == 0)
    {
        int i;

        /* this sets a new group id and detaches from the parent process */
        if (setSid == FWK_SID_TRUE)
        {
            setsid();
        }

        /* close all open file descriptors except for stdin/stdout/stderr */
        for (i=getdtablesize(); i > 2; --i)
        {
            close(i);
        }

        /* run the desired application */
        if (execve(appName, (char* const *)av, environ) < 0)
        {
            _exit(-1);
        }
    }
    else if (pid < 0)
    {
        /* this is an error */
        return -1;
    }

    /* set the priority */
    setpriority(PRIO_PROCESS, pid, priority);

    return pid;
}

int
fwkProcessTerminate(
    int processId)
{
    return kill(processId, SIGTERM);
}

int
fwkProcessKill(
    int processId)
{
    return kill(processId, SIGKILL);
}

int
fwkProcessWait(
    int* processId,
    int block)
{
    int status;
    int ret;

    if (processId == NULL)
    {
        return FWK_WAIT_ERROR;
    }

    ret = waitpid(*processId, &status, (block ? 0 : WNOHANG));
    if (ret == 0)
    {
        /* no child exited */
        return FWK_WAIT_NO_CHILD;
    }
    else if (ret < 0)
    {
        if (errno == EINVAL)
        {
            /* this is an error */
            return FWK_WAIT_ERROR;
        }
        else
        {
            return FWK_WAIT_NO_CHILD;
        }
    }

    *processId = ret;
    if (WIFEXITED(status))
    {
        int exitCode = WEXITSTATUS(status);
        return exitCode;
    }

    return FWK_WAIT_ABNORMAL;
}

int
fwkProcessGetPid(void)
{
    return getpid();
}

int
fwkProcessGetThreadId(void)
{
    return syscall(SYS_gettid);
}

int
fwkSignalProcess(
    int processId,
    int signal)
{
    int id;

    switch(signal)
    {
    case FWK_SIG_USR1:
        id=SIGUSR1;
        break;
    case FWK_SIG_INT:
        id=SIGINT;
        break;
    case FWK_SIG_TERM:
        id=SIGTERM;
        break;
    case FWK_SIG_HUP:
        id=SIGHUP;
        break;
    default:
        // invalid signal
        return -1;
    }

    return kill(processId, id);

}

void
fwkProcessCloseDescriptors(void)
{
    int i;

    for (i = 3; i < getdtablesize(); i++)
    {
        close(i);
    }
}

ssize_t
getProcessExePath(
    int pid,
    char *path,
    size_t maxLen)
{
    if(path == NULL)
       return -1;

    const int bufSize = 32;
    char buf[bufSize];
    snprintf(buf, bufSize, "/proc/%d/exe", pid);
    ssize_t len = readlink(buf, path, maxLen-1);
    if(len < 0)
       return -1;

    path[len] = '\0';
    return len;
}
