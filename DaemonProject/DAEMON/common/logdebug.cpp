//
// Created by root on 19-8-30.
//
#include "../common/logdebug.h"
#include <uuid/uuid.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
namespace  nsLogDebug {
/***
 * (1)-lpthread
 * (2)logdebug(LOG_ERR,LOG_PRINT_FILE,"file:%s,line:%d:  alreading runing\n",__FILE__,__LINE__);
 * */
    pthread_rwlock_t rwlock;

    static const char *log_level[] =
            {
                    "INFO>",
                    "DEBUG>",
                    "WARN>",
                    "ERR>",
                    "FATAL>"
            };

    char *getCurrentDate(char *str) {
        if (!str)
            return NULL;

        time_t timer = time(NULL);
        struct tm *tblock;
        tblock = localtime(&timer);

        snprintf(str, 64, "%04d-%02d-%02d %02d:%02d:%02d", tblock->tm_year + 1900, tblock->tm_mon + 1,
                 tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);

        return str;
    }


   void logdebug(log_level_e level, const char *path, const char *fmt, ...) {
        FILE *fp = NULL;

        fp = fopen(path, "a");
        if (!fp) {
            perror("fopen");
            return;
        }

        char gentime[64] = {0};

        pthread_rwlock_trywrlock(&rwlock);//请求写锁

        getCurrentDate(gentime);

        fprintf(fp, "%s ", gentime);
        fprintf(fp, "%s\t", log_level[level]);

        va_list argp;

        va_start(argp, fmt);
        if (&argp) {
            vfprintf(fp, fmt, argp);
            vfprintf(fp, "\n", argp);
            va_end(argp);
        } else
            fprintf(fp, "%s\n", fmt);

        fflush(fp);
        if (fp != stdout)
            fclose(fp);
        pthread_rwlock_unlock(&rwlock);//解锁
        pthread_rwlock_destroy(&rwlock);

        return;
    }

}
