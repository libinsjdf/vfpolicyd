#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "log.h"

static struct logger g_logger;

// logger util for vscprintf
int _scnprintf(char *buf, size_t size, const char *fmt, ...);
int _vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

int
_scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = _vscnprintf(buf, size, fmt, args);
    va_end(args);

    return i;
}

int
_vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int i;

    i = vsnprintf(buf, size, fmt, args);

    /*
     * The return value is the number of characters which would be written
     * into buf not including the trailing '\0'. If size is == 0 the
     * function returns 0.
     *
     * On error, the function also returns 0. This is to allow idiom such
     * as len += _vscnprintf(...)
     *
     * See: http://lwn.net/Articles/69419/
     */
    if (i <= 0) {
        return 0;
    }

    if (i < size) {
        return i;
    }

    return size - 1;
}

int
logger_init(int level, char * name)
{
    struct logger * logger = &g_logger;
    logger->level = level;
    if (name == NULL || !strlen(name)  ) {
        logger->fd = STDERR_FILENO;
    }
    else{
        logger->fd = open(name, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (logger->fd < 0) {
            _log_stderr("open log file '%s' error!", name);
            return -1;
        }
    }
    return 0;
}

void
logger_close()
{
    struct logger * logger = &g_logger;
    if (logger->fd >= 0 && logger->fd != STDERR_FILENO) {
        close(logger->fd);
    }
}

int
logger_canlog(int level)
{
    struct logger * logger = &g_logger;
    if (level >= logger->level) {
        return 1;
    }
    return 0;
}

void
_log(const char * file, int line, int is_panic, const char * fmt, ...) 
{
    struct logger * logger  = &g_logger;
    char buf[MAX_LOG_BUF], * time_str;
    va_list args;

    struct tm * local;
    time_t t;
    ssize_t n;

    int len,size;

    len = 0;
    size = MAX_LOG_BUF;

    if (logger->fd < 0) {
        return;
    }
    t = time(NULL);
    local = localtime(&t);
    time_str = asctime(local);

    len += _scnprintf(buf + len , size - len, "[%.*s] %s %d ", strlen(time_str) -1 , time_str, file, line);

    va_start(args, fmt);
    len += _vscnprintf(buf + len, size - len, fmt, args);
    va_end(args);

    buf[len++] = '\n';

    n = write(logger->fd, buf, len);
    if (n < 0) {
        return ;
    }

    if (is_panic) {
        abort();
    }
}

void
_log_stderr(const char * fmt, ...) {
    char buf[MAX_LOG_BUF], * time_str;
    int len , size;
    va_list args;
    struct tm * local;
    time_t t;
    ssize_t n;

    t = time(NULL);
    local = localtime(&t);
    time_str = asctime(local);

    size = MAX_LOG_BUF;
    len = 0;

    len += _scnprintf(buf + len , size - len, "[%.*s] ", strlen(time_str) -1 , time_str);
    va_start(args, fmt);
    len += _vscnprintf(buf + len, size - len, fmt, args);
    va_end(args);

    buf[len++] = '\n';

    n = write(STDERR_FILENO, buf, len);
    if (n < 0) {
        return ;
    }
}

int main()
{ 
    logger_init(LOG_INFO, "log");
    log_info("fuck");
    logger_close();
}

