
/*
 * Simple logger in C
 * inspired by memcached's logger
 * written by c4pt0r
 */
#ifndef _LOG_H_
#define _LOG_H_

struct logger {
    char * name;
    int fd;
    int level;
};

#define LOG_INFO  0
#define LOG_DEBUG 1
#define LOG_WARN  2
#define LOG_EMERG 3

#define MAX_LOG_BUF 256

// log functions
void _log(const char * file, int line, int is_panic, const char * fmt, ...);
void _log_err(const char * fmt, ...);
int  logger_init(int level, char * name);
void logger_close();

// logger interfaces
#define log_info(...) do {                                                  \
    if (logger_canlog(LOG_INFO) != 0) {                                     \
        _log(__FILE__, __LINE__, 0, __VA_ARGS__);                           \
    }                                                                       \
} while (0)

#define log_debug(...) do {                                                 \
    if (logger_canlog(LOG_DEBUG) != 0) {                                    \
        _log(__FILE__, __LINE__, 0, __VA_ARGS__);                           \
    }                                                                       \
} while (0)

#define log_warn(...) do {                                                  \
    if (logger_canlog(LOG_WARN) != 0) {                                     \
        _log(__FILE__, __LINE__, 0, __VA_ARGS__);                           \
    }                                                                       \
} while (0)

#define log_warn(...) do {                                                  \
    if (logger_canlog(LOG_EMERG) != 0) {                                    \
        _log(__FILE__, __LINE__, 0, __VA_ARGS__);                           \
    }                                                                       \
} while (0)

#endif
