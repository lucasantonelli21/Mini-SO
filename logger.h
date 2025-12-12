#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

void init_logger();
void log_msg(const char *msg);
void close_logger();

#endif
