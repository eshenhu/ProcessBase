
#include "ProcessImpl.hpp"
#include "../Process.hpp"

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>
#include <boost/asio.hpp>
#include "fwkLogStream.hpp"
#include "fwkProcessUtils.h"
#include "fwkFile.h"

namespace asio = boost::asio;

namespace Process
{
   const FwkUint32_t THREAD_JOIN_DURATION_SECS = 5;
   // Parses the command-line arguments returns 0 on success, < 0 on error
   static int
   _processOptions(
      const boost::program_options::options_description& options,
      boost::program_options::variables_map& variablesMap,
      unsigned int flags,
      char* argv[],
      int argc,
      unsigned int& instanceNumber)
   {
      instanceNumber = Process::INVALID_PROCESS_INSTANCE_ID;

      boost::program_options::options_description generic("Generic Options");

      generic.add_options()
         ("help,h",
          "Display this help");

      if (!(flags & PROCESS_FLAG_SINGLE))
      {
         generic.add_options()
            ("instance-number",
             boost::program_options::value<int>()->default_value(
                0),
             "Instance number");
      }

      if (options.find_nothrow("default-log-level", false) == NULL)
      {
         generic.add_options()
            ("default-log-level",
             boost::program_options::value<std::string>()->default_value("WARNING"),
             "Default log level");
      }

      boost::program_options::options_description allOptions;
      allOptions.add(generic).add(options);

      try
      {
         boost::program_options::store(
            boost::program_options::command_line_parser(
               argc, argv).options(allOptions).run(), variablesMap);
      }
      catch (std::exception& e)
      {
         std::cerr << "Error: " << e.what() << std::endl;
         std::cerr << allOptions << std::endl;
         exit(0);
      }

      boost::program_options::notify(variablesMap);

      if (variablesMap.count("help"))
      {
         std::cout << allOptions << std::endl;
         exit(0);
      }

      if (!(flags & PROCESS_FLAG_SINGLE))
      {
         instanceNumber = variablesMap["instance-number"].as<int>();
      }

      return 0;
   }


// ProcessImpl
//
const std::size_t ProcessImpl::MAX_LEN_LOCK_FILE_PATH = 256;

ProcessImpl::ProcessImpl(
   Process& parent,
   int argc,
   char *argv[],
   FwkUint32_t flags,
   const boost::program_options::options_description& options) : parent_(parent),
   flags_(flags),
   isGood_(true),
   options_(options),
   variablesMap_(),
   instanceNum_(Process::INVALID_PROCESS_INSTANCE_ID),
   baseName_(reinterpret_cast<const char *>(basename(argv[0]))),
   basePlusInstanceName_(baseName_),
   io_service_(),
   pid_(fwkProcessGetPid()),
   mainPthreadId_(::pthread_self()),
   lockFile_("/var/lock/"),
   logStream_(),
   fdLock_(-1)
{
   char exePath[MAX_LEN_LOCK_FILE_PATH];

   if (getProcessExePath(pid_, exePath, sizeof(exePath) - 1) < 0)
   {
      LOG(WARNING) << "Failed to get process path location.";
      isGood_ = false;
      return;
   }

   lockFile_ += basename(exePath);

   if (flags & PROCESS_FLAG_CLOSE_ALL_DESC)
   {
      fwkProcessCloseDescriptors();
   }

   std::string parentQmsgName;
   if (_processOptions(options_,
                       variablesMap_,
                       flags,
                       argv,
                       argc,
                       instanceNum_) < 0)
   {
      isGood_ = false;
      return;
   }

   if (flags & PROCESS_FLAG_SINGLE)
   {
      instanceNum_ = Process::SINGLE_PROCESS_INSTANCE_ID;

      if (ProcessSingletonCheck() < 0)
      {
         isGood_ = false;
         return;
      }
   }

   makeBasePlusInstanceName();

   asio::signal_set signals(io_service_);
   signals.add(SIGUSR1);
   signals.add(SIGUSR2);
   signals.add(SIGINT);
   signals.add(SIGTERM);
   signals.add(SIGHUP);
   signals.add(SIGPIPE);
   if (flags_ & PROCESS_FLAG_CHILD_TERM) 
      signals.add(SIGCHLD);

   using namespace std::placeholders;
   signals.async_wait(std::bind(&ProcessImpl::processUnixSignal, this, _1, _2));

   FwkLogLevel dfltLevel(
      fwkLogLevelFromStr(
         variablesMap_["default-log-level"].as<std::string>().c_str()));

   if (dfltLevel != FWK_LOG_LEVEL_INVALID)
   {
      logStream_.setLogLevel(dfltLevel);
   }
}

ProcessImpl::~ProcessImpl()
{
   cleanupThreads();

   if (fdLock_ >= 0)
   {
      close(fdLock_);
      fdLock_ = -1;
      if (fwkUnlinkFile(lockFile_.c_str()) != 0)
      {
         LOG(WARNING) << "Failed to remove lock file.";
      }
   }
}

bool ProcessImpl::isGood() const
{
   return isGood_;
}

int ProcessImpl::ProcessSingletonCheck()
{
   if ((fdLock_ = getFileWriteLock(lockFile_.c_str())) < 0)
   {
      LOG(WARNING) << "Process not allowed to run -> Can't get lock!";
      std::cerr << "Process not allowed to run -> Can't get lock!" << std::endl;

      return -1;
   }

   return 0;
}

void ProcessImpl::exit()
{
   exit_ = true;
   if (::pthread_equal(pthread_self(), mainPthreadId_) == 0)
   {
      fwkProcessTerminate(pid_);
   }
}

bool ProcessImpl::exiting()
{
   return exit_; 
}

int ProcessImpl::system(
   const char *const cmdStr)
{
   FwkUint16_t sigIntQuit(false);
   int status = fwkProcessCall(
      cmdStr,
      &sigIntQuit);

   if (sigIntQuit)
   {
       parent_.quit();
   }
   return status;
}

FwkInt8_t
ProcessImpl::instanceNum() const
{
   return instanceNum_;
}

const std::string&
ProcessImpl::name() const
{
   return basePlusInstanceName_;
}

const std::string&
ProcessImpl::baseName() const
{
   return baseName_;
}

pthread_t
ProcessImpl::mainPthreadId() const
{
   return mainPthreadId_;
}

boost::asio::io_service&
ProcessImpl::get_io_service()
{
    return io_service_;
}

int ProcessImpl::run()
{
   LOG(INFO) << "Starting process " << basePlusInstanceName_;

   int retval(0);
   try {
      io_service_.run();       
   }
   catch(std::exception& e)
   {
       retval = -1;
       std::cerr << "Exception occurs during io_service with " 
                << e.what()
                << std::endl;
   }
   return retval;
}

const boost::program_options::variables_map&
ProcessImpl::programOptions() const
{
   return variablesMap_;
}

void ProcessImpl::processUnixSignal(
   const boost::system::error_code& code,
   int sig_num)
{
    LOG(INFO) 
      << "Receiving UNIX signal "
      << std::to_string(sig_num);

   if (!code.value())
   {  
      LOG(WARNING)
         << "error occurs when receiving UNIX "
         << std::to_string(sig_num)
         << "with error reason"
         << code.message();
      return;
   }
   

   switch (sig_num)
   {
   case SIGUSR1:
         parent_.dumpStats();
      break;

   case SIGUSR2:
         parent_.userDefined();
      break;

   case SIGHUP:
         parent_.reload();
      break;

   case SIGPIPE:
         processDoNothing(SIGPIPE);
      break;

   case SIGCHLD:
      {
         if (::pthread_equal(pthread_self(), Process::instance().pimpl_->mainPthreadId()) != 0)
         {
            parent_.childTerm();
         }
         else
         {
            pthread_kill(Process::instance().pimpl_->mainPthreadId(), sig_num);
         }
      }
      break;

   case SIGTERM:
      {
         if (::pthread_equal(pthread_self(), Process::instance().pimpl_->mainPthreadId()) != 0)
         {
            parent_.childTerm();
         }
         else
         {
            pthread_kill(Process::instance().pimpl_->mainPthreadId(), sig_num);
         }
      }
      break;

   case SIGINT:
      {
         if (::pthread_equal(pthread_self(), Process::instance().pimpl_->mainPthreadId()) != 0)
         {
            parent_.quit();
         }
         else
         {
            pthread_kill(Process::instance().pimpl_->mainPthreadId(), sig_num);
         }
      }
      break;
   default:
      break;
   }
}

void ProcessImpl::processConnClose(
   int sig)
{
}

void ProcessImpl::processDoNothing(
   int sig)
{
}


void ProcessImpl::addThreadForCleanup(
   pthread_t tid)
{
   std::lock_guard<std::recursive_mutex> guard(mutex_);
   if (std::find(threadsToCleanup_.begin(), threadsToCleanup_.end(), tid) == threadsToCleanup_.end())
   {
      threadsToCleanup_.push_back(tid);
      LOG(INFO)
            << "addThreadForCleanup: thread "
            << std::to_string(static_cast<FwkUint32_t>(tid));
   }
   else
   {
      LOG(WARNING)
            << "addThreadForCleanup: thread "
            << std::to_string(static_cast<FwkUint32_t>(tid))
            << " Already Exists.";
   }
}

void ProcessImpl::removeThreadForCleanup(
                                         pthread_t tid)
{
   std::lock_guard<std::recursive_mutex> guard(mutex_);
   std::vector<pthread_t>::iterator it = std::find(threadsToCleanup_.begin(), threadsToCleanup_.end(), tid);
   if (it != threadsToCleanup_.end())
   {
      LOG(INFO)
            << "removeThreadForCleanup: thread "
            << std::to_string(static_cast<FwkUint32_t>(tid));
      threadsToCleanup_.erase(it);
   }
   else
   {
      LOG(WARNING)
            << "removeThreadForCleanup: thread "
            << std::to_string(static_cast<FwkUint32_t>(tid))
            << " does not exist.";
   }
}

void ProcessImpl::cleanupThreads(
                                 void)
{
   LOG(DEBUG)
      << "cleanupThreads "
      << threadsToCleanup_.size();

   std::lock_guard<std::recursive_mutex> guard(mutex_);
   performThreadOperation("cancel", &pthread_cancel);
   performThreadOperation("join", &ProcessImpl::timedJoinWrapper);
   threadsToCleanup_.clear();
}

int ProcessImpl::timedJoinWrapper(
   pthread_t tid)
{
   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   ts.tv_sec += THREAD_JOIN_DURATION_SECS;
   return pthread_timedjoin_np(tid, NULL, &ts);
}

void ProcessImpl::performThreadOperation(
   const char *opName,
   int (*threadOp)(pthread_t))
{
   pthread_t self = pthread_self();
   if (!::pthread_equal(self, mainPthreadId_))
   {
      LOG(WARNING)
            << opName
            << ": should be called only from main thread, found tid = "
            << " mainthreadid = " << std::to_string(mainPthreadId_)
            << std::to_string(self);
      return;
   }
   for (std::vector<pthread_t>::iterator it = threadsToCleanup_.begin();
        it != threadsToCleanup_.end();
        ++it)
   {
      pthread_t tid = *it;
      if (::pthread_equal(self, tid))
      {
         LOG(WARNING)
            << opName
            << ": found self thread id in the list: "
            << std::to_string(static_cast<FwkUint32_t>(tid));
          continue;
      }
      int ret = (*threadOp)(tid);
      if (ret == 0)
      {
         LOG(INFO)
            << opName
            << ": success for tid: "
            << std::to_string(static_cast<FwkUint32_t>(tid));
      }
      //log the error only if the thread exist
      else if (ret != ESRCH)
      {
         LOG(WARNING)
            << opName
            << ": failed for tid: "
            << std::to_string(tid)
            << "with code: "
            << strerror(ret);
      }
      else
      {
         LOG(INFO)
            << opName
            << ": ESRCH in error return for tid: "
            << std::to_string(static_cast<FwkUint32_t>(tid));
      }
   }
}

// Creates a new name which contains basenaem(argv[0]), and
// perhaps, an instance number
void ProcessImpl::makeBasePlusInstanceName(void)
{
   if (instanceNum_ != Process::INVALID_PROCESS_INSTANCE_ID)
   {
      basePlusInstanceName_ += std::to_string(instanceNum_);
   }
}
} // namespace Process
