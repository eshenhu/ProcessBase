/*
 * $Id:$
 *
 * Source file for signal handling definitions.
 */

/** \file
 This file provides source code for signal handling functions.
*/

#include "fwkSignal.h"
#include "pthread.h"

#include <signal.h>

static int
_fwkSignalDisableSaRestart(
    int signalNum )
{
    int rc;
    struct sigaction sigactionStruct;
    rc = sigaction( signalNum,
                     0,
                     &sigactionStruct);
    if ( rc < 0 )
    {
        return rc;
    }

    sigactionStruct.sa_flags &= ~SA_RESTART;

    rc = sigaction( signalNum, &sigactionStruct, 0);

    return rc;
}

int
fwkSignalMaskThreadSignals(void)
{
    sigset_t signalSet;
    sigfillset(&signalSet);
    if (pthread_sigmask(SIG_BLOCK, &signalSet, 0) != 0)
    {
        return -1;
    }
    return 0;
}

int
fwkSignalUnmaskThreadSignals(void)
{
    sigset_t signalSet;
    sigfillset(&signalSet);
    if (pthread_sigmask(SIG_UNBLOCK, &signalSet, 0) != 0)
    {
        return -1;
    }
    return 0;
}

int
fwkSignalMaskThreadSignal(
    int sigId)
{
    int signum = fwkSignalGetSignum(sigId);
    if (signum < 0)
    {
        return -1;
    }

    sigset_t signalSet;
    sigemptyset(&signalSet);
    sigaddset(&signalSet, signum);
    if (pthread_sigmask(SIG_BLOCK, &signalSet, 0) != 0)
    {
        return -1;
    }
    return 0;
}

int
fwkSignalUnmaskThreadSignal(
    int sigId)
{
    int signum = fwkSignalGetSignum(sigId);
    if (signum < 0)
    {
        return -1;
    }

    sigset_t signalSet;
    sigemptyset(&signalSet);
    sigaddset(&signalSet, signum);
    if (pthread_sigmask(SIG_UNBLOCK, &signalSet, 0) != 0)
    {
        return -1;
    }
    return 0;
}

int
fwkSignalGetId(
    int signal)
{
    int id=-1;

    switch(signal)
    {
    case SIGUSR1:
        id=FWK_SIG_USR1;
        break;
    case SIGUSR2:
        id=FWK_SIG_USR2;
        break;
    case SIGWINCH:
        id=FWK_SIG_WINCH;
        break;
    case SIGINT:
        id=FWK_SIG_INT;
        break;
    case SIGTERM:
        id=FWK_SIG_TERM;
        break;
    case SIGHUP:
        id=FWK_SIG_HUP;
        break;
    default:
        // invalid signal
        break;
    }
    return id;
}


int
fwkSignalGetSignum(
    int sigId)
{
    int signum=-1;

    switch(sigId)
    {
    case FWK_SIG_USR1:
        signum=SIGUSR1;
        break;
    case FWK_SIG_USR2:
        signum=SIGUSR2;
        break;
    case FWK_SIG_WINCH:
        signum=SIGWINCH;
        break;
    case FWK_SIG_INT:
        signum=SIGINT;
        break;
    case FWK_SIG_TERM:
        signum=SIGTERM;
        break;
    case FWK_SIG_HUP:
        signum=SIGHUP;
        break;
    default:
        // invalid signal
        break;
    }
    return signum;
}