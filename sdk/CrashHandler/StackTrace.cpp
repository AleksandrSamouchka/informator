#include "StackTrace.h"

#ifndef WIN32
#include <cxxabi.h>
#include <execinfo.h>
#endif
#include <cstdlib>


StackTrace::StackTraceEntry::StackTraceEntry(std::size_t index, const char* loc, const char* demang, const char* hex, const char* addr)
{
  m_index = index;
  m_location = std::string(loc);
  m_demangled = std::string(demang);
  m_hex = std::string(hex);
  m_addr = std::string(addr);
}


StackTrace::StackTraceEntry::StackTraceEntry(std::size_t index, char* loc)
{
  m_index = index;
  m_location = std::string(loc);
}


StackTrace::StackTrace()
{
  generateNew();
}


std::vector<StackTrace::StackTraceEntry>& StackTrace::getLatestStack()
{
  return m_stack;
}


void StackTrace::generateNew()
{
  m_stack.clear();
  void* stack[kMaxStack];

#ifdef WIN32 // TODO: а под виндой делается по другому, да и не надо пока

#else

  std::size_t size = backtrace(stack, kMaxStack);
  char** strings = backtrace_symbols(stack, size);
  if (size > kStackStart)
  {  // Skip StackTrace c'tor and generateNew
    for (std::size_t i = kStackStart; i < size; ++i)
    {
      char* mangName = NULL;
      char* hex = NULL;
      char* addr = NULL;
      for (char* c = strings[i]; *c; ++c)
      {
        switch (*c)
        {
        case '(':
          mangName = c;
          break;
        case '+':
          hex = c;
          break;
        case ')':
          addr = c;
          break;
        }
      }
      // Perform demangling if parsed properly
      if (mangName != NULL && hex != NULL && addr != NULL && mangName < hex)
      {
        *mangName++ = '\0';
        *hex++ = '\0';
        *addr++ = '\0';
        int status = 0;
        char* demangName = abi::__cxa_demangle(mangName, 0, 0, &status);
        // if demangling is successful, output the demangled function name
        if (status == 0)
        {
          // Success (see http://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html)
          StackTraceEntry entry(i - 1, strings[i], demangName, hex, addr);
          m_stack.push_back(entry);
        } else {
          // Not successful - we will use mangled name
          StackTraceEntry entry(i - 1, strings[i], mangName, hex, addr);
          m_stack.push_back(entry);
        }
        free(demangName);
      } else {
        StackTraceEntry entry(i - 1, strings[i]);
        m_stack.push_back(entry);
      }
    }
  }
  free(strings);

#endif
}
