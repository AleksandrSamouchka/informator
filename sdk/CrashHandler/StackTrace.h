#ifndef STACKTRACE_H
#define STACKTRACE_H

#include <iostream>
#include <string>
#include <vector>


class StackTrace
{
  public:
    class StackTraceEntry
    {
      public:
        StackTraceEntry(std::size_t index, const char* loc, const char* demang, const char* hex, const char* addr);
        StackTraceEntry(std::size_t index, char* loc);
        std::size_t m_index;
        std::string m_location;
        std::string m_demangled;
        std::string m_hex;
        std::string m_addr;
        friend std::ostream& operator<<(std::ostream& ss, const StackTraceEntry& si)
        {
          ss << "[" << si.m_index << "] " << si.m_location << (si.m_demangled.empty() ? "" : ":") << si.m_demangled
             << (si.m_hex.empty() ? "" : "+") << si.m_hex << si.m_addr;
          return ss;
        }

      private:
        StackTraceEntry(void);
    };

    StackTrace(void);

    std::vector<StackTraceEntry>& getLatestStack(void);
    friend std::ostream& operator<<(std::ostream& os, const StackTrace& st)
    {
      std::vector<StackTraceEntry>::const_iterator it = st.m_stack.begin();
      while (it != st.m_stack.end()) {
        os << "    " << *it++ << "\n";
      }
      return os;
    }

    static const std::size_t kMaxStack = 64;
    static const std::size_t kStackStart = 2;

  private:
    void generateNew(void);

    std::vector<StackTraceEntry> m_stack;
};

#endif // STACKTRACE_H
