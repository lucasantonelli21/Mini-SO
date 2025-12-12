#include "core.h"
#include "globals.h"
#include "logger.h"

void process_event(EventMsg evt) {
    char buffer[100];

    pthread_mutex_lock(&state_mutex);
    
    switch (evt.type) {
        case EVT_CMD_ARM:
            if (evt.data == config_password) {
                current_state = ARMED;
                log_msg("COMANDO: Sistema ARMADO.");
            } else {
                log_msg("ERRO: Senha incorreta ao tentar armar.");
            }
            break;

        case EVT_CMD_DISARM:
            if (evt.data == config_password) {
                current_state = DISARMED;
                log_msg("COMANDO: Sistema DESARMADO.");
            } else {
                log_msg("ERRO: Senha incorreta ao tentar desarmar.");
            }
            break;

        case EVT_TOGGLE_SENSOR:
            // Atualiza estado do sensor simulado
            if (evt.data >= 0 && evt.data < 3) {
                sensors[evt.data] = !sensors[evt.data];
                sprintf(buffer, "SENSOR: Sensor %d mudou para %d", evt.data, sensors[evt.data]);
                log_msg(buffer);

                // Se estiver armado e sensor for ativado (1), DISPARA
                if (current_state == ARMED && sensors[evt.data] == 1) {
                    current_state = TRIGGERED;
                    log_msg("ALERTA: ALARME DISPARADO POR SENSOR!");
                }
            }
            break;
            
        default:
            break;
    }
    pthread_mutex_unlock(&state_mutex);
}

void* core_task(void* arg) {
    while (system_running) {
        EventMsg evt;
        
        // Sincronização: Espera passiva por evento
        pthread_mutex_lock(&queue_mutex);
        while (queue_count == 0 && system_running) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        // Verifica se deve parar
        if (!system_running) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }

        // Retira da fila (FIFO)
        evt = event_queue[queue_head];
        queue_head = (queue_head + 1) % EVENT_QUEUE_SIZE;
        queue_count--;
        
        pthread_mutex_unlock(&queue_mutex);

        if (evt.type == EVT_STOP) break;

        process_event(evt);
    }
    return NULL;
}
