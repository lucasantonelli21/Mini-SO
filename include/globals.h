#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// --- Configurações ---
#define MAX_LOG_LEN 256
#define EVENT_QUEUE_SIZE 10

// --- Estados do Sistema ---
typedef enum {
    DISARMED,
    ARMED,
    TRIGGERED // Alarme disparado
} SystemState;

// --- Tipos de Eventos ---
typedef enum {
    EVT_NONE,
    EVT_TOGGLE_SENSOR, // Simula sensor ativando
    EVT_CMD_ARM,       // Comando do usuário
    EVT_CMD_DISARM,    // Comando do usuário
    EVT_STOP           // Parar o sistema
} EventType;

// --- Estrutura de Mensagem (Evento) ---
typedef struct {
    EventType type;
    int data; // Ex: ID do sensor ou senha
} EventMsg;

// --- Variáveis Globais (Compartilhadas) ---
// Estado do Sistema
extern SystemState current_state;
extern int sensors[3]; // 0=Porta, 1=Janela, 2=Movimento (0=Fechado, 1=Aberto)
extern int config_sensor_interval;
extern int config_password;
extern int system_running; // Flag para parar threads

// Sincronização
extern pthread_mutex_t state_mutex; // Protege current_state e sensors
extern pthread_mutex_t log_mutex;   // Protege a escrita no arquivo de log
extern pthread_mutex_t queue_mutex; // Protege a fila de eventos
extern pthread_cond_t queue_cond;   // Sinaliza novos eventos

// Fila de Eventos (Comunicação)
extern EventMsg event_queue[EVENT_QUEUE_SIZE];
extern int queue_count;
extern int queue_head;
extern int queue_tail;

#endif