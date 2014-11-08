#ifndef _YOUMIX_LOG_H_
#define _YOUMIX_LOG_H_

#include "ostream.hpp"

#define LOG(...) Log::Log_Variadic(__FILE__, __LINE__, __VA_ARGS__)

namespace Log {
    inline void Log_Recursive(io::ostream&) {}

    template<typename T, typename... Args>
    void Log_Recursive(io::ostream& os, T hd, const Args&... rest) {
        os << hd << " ";
        Log_Recursive(os, rest...);
    }

    template<typename... Args>
    void Log_Variadic(const char* file, int line, const Args&... rest) {
        io::cout << file << "(" << line << "): ";
        Log_Recursive(io::cout, rest...);
        io::cout << io::endl;
    }
}

#endif
