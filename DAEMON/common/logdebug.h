//
// Created by root on 19-8-30.
//

#ifndef COMMUNICATION_LOGDEBUG_H
#define COMMUNICATION_LOGDEBUG_H
namespace  nsLogDebug {
#define LOG_PRINT_FILE   "/opt/daemonProgram/logDebug/communication.log"
#define  LOG_PRINT_UI_FILE  "/opt/daemonProgram/logDebug/ui.log"


    typedef enum {
        LOG_INFO,
        LOG_DEBUG,
        LOG_WARN,
        LOG_ERR,
        LOG_FATAL
    } log_level_e;

    extern void logdebug(log_level_e level, const char *path, const char *fmt, ...);

    extern char *getCurrentDate(char *str);
}

#endif //COMMUNICATION_LOGDEBUG_H
