#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <cstdio>
#include <cstring>

#define LOG_ERROR(fmt, ...)  fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)   fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)   fprintf(stdout, "[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)  fprintf(stdout, "[DEBUG] " fmt "\n", ##__VA_ARGS__)

#endif // _LOGGER_H_
