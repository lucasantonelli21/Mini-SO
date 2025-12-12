#ifndef INTERFACE_H
#define INTERFACE_H

#include "globals.h"

void send_event(EventType type, int data);
void* interface_task(void* arg);

#endif
