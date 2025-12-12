#include "monitor.h"
#include "globals.h"
#include "logger.h"

void* monitor_task(void* arg) {
    while (system_running) {
        // Tarefa Periódica: Dorme pelo tempo configurado
        sleep(config_sensor_interval);

        pthread_mutex_lock(&state_mutex);
        
        // Se o sistema estiver DISPARADO, a "sirene" continua tocando no log
        if (current_state == TRIGGERED) {
            log_msg("SIRENE: WEE-WOO-WEE-WOO (Alarme Ativo!)");
            printf("\n!!! SIRENE TOCANDO !!!\n"); 
        }
        
        pthread_mutex_unlock(&state_mutex);
    }
    return NULL;
}