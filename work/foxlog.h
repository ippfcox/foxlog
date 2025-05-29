#ifndef FOXLOG_H__
#define FOXLOG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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

#define foxlogerrorm(logger, fmt, ...)                                                                 \
    do                                                                                                 \
    {                                                                                                  \
        struct foxlogger *logger_ = logger ? logger : g_foxlogger;                                     \
        if (FOXLOG_LEVEL_ERROR >= *(int *)logger_)                                                     \
            foxlog_log(logger_, FOXLOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                                                  \
    while (0)

#define foxlogerror(fmt, ...) foxlogerrorm(NULL, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif //__cplusplus
#endif // FOXLOG_H__