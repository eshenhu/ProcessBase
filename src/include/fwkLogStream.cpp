#include "fwkLogStream.hpp"

namespace Fwk
{
const std::string FwkLogStream::path_to_log_file = "/tmp";
const std::string FwkLogStream::log_file("log_to_file.log");

FwkLogStream::FwkLogStream(
    std::string log_path,
    std::string log_file)
{
    //g3::addLogLevel(DEVEL, true);
    //g3::addLogLevel(ERROR, true);

    std::unique_ptr<g3::LogWorker> logWorker_ = g3::LogWorker::createLogWorker();
    auto handle = logWorker_->addDefaultLogger(log_file, log_path);
    //auto sink =
    //    logWorker_->addSink(std::make_unique<LogRotate>(), &LogRotate::save);

    g3::initializeLogging(logWorker_.get());
}

FwkLogStream::~FwkLogStream()
{
    g3::internal::shutDownLogging();
}

void FwkLogStream::setLogLevel(enum FwkLogLevel value)
{
    struct LEVELS level(WARNING);

    switch (value)
    {
        case FWK_LOG_LEVEL_ERROR:
            level = ERROR;
            break;
        case FWK_LOG_LEVEL_WARNING:
            level = WARNING;
            break;
        case FWK_LOG_LEVEL_INFO:
            level = INFO;
            break;
        case FWK_LOG_LEVEL_DEBUG:
            level = DEBUG;
            break;
        case FWK_LOG_LEVEL_DEVEL:
            level = DEVEL;
            break;
        default:
            break;
    }

    //g3::setHighest(level);
}
}

