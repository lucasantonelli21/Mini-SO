# --- Definições de Variáveis ---

# Compilador a ser utilizado
CC = gcc

# Flags de compilação:
# -Wall: Ativa todos os avisos (warnings) do compilador (importante para qualidade do código)
# -g:    Adiciona informações de debug (útil para usar com gdb ou valgrind)
# -pthread: Define macros necessárias para threads POSIX
CFLAGS = -Wall -g -pthread

# Flags de ligação (Linker):
# Necessário para linkar a biblioteca pthread
LDFLAGS = -pthread

# Nome do executável final
TARGET = alarme_so

# Lista automática de todos os arquivos .c no diretório atual
SRCS = $(wildcard *.c)

# Gera a lista de arquivos objeto (.o) baseada nos .c
OBJS = $(SRCS:.c=.o)

# --- Regras de Construção ---

# Regra padrão (o que acontece quando você digita apenas 'make')
all: $(TARGET)

# Regra para linkar o executável final
# $@ = nome do alvo (TARGET)
# $^ = todos os pré-requisitos (OBJS)
$(TARGET): $(OBJS)
	@echo "Ligando objetos para criar o executável: $(TARGET)..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Compilação concluída com sucesso!"

# Regra genérica para compilar qualquer .c em .o
# $< = o primeiro pré-requisito (o arquivo .c)
%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# --- Regras Utilitárias ---

# Limpa os arquivos compilados (objetos e executável)
clean:
	@echo "Limpando arquivos temporários..."
	rm -f $(OBJS) $(TARGET)

# Compila e executa o programa imediatamente
run: all
	@echo "Executando o sistema..."
	./$(TARGET)

# Ajuda para depuração com Valgrind (opcional, para verificar vazamento de memória)
valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# Define que estes alvos não são arquivos reais
.PHONY: all clean run valgrind