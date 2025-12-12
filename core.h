#ifndef CORE_H
#define CORE_H

#include "globals.h"

void process_event(EventMsg evt);
void* core_task(void* arg);

#endif
