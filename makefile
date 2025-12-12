# --- Definições de Variáveis ---

# Compilador a ser utilizado
CC = gcc

# Diretórios do projeto
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
CONFIG_DIR = config

# Flags de compilação:
# -Wall: Ativa todos os avisos (warnings) do compilador
# -g:    Adiciona informações de debug
# -pthread: Define macros necessárias para threads POSIX
# -I$(INC_DIR): Diretório de headers
CFLAGS = -Wall -g -pthread -I$(INC_DIR)

# Flags de ligação (Linker):
LDFLAGS = -pthread

# Nome do executável final
TARGET = alarme_so

# Lista automática de todos os arquivos .c no diretório src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Gera a lista de arquivos objeto (.o) na pasta obj/
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --- Regras de Construção ---

# Regra padrão
all: $(OBJ_DIR) $(TARGET)

# Cria diretório de objetos se não existir
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Regra para linkar o executável final
$(TARGET): $(OBJS)
	@echo "Ligando objetos para criar o executável: $(TARGET)..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Compilação concluída com sucesso!"

# Regra genérica para compilar .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# --- Regras Utilitárias ---

# Limpa os arquivos compilados
clean:
	@echo "Limpando arquivos temporários..."
	rm -rf $(OBJ_DIR) $(TARGET)

# Compila e executa o programa
run: all
	@echo "Executando o sistema..."
	./$(TARGET)

# Define que estes alvos não são arquivos reais
.PHONY: all clean run