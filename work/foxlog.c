#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include "foxlog.h"

#define MODULE_NAME_LEN 8

struct foxlogger
{
    enum foxlog_level level; // must be 1st member!
    char module_name[MODULE_NAME_LEN];
};

struct foxlogger *g_foxlogger = &(struct foxlogger){
    .level = FOXLOG_LEVEL_WARN,
};

struct foxlogger *foxlog_create(const char *module_name)
{
    struct foxlogger *logger = calloc(1, sizeof(struct foxlogger));
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