#ifndef ANSWER_LOGGER_H
#define ANSWER_LOGGER_H

#include "../Other/Logger.h"
#include "../Other/Answer.h"
#include <cstdarg>

namespace Answer {

class Logger {
public:
    static void print(LogLevel level, const char* format, ...) {
        va_list args;
        va_start(args, format);
        if (level >= LogLevel::LOG_LEVEL_ERROR) {
            fprintf(stderr, "[ERROR] ");
            vfprintf(stderr, format, args);
        } else {
            fprintf(stdout, "[INFO] ");
            vfprintf(stdout, format, args);
        }
        va_end(args);
    }

    static void print(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
    }
};

} // namespace Answer

#endif // ANSWER_LOGGER_H
