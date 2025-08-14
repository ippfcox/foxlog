// The MIT License (MIT)

// Copyright 2025 ippfcox

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the “Software”), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// foxlog - A lightweight header-only C logging library

// Update Date: 2025.8.14 16:00

// Usage: 

// #define FOXLOG_IMPLEMENTATION
// #include "foxlog.h"

// TODO
// [ ] Support redirect log output (file/stdout/etc.)
// [ ] Add runtime color enable/disable option
// [ ] Support thread ID
// [ ] Thread safety

#ifndef FOXLOG_H__
#define FOXLOG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>

    enum foxlog_level
    {
        FOXLOG_LEVEL_DEBUG,
        FOXLOG_LEVEL_INFO,
        FOXLOG_LEVEL_WARN,
        FOXLOG_LEVEL_ERROR,
        FOXLOG_LEVEL_FATAL,
        FOXLOG_LEVEL_TAIL,
    };

    struct foxlogger;
    extern struct foxlogger *g_foxlogger;
    static const int level_colors[] = {37, 34, 33, 31, 31};
    static const char *level_names[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

    struct foxlogger *foxlog_create(const char *module_name);
    void foxlog_destroy(struct foxlogger *logger);
    void foxlog_set_level(struct foxlogger *logger, enum foxlog_level level);
    void foxlog_log(struct foxlogger *logger, enum foxlog_level level, const char *file, int line, const char *func, const char *fmt, ...);

#define foxlogdebugm(logger, fmt, ...)                                                                 \
    do                                                                                                 \
    {                                                                                                  \
        struct foxlogger *logger_ = logger ? logger : g_foxlogger;                                     \
        if (FOXLOG_LEVEL_DEBUG >= *(int *)logger_)                                                     \
            foxlog_log(logger_, FOXLOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                                                  \
    while (0)
#define foxloginfom(logger, fmt, ...)                                                                 \
    do                                                                                                \
    {                                                                                                 \
        struct foxlogger *logger_ = logger ? logger : g_foxlogger;                                    \
        if (FOXLOG_LEVEL_INFO >= *(int *)logger_)                                                     \
            foxlog_log(logger_, FOXLOG_LEVEL_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                                                 \
    while (0)
#define foxlogwarnm(logger, fmt, ...)                                                                 \
    do                                                                                                \
    {                                                                                                 \
        struct foxlogger *logger_ = logger ? logger : g_foxlogger;                                    \
        if (FOXLOG_LEVEL_WARN >= *(int *)logger_)                                                     \
            foxlog_log(logger_, FOXLOG_LEVEL_WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                                                 \
    while (0)
#define foxlogerrorm(logger, fmt, ...)                                                                 \
    do                                                                                                 \
    {                                                                                                  \
        struct foxlogger *logger_ = logger ? logger : g_foxlogger;                                     \
        if (FOXLOG_LEVEL_ERROR >= *(int *)logger_)                                                     \
            foxlog_log(logger_, FOXLOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                                                  \
    while (0)
#define foxlogfatalm(logger, fmt, ...)                                                                 \
    do                                                                                                 \
    {                                                                                                  \
        struct foxlogger *logger_ = logger ? logger : g_foxlogger;                                     \
        if (FOXLOG_LEVEL_FATAL >= *(int *)logger_)                                                     \
            foxlog_log(logger_, FOXLOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                                                  \
    while (0)

#define foxlogdebug(fmt, ...) foxlogdebugm(NULL, fmt, ##__VA_ARGS__)
#define foxloginfo(fmt, ...) foxloginfom(NULL, fmt, ##__VA_ARGS__)
#define foxlogwarn(fmt, ...) foxlogwarnm(NULL, fmt, ##__VA_ARGS__)
#define foxlogerror(fmt, ...) foxlogerrorm(NULL, fmt, ##__VA_ARGS__)
#define foxlogfatal(fmt, ...) foxlogfatalm(NULL, fmt, ##__VA_ARGS__)

#ifdef FOXLOG_IMPLEMENTATION

#define MODULE_NAME_LEN 16

    struct foxlogger
    {
        enum foxlog_level level; // must be 1st member!
        char module_name[MODULE_NAME_LEN];
    };

    struct foxlogger g_foxlogger_ = {
        .level = FOXLOG_LEVEL_WARN,
    };
    struct foxlogger *g_foxlogger = &g_foxlogger_;

    struct foxlogger *foxlog_create(const char *module_name)
    {
        struct foxlogger *logger = (struct foxlogger *)calloc(1, sizeof(struct foxlogger));
        if (!logger)
        {
#if defined(_WIN32)
            char err_str[256] = {0};
            strerror_s(err_str, 256, errno);
            fprintf(stderr, "calloc failed: %s\n", err_str);
#elif defined(__linux__)
            fprintf(stderr, "calloc failed: %s\n", strerror(errno));
#else
            fprintf(stderr, "calloc failed: %s\n", strerror(errno));
#endif
            return NULL;
        }

        if (module_name)
        {
#if defined(_WIN32)
            strncpy_s(logger->module_name, MODULE_NAME_LEN, module_name, MODULE_NAME_LEN);
#elif defined(__linux__)
            strncpy(logger->module_name, module_name, MODULE_NAME_LEN);
#else
            strncpy(logger->module_name, module_name, MODULE_NAME_LEN);
#endif
        }
        logger->level = FOXLOG_LEVEL_WARN;

        return logger;
    }

    void foxlog_destroy(struct foxlogger *logger)
    {
        if (!logger)
            return;
        free(logger);
    }

    void foxlog_set_level(struct foxlogger *logger, enum foxlog_level level)
    {
        logger = logger ? logger : g_foxlogger;
        logger->level = level;
    }

    void foxlog_log(struct foxlogger *logger, enum foxlog_level level, const char *file, int line, const char *func, const char *fmt, ...)
    {
        logger = logger ? logger : g_foxlogger;

        // datetime
        time_t t = time(NULL);
        char time_str[32] = {0};
#if defined(_WIN32)
        struct tm tm;
        localtime_s(&tm, &t);
        strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", &tm);
#elif defined(__linux__)
        strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", localtime(&t));
#else
        strftime(time_str, 32, "%Y-%m-%d %H:%M:%S", localtime(&t));
#endif

        // parse filename

#if defined(_WIN32)
        const char *pos = strrchr(file, '\\');
#elif defined(__linux__)
        const char *pos = strrchr(file, '/');
#else
        const char *pos = strrchr(file, '/');
#endif

        char fmt_str[2048] = {0}; // extend
#if defined(_WIN32)
        snprintf(fmt_str, 2048, "[%s]\t%s %s:%d %s() %s ",
            level_names[level], time_str, pos ? pos + 1 : file, line, func, logger->module_name);
#elif defined(__linux__)
        snprintf(fmt_str, 2048, "\033[1;%dm[%s]\033[0m\t\033[32m%s \033[35m%s:%d \033[36m%s()\033[0m \033[1;34m%s\033[0m ",
            level_colors[level], level_names[level], time_str, pos ? pos + 1 : file, line, func, logger->module_name);
#else
        snprintf(fmt_str, 2048, "\033[1;%dm[%s]\033[0m\t\033[32m%s \033[35m%s:%d \033[36m%s()\033[0m \033[1;34m%s\033[0m ",
            level_colors[level], level_names[level], time_str, pos ? pos + 1 : file, line, func, logger->module_name);
#endif

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

        if (level == FOXLOG_LEVEL_FATAL)
            exit(-1);
    }
#endif // FOXLOG_IMPLEMENTATION

#ifdef __cplusplus
}
#endif //__cplusplus
#endif // FOXLOG_H__