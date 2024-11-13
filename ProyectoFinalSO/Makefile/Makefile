# Variables de compilador y flags
CC = gcc
CFLAGS = -Wall

# Archivos fuente y ejecutables
SISTEMA_SRC = SistemaComunicacion.c
PUBLICADOR_SRC = Publicador.c
SUSCRIPTOR_SRC = Suscriptor.c

SISTEMA_EXEC = SistemaComunicacion
PUBLICADOR_EXEC = Publicador
SUSCRIPTOR_EXEC = Suscriptor

# Nombres de los pipes
PIPE_PSC = pipePSC
PIPE_SSC1 = pipeSSC1
PIPE_SSC2 = pipeSSC2

# Compilación de todos los ejecutables
all: $(SISTEMA_EXEC) $(PUBLICADOR_EXEC) $(SUSCRIPTOR_EXEC) pipes

$(SISTEMA_EXEC): $(SISTEMA_SRC)
	$(CC) $(CFLAGS) -o $(SISTEMA_EXEC) $(SISTEMA_SRC)

$(PUBLICADOR_EXEC): $(PUBLICADOR_SRC)
	$(CC) $(CFLAGS) -o $(PUBLICADOR_EXEC) $(PUBLICADOR_SRC)

$(SUSCRIPTOR_EXEC): $(SUSCRIPTOR_SRC)
	$(CC) $(CFLAGS) -o $(SUSCRIPTOR_EXEC) $(SUSCRIPTOR_SRC)

# Crear los pipes
pipes:
	mkfifo $(PIPE_PSC)
	mkfifo $(PIPE_SSC1)
	mkfifo $(PIPE_SSC2)

# Limpieza de ejecutables y pipes
clean:
	rm -f $(SISTEMA_EXEC) $(PUBLICADOR_EXEC) $(SUSCRIPTOR_EXEC) $(PIPE_PSC) $(PIPE_SSC1) $(PIPE_SSC2)


