#include "logger.h"
#include "globals.h"

FILE *log_file;

void init_logger() {
    log_file = fopen("log.txt", "a");
    if (!log_file) {
        perror("Falha ao abrir log.txt");
        exit(1);
    }
}

void log_msg(const char *msg) {
    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0'; // Remove newline

    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        fprintf(log_file, "[%s] %s\n", date, msg);
        fflush(log_file); // Garante escrita imediata no disco
    }
    pthread_mutex_unlock(&log_mutex);
}

void close_logger() {
    if (log_file) fclose(log_file);
}
