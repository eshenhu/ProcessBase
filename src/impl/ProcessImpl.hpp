/*
 * $Id:$
 *
 * Header file for the processimpl class.
 *
 */
/** \file
 This file provides definitions and functions for the processimpl class.
*/

#ifndef PROCESSIMPL_HPP
#define PROCESSIMPL_HPP

#include <string>
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "fwkLogStream.hpp"

namespace Process {
class Process;

class ProcessImpl {
public:
    ProcessImpl(
	Process &parent,
	int argc,
	char *argv[],
	FwkUint32_t flags,
	const boost::program_options::options_description &options);

    ~ProcessImpl();

    bool
    isGood() const;

    void
    exit();

    bool
    exiting();

    int
    system(
	const char *const cmdStr
	);

    FwkInt8_t
    instanceNum() const;

    const std::string&
    name() const;

    const std::string&
    baseName() const;

    int
    run();

    const boost::program_options::variables_map&
    programOptions() const;

    /// Add threads that will be cleanedup (cancel/join) upon process exit
    void
    addThreadForCleanup(
	pthread_t tid);

    /// Remove thread from the cleanup list
    void
    removeThreadForCleanup(
	pthread_t tid);

private:
    void 
    processConnClose(
       int sig);

    void
    processDoNothing(
	int sig);

    int
    ProcessSingletonCheck(
	void);

    void
    makeBasePlusInstanceName(void);
    
    static int timedJoinWrapper(
	pthread_t tid);

    void
    performThreadOperation(
	const char *opName,
	int (*threadOp)(pthread_t));

    void
    cleanupThreads(
	void);

    pthread_t
    mainPthreadId(void) const;

    boost::asio::io_service&
    get_io_service(void);

    void processUnixSignal(
	const boost::system::error_code &code,
	int sig_num);



    Process &parent_;
    FwkUint32_t flags_;
    bool isGood_;
    bool exit_ = false;
    boost::program_options::options_description options_;
    boost::program_options::variables_map variablesMap_;
    FwkUint32_t instanceNum_;
    std::string baseName_;
    std::string basePlusInstanceName_;
    boost::asio::io_service io_service_;
    int pid_;
    pthread_t mainPthreadId_;
    std::string lockFile_;
    Fwk::FwkLogStream logStream_;
    int fdLock_;
    std::vector<pthread_t> threadsToCleanup_;
    mutable std::recursive_mutex mutex_;
    static const size_t MAX_LEN_LOCK_FILE_PATH;
};
}
#endif
