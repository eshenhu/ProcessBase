/*
 * $Id:$
 *
 * Header file for the process class.
 *
 */
/** \file
 This file provides definitions and functions for the process class.
*/

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <list>
#include <map>
#include <pthread.h>
#include "fwkTypes.h"
#include "boost/program_options.hpp"

namespace Process {
enum ProcessFlags {
    PROCESS_FLAG_NONE,
    PROCESS_FLAG_SINGLE         = 0x01, ///< Only one instance allowed.
    PROCESS_FLAG_CLOSE_ALL_DESC = 0x02,
    PROCESS_FLAG_CHILD_TERM     = 0x04,
    PROCESS_FLAG_LAST
};

/// The implementation class for Process.
class ProcessImpl;

/**
 * @brief 
 *  The process class is base class of all applications.
 *  It was expected to derive the subclass based on this process.
 *  
 *  Any number of the argv will pass through to the Process class.
 */
class Process {
public:
    /**
     *  Process destructor.
     */
    virtual ~Process();

    /**
     * @brief Entry point for starting the process, mainly it was delegated
     * to the io_services.
     * @return int Returns the process exit code (zero for success)
     */
    int
    run();

    /**
     * @brief  Describe success or not during the constructor.
     * @return Returns true if the process construction was successful
     */
    bool
    isGood() const;

    /**
     * @brief 
     * 
     */
    void
    exit();

    /// True if we're on the way down.
    bool
    exiting();

    /// Used to provide common system call handling in regards to SIGINT
    /// @return Returns exit status of cmdStr
    int
    system(
	const char *const cmdStr ///< command to execute
	);

    /**
     * @brief 
     * term signal handler. Devived class should override this function
     * if needed.
     */
    virtual void
    halt();

    /**
     * @brief 
     * int signal handler. Devived class should override this function
     * if needed.
     */
    virtual void
    quit();

    /**
     * @brief 
     * sigusr2 signal handler. Devived class should override this function
     * if needed.
     */
    virtual void
    userDefined();

    /**
     * @brief 
     * dump signal handler. Devived class should override this function
     * if needed.
     */
    virtual void
    dumpStats();

    /**
     * @brief 
     * reload signal handler. Devived class should override this function
     * if needed.
     */
    virtual void
    reload();

    /// This method is called when a child termination signal is received
    /// by the process.  Derived processes can override this to provide
    /// this functionality.  The default method base class method does
    /// nothing.
    virtual void
    childTerm();

    /// Returns the instance number of the process.  This is used to
    /// distinguish between multiple instance of the same process.
    /// @return Returns the instance number of the process.
    unsigned int
    instanceNum() const;

    /// Returns the name of the process.
    /// @return Returns the name of the process
    const std::string&
    name() const;

    /// Returns the name of the process without instance number.
    /// @return Returns the name of the process without instance number
    const std::string&
    baseName() const;

    /// Returns the Process singleton instance.
    /// @return Returns the Process singleton instance
    static Process&
    instance();

    /// Returns the process (or thread ID).
    ///
    /// @return Returns the process or thread ID.
    int
    pid() const;

    /// Provides access to the set of parsed command line options.
    ///
    /// @return Returns a static reference to the variables map.  See
    ///         boost::program_options for details on how to extract
    ///         individual flags.
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
protected:
    /**
     * @brief Process constructor 
     * 
     * @param argc - the number of arguments passed to the process
     * @param argv - the array of string arguments passed to the process
     * @param flags - specifies the flags for the process.  Multiple
     *                values can be OR-ed together.
     * @param options - command line options
     */
    Process(
	int argc,
	char *argv[],
	enum ProcessFlags flags = PROCESS_FLAG_SINGLE,
	const boost::program_options::options_description &options =
	boost::program_options::options_description()
	);

    Process(const Process&) = delete;

    bool isGood_;

private:
    friend class ProcessImpl;
    std::unique_ptr<ProcessImpl> pimpl_; ///< The implementation
					 /// instance.
    static Process *proc_;

public:
    static const FwkUint16_t INVALID_PROCESS_INSTANCE_ID;
    static const FwkUint16_t SINGLE_PROCESS_INSTANCE_ID;
    static const FwkUint32_t THREAD_JOIN_DURATION_SECS;

};
}

#endif /* PROCESS_HPP */
