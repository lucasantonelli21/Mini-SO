# Trabalho Final: Mini Sistema Operacional - Controle de Alarme

**Disciplina:** Sistemas Operacionais II
**Instituição:** [Nome da Universidade/Faculdade]
**Data:** 12/12/2025

**Integrantes do Grupo:**

- [Nome do Aluno 1]
- [Nome do Aluno 2]
- [Nome do Aluno 3]

---

## 1. Descrição do Projeto

Este projeto consiste na implementação de um **Mini Sistema Operacional em Espaço de Usuário**, desenvolvido em linguagem C sobre Linux. O sistema simula o funcionamento de uma central de alarme residencial, gerenciando múltiplas tarefas concorrentes, comunicação entre processos e persistência de dados.

O objetivo é demonstrar a aplicação prática dos seguintes conceitos:

- **Multithreading:** Execução concorrente de tarefas (monitoramento, interface, núcleo).
- **Sincronização:** Uso de Mutexes e Variáveis de Condição para evitar condições de corrida.
- **IPC (Inter-Process Communication):** Comunicação entre threads via memória compartilhada e filas de mensagens.
- **Sistema de Arquivos:** Persistência de logs e leitura de configurações.

---

## 2. Arquitetura do Sistema

O sistema foi arquitetado utilizando a biblioteca **POSIX Threads (pthreads)** e está dividido em 4 módulos de execução principais:

### 2.1. Tarefas (Threads)

| Thread          | Tipo       | Responsabilidade                                                                                                 |
| :-------------- | :--------- | :--------------------------------------------------------------------------------------------------------------- |
| **`Core`**      | Reativa    | Núcleo do sistema. Processa eventos da fila, gerencia a máquina de estados (ARMED/DISARMED) e toma decisões.     |
| **`Monitor`**   | Periódica  | Simula o hardware. Verifica o estado dos sensores em intervalos definidos e mantém a sirene ativa se necessário. |
| **`Interface`** | Interativa | Gerencia a entrada do usuário (CLI) e envia comandos para o núcleo.                                              |
| **`Logger`**    | Serviço    | Garante a escrita thread-safe de eventos no arquivo de log.                                                      |

### 2.2. Sincronização e Comunicação

- **Fila de Eventos:** Implementada como um buffer circular em memória compartilhada. A Interface e o Monitor agem como _produtores_ e o Core como _consumidor_.
- **Mutex (`pthread_mutex_t`):**
  - `queue_mutex`: Protege o acesso à fila de eventos.
  - `state_mutex`: Protege o estado global do sistema e o array de sensores.
  - `log_mutex`: Garante exclusão mútua na escrita do arquivo de log.
- **Variável de Condição (`pthread_cond_t`):** Utilizada pelo `Core` para aguardar novos eventos sem consumir CPU (_sleep_ eficiente), sendo sinalizado pelas outras threads.

---

## 3. Estrutura dos Arquivos

O código-fonte está organizado da seguinte forma:

- **`main.c`**: Ponto de entrada. Inicializa as threads, carrega configurações e realiza a limpeza de memória ao sair.
- **`globals.h`**: Definições globais, estruturas de dados (fila, eventos) e variáveis de sincronização extern.
- **`core.c/.h`**: Lógica principal da máquina de estados.
- **`monitor.c/.h`**: Lógica de simulação dos sensores e temporização.
- **`interface.c/.h`**: Interface de linha de comando (CLI).
- **`logger.c/.h`**: Funções de escrita em arquivo (`log.txt`).
- **`Makefile`**: Script para automação da compilação.
- **`config.txt`**: Arquivo de configuração editável.

---

## 4. Instruções de Compilação e Execução

### Pré-requisitos

- Sistema Operacional Linux (ou WSL).
- Compilador GCC instalado.
- Utilitário Make (opcional).

### Compilando

Abra o terminal na pasta do projeto e execute:

```bash
make
```
