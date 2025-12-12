#include "globals.h"
#include "core.h"
#include "monitor.h"
#include "interface.h"
#include "logger.h"

// Instanciando as globais
SystemState current_state = DISARMED;
int sensors[3] = {0, 0, 0}; 
int config_sensor_interval = 2; // Default
int config_password = 1234;     // Default
int system_running = 1;         // Flag para parar threads

pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

EventMsg event_queue[EVENT_QUEUE_SIZE];
int queue_count = 0;
int queue_head = 0;
int queue_tail = 0;

void load_config() {
    FILE *f = fopen("config.txt", "r");
    if (f) {
        fscanf(f, "INTERVAL=%d\n", &config_sensor_interval);
        fscanf(f, "PASSWORD=%d\n", &config_password);
        fclose(f);
        log_msg("Configuracao carregada com sucesso.");
    } else {
        log_msg("Erro ao abrir config.txt. Usando padroes.");
    }
}

int main() {
    // 1. Inicializa Sistema de Arquivos (Log)
    init_logger();
    log_msg("Sistema Iniciando...");
    
    // 2. Carrega Configuração
    load_config();

    // 3. Criação das Threads (Tarefas)
    pthread_t thread_core, thread_monitor, thread_ui;

    pthread_create(&thread_core, NULL, core_task, NULL);
    pthread_create(&thread_monitor, NULL, monitor_task, NULL);
    pthread_create(&thread_ui, NULL, interface_task, NULL);

    // 4. Aguarda o término (Join)
    pthread_join(thread_ui, NULL); // O sistema encerra quando a UI termina
    
    // Sinaliza para as outras threads pararem
    system_running = 0;
    pthread_cond_broadcast(&queue_cond); // Acorda thread core se estiver esperando
    
    pthread_join(thread_core, NULL);
    pthread_join(thread_monitor, NULL);

    log_msg("Sistema Encerrado.");
    close_logger();
    
    // Limpeza
    pthread_mutex_destroy(&state_mutex);
    pthread_mutex_destroy(&log_mutex);
    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_cond);

    return 0;
}