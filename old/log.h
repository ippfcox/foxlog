// modified: 2024-9-26 17:22
#ifndef LOG_H__
#define LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>

    enum log_level
    {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL,
    };

    static enum log_level log_level_ = LOG_LEVEL_WARN;
    static char fmt_str[2048];
    static const int level_colors[] = {37, 34, 33, 31, 31};
    static const char *level_names[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    static char time_str[32];

    void set_log_level(enum log_level level);

    void log_(enum log_level level, const char *file, int line, const char *func, const char *fmt, ...);

#define logdebug(fmt, ...)                                                       \
    do                                                                           \
    {                                                                            \
        log_(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                            \
    while (0)

#define loginfo(fmt, ...)                                                       \
    do                                                                          \
    {                                                                           \
        log_(LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                           \
    while (0)

#define logwarn(fmt, ...)                                                       \
    do                                                                          \
    {                                                                           \
        log_(LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                           \
    while (0)

#define logerror(fmt, ...)                                                       \
    do                                                                           \
    {                                                                            \
        log_(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                            \
    while (0)

#define logfatal(fmt, ...)                                                       \
    do                                                                           \
    {                                                                            \
        log_(LOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                            \
    while (0)

#ifdef LOG_IMPLEMENTATION__
    void set_log_level(enum log_level level)
    {
        log_level_ = level;
    }

    void log_(enum log_level level, const char *file, int line, const char *func, const char *fmt, ...)
    {
        if (level < log_level_)
            return;

        time_t t = time(NULL);
#if defined(_WIN32)
        struct tm tm;
        localtime_s(&tm, &t);
        strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", &tm);
#elif defined(__linux__)
        strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", localtime(&t));
#else
        strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", localtime(&t));
#endif

        const char *pos = strrchr(file, '/');
        snprintf(fmt_str, 2048, "\033[1;%dm[%s]\033[0m\t\033[32m%s \033[35m%s:%d \033[36m%s()\033[0m ",
            level_colors[level], level_names[level], time_str, pos ? pos + 1 : file, line, func);
        va_list args;

        va_start(args, fmt);
        vsnprintf(fmt_str + strlen(fmt_str), 2048, fmt, args);
        va_end(args);

        if (fmt_str[strlen(fmt_str) - 1] != '\n')
        {
#if defined(_WIN32)
            strncat_s(fmt_str, 2048, "\n", 1);
#elif defined(__linux__)
            strncat(fmt_str, "\n", 2);
#else
            strncat(fmt_str, "\n", 2);
#endif
        }

        fprintf(stderr, "%s", fmt_str);

        if (level == LOG_LEVEL_FATAL)
            exit(-1);
    }
#endif // LOG_IMPLEMENTATION__

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LOG_H__