// Local
#include "CrashHandler.h"
#include "StackTrace.h"

// STL
#include <iostream>
#include <csignal>
#include <sstream>
#include <cstdlib>
#include <cstring>

// SDK
#include <CLoggerHandler/CLoggerHandler.h>


std::string crashReason(int sig)
{
  std::stringstream ss;
  bool foundReason = false;
  for (int i = 0; i < crashSignalsCount; ++i)
  {
    if (crashSignals[i].numb == sig)
    {
      ss << "Application has crashed due to [" << crashSignals[i].name << "] signal" << std::endl
         << "    " << crashSignals[i].brief << std::endl
         << "    " << crashSignals[i].detail;
      foundReason = true;
    }
  }
  if (!foundReason)
    ss << "Application has crashed due to unknown signal [" << sig << "]";
  return ss.str();
}


void logCrashReason(int sig, bool stackTraceIfAvailable)
{
  std::stringstream ss;
  ss << "CRASH HANDLED; ";
  ss << crashReason(sig);
  if (stackTraceIfAvailable)
    ss << std::endl << "    ======= Backtrace: =========" << std::endl << StackTrace();

  PRINT_CONSOLE_MESSAGE_ERROR(ss.str().c_str());
}


void crashAbort(int sig, const char* sourceFile = "", unsigned int long line = 0)
{
  std::stringstream ss;
  ss << crashReason(sig).c_str();
  ss << " - [Called el::Helpers::crashAbort(" << sig << ")]";
  if (sourceFile != NULL && strlen(sourceFile) > 0)
  {
    ss << " - Source: " << sourceFile;
    if (line > 0)
      ss << ":" << line;
    else
      ss << " (line number not specified)";
  }

  const std::string trace = ss.str();
  PRINT_CONSOLE_MESSAGE_ERROR(ss.str().c_str());

#if defined(ELPP_COMPILER_MSVC) && defined(_M_IX86) && defined(_DEBUG)
  // Ignore msvc critical error dialog - break instead (on debug mode)
  _asm int 3
#else
  ::exit(EXIT_FAILURE);
#endif  // defined(ELPP_COMPILER_MSVC) && defined(_M_IX86) && defined(_DEBUG)
}


CrashHandler::CrashHandler()
{
  int i = 3;  // SIGABRT is at kCrashSignals[1]
  for (; i < crashSignalsCount; ++i) {
    m_handler = signal(crashSignals[i].numb, &handler);
  }
}


CrashHandler* CrashHandler::getInstance()
{
  static CrashHandler h;
  return &h;
}


void CrashHandler::handler(int sig)
{
  logCrashReason(sig, true);
  crashAbort(sig);
}
