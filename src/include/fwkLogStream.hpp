/*
 * $Id:$
 *
 * Header file for the log stream class.
 *
 * g3log wrapper
 */

/** \file
 This file provides definitions and functions for the log stream class.
*/
#ifndef FWK_LOG_STREAM_HPP
#define FWK_LOG_STREAM_HPP

#include <memory>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/loglevels.hpp>
#include "fwkLog.h"

namespace Fwk{

class FwkLogStream{
    public:
       const LEVELS DEVEL {DEBUG.value   -1, "DEVEL"};
       const LEVELS ERROR {WARNING.value +1, "ERROR"};

    public:
        FwkLogStream(
            std::string log_path = path_to_log_file,
            std::string log_file = log_file);
        ~FwkLogStream();

        /**
         * Opens a directory for reading;
         *
         * @param logLevel - the logLevel wanted
         *
         * @return void
         *
         */
       void setLogLevel(enum FwkLogLevel);

    private:
        std::unique_ptr<g3::LogWorker> logWorker_;

	static const std::string path_to_log_file;
	static const std::string log_file;
};
}
#endif