#include "interface.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>

void send_event(EventType type, int data) {
    pthread_mutex_lock(&queue_mutex);
    if (queue_count < EVENT_QUEUE_SIZE) {
        event_queue[queue_tail].type = type;
        event_queue[queue_tail].data = data;
        queue_tail = (queue_tail + 1) % EVENT_QUEUE_SIZE;
        queue_count++;
        pthread_cond_signal(&queue_cond); // Acorda o Core
    } else {
        printf("ERRO: Fila de eventos cheia!\n");
    }
    pthread_mutex_unlock(&queue_mutex);
}

void* interface_task(void* arg) {
    char cmd[20];
    int val;

    printf("--- Mini SO Alarme Iniciado ---\n");
    printf("Comandos: arm <senha>, disarm <senha>, sensor <id>, status, exit\n");

    while (1) {
        printf("> ");
        scanf("%s", cmd);

        if (strcmp(cmd, "exit") == 0) {
            send_event(EVT_STOP, 0);
            break;
        } 
        else if (strcmp(cmd, "arm") == 0) {
            scanf("%d", &val);
            send_event(EVT_CMD_ARM, val);
            usleep(100000); // Aguarda 100ms para processar evento
        } 
        else if (strcmp(cmd, "disarm") == 0) {
            scanf("%d", &val);
            send_event(EVT_CMD_DISARM, val);
            usleep(100000); // Aguarda 100ms para processar evento
        }
        else if (strcmp(cmd, "sensor") == 0) {
            scanf("%d", &val); // Simula acionamento do sensor ID (0, 1 ou 2)
            send_event(EVT_TOGGLE_SENSOR, val);
            usleep(100000); // Aguarda 100ms para processar evento
        }
        else if (strcmp(cmd, "status") == 0) {
            pthread_mutex_lock(&state_mutex);
            printf("Estado: %d (0=OFF, 1=ARM, 2=TRIG)\n", current_state);
            printf("Sensores: [0]=%d, [1]=%d, [2]=%d\n", sensors[0], sensors[1], sensors[2]);
            pthread_mutex_unlock(&state_mutex);
        }
        else {
            printf("Comando desconhecido.\n");
        }
    }
    return NULL;
}
