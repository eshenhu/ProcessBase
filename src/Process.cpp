/*
 * $Id:$
 *
 * Source file for processes.
 *
 */

#include "Process.hpp"
#include "fwkTypes.h"
#include "fwkSignal.h"
#include "fwkProcessUtils.h"
#include "fwkFile.h"
#include "impl/ProcessImpl.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <boost/program_options.hpp>

using namespace std;

namespace Process
{
   const FwkUint16_t Process::INVALID_PROCESS_INSTANCE_ID = 0xff;
   const FwkUint16_t Process::SINGLE_PROCESS_INSTANCE_ID = 0;

   //
   // Process
   //
   Process* Process::proc_;

   Process::Process(
      int argc,
      char* argv[],
      enum ProcessFlags flags,
      const boost::program_options::options_description& options) :
      isGood_(false),
      pimpl_(new ProcessImpl(
                *this,
                argc,
                argv,
                flags,
                options))
   {
      if (!pimpl_->isGood())
      {
         return;
      }

      isGood_ = true;

      proc_ = this;
   }

   Process::~Process()
   {
      // Mask out all signals for this main thread ONLY!
      fwkSignalMaskThreadSignals();
   }

   bool
   Process::isGood() const
   {
      return isGood_;
   }

   void
   Process::exit()
   {
      pimpl_->exit();
   }

   bool
   Process::exiting()
   {
      return pimpl_->exiting();
   }

   int
   Process::system(
      const char* const cmdStr
                   )
   {
      return pimpl_->system( cmdStr );
   }


   void
   Process::halt()
   {
      exit();
   }

   void
   Process::quit()
   {
      halt();
   }

   void
   Process::userDefined()
   {
   }

   void
   Process::dumpStats()
   {
   }

   void
   Process::reload()
   {
   }

   void
   Process::childTerm()
   {
   }

   unsigned int
   Process::instanceNum() const
   {
      return pimpl_->instanceNum();
   }

   const std::string&
   Process::name() const
   {
      return pimpl_->name();
   }

   const std::string&
   Process::baseName() const
   {
      return pimpl_->baseName();
   }

   int
   Process::run()
   {
      return pimpl_->run();
   }

   Process&
   Process::instance()
   {
      return *proc_; 
   }

   int
   Process::pid() const
   {
      return fwkProcessGetPid();
   }

   const boost::program_options::variables_map&
   Process::programOptions() const
   {
      return pimpl_->programOptions();
   }

   void Process::addThreadForCleanup(
       pthread_t tid)
   {
       return pimpl_->addThreadForCleanup(tid);
   }

   void Process::removeThreadForCleanup(
       pthread_t tid)
   {
       return pimpl_->removeThreadForCleanup(tid);
   }
}

//C helper functions
void
register_thread_with_infra(
    pthread_t tid)
{
    Process::Process::instance().addThreadForCleanup(tid);
}

void
unregister_thread_with_infra(
    pthread_t tid)
{
    Process::Process::instance().removeThreadForCleanup(tid);
}

void
exit_process(
    void)
{
    Process::Process::instance().exit();
}
