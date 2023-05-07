#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <signal.h>

const struct {
    int numb;
    const char* name;
    const char* brief;
    const char* detail;
} crashSignals[] = {
  // NOTE: Do not re-order, if you do please check CrashHandler(bool) constructor and CrashHandler::setHandler(..)
  { SIGINT, "SIGINT", "Interactive attention signal",
    "Interruption generated (generally) by user or operating system." },
#ifndef WIN32
  { SIGKILL, "SIGKILL", "Killed",
    "Program terminated immediately by user or operating system." },
#endif
  { SIGTERM, "SIGTERM", "Terminated",
    "Program terminated by user or operating system." },
  { SIGABRT, "SIGABRT", "Abnormal termination",
    "Program was abnormally terminated." },
  { SIGFPE, "SIGFPE", "Erroneous arithmetic operation",
    "Arithemetic operation issue such as division by zero or operation resulting in overflow." },
  { SIGILL, "SIGILL", "Illegal instruction",
    "Generally due to a corruption in the code or to an attempt to execute data."},
  { SIGSEGV, "SIGSEGV", "Invalid access to memory",
    "Program is trying to read an invalid (unallocated, deleted or corrupted) or inaccessible memory." },
#ifndef WIN32
  { SIGSYS, "SIGSYS", "Bad system call",
    "Bad argument sended to system call." },
#endif
  { SIGSEGV, "SIGSEGV", "Segmentation fault",
    "Invalid memory reference." },
#ifndef WIN32
  { SIGBUS, "SIGBUS", "Bus error",
    "Access to an undefined portion of a memory object." },
  { SIGXFSZ, "SIGXFSZ", "File size limit exceeded",
    "File larger than the maximum allowed size." },
#endif
};


static const int crashSignalsCount = sizeof(crashSignals) / sizeof(crashSignals[0]);


class CrashHandler
{
  public:
    typedef void (*Handler) (int);

    static CrashHandler* getInstance();

private:
    inline explicit CrashHandler();
    static void handler(int sig);

    Handler m_handler = &handler;
};

#endif // CRASHHANDLER_H
