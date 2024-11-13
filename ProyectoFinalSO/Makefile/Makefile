/*##################################################################################################
#                            Autores: Mateo Maldonado, Edward Quintero                             #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                       Tema: Proyecto Final                                       #
#                         Tópico: Make - Sistema de Noticias publicador/Suscriptor                 #
####################################################################################################*/

# Este Makefile automatiza el proceso de compilación de un sistema de comunicación
# entre procesos en C, compuesto por tres ejecutables: SistemaComunicacion, Publicador y Suscriptor.
# También crea y elimina los pipes (canales de comunicación) necesarios para la comunicación entre estos
# ejecutables. El Makefile simplifica la compilación y limpieza del proyecto.

# Variables de compilador y flags
CC = gcc          # Define el compilador a utilizar (gcc en este caso)
CFLAGS = -Wall    # Define los flags de compilación: -Wall muestra todas las advertencias posibles

# Archivos fuente y ejecutables
SISTEMA_SRC = SistemaComunicacion.c   # Archivo fuente del ejecutable SistemaComunicacion
PUBLICADOR_SRC = Publicador.c         # Archivo fuente del ejecutable Publicador
SUSCRIPTOR_SRC = Suscriptor.c         # Archivo fuente del ejecutable Suscriptor

SISTEMA_EXEC = SistemaComunicacion    # Nombre del ejecutable para SistemaComunicacion
PUBLICADOR_EXEC = Publicador          # Nombre del ejecutable para Publicador
SUSCRIPTOR_EXEC = Suscriptor          # Nombre del ejecutable para Suscriptor

# Nombres de los pipes
PIPE_PSC = pipePSC    # Nombre del pipe entre Publicador y SistemaComunicacion
PIPE_SSC1 = pipeSSC1  # Nombre del primer pipe entre Suscriptor y SistemaComunicacion
PIPE_SSC2 = pipeSSC2  # Nombre del segundo pipe entre Suscriptor y SistemaComunicacion

# Compilación de todos los ejecutables
all: $(SISTEMA_EXEC) $(PUBLICADOR_EXEC) $(SUSCRIPTOR_EXEC) pipes
# La regla 'all' define que al ejecutar 'make' se compilarán todos los ejecutables
# y se crearán los pipes necesarios

$(SISTEMA_EXEC): $(SISTEMA_SRC)
	$(CC) $(CFLAGS) -o $(SISTEMA_EXEC) $(SISTEMA_SRC)
	# Compila el archivo SistemaComunicacion.c y genera el ejecutable SistemaComunicacion

$(PUBLICADOR_EXEC): $(PUBLICADOR_SRC)
	$(CC) $(CFLAGS) -o $(PUBLICADOR_EXEC) $(PUBLICADOR_SRC)
	# Compila el archivo Publicador.c y genera el ejecutable Publicador

$(SUSCRIPTOR_EXEC): $(SUSCRIPTOR_SRC)
	$(CC) $(CFLAGS) -o $(SUSCRIPTOR_EXEC) $(SUSCRIPTOR_SRC)
	# Compila el archivo Suscriptor.c y genera el ejecutable Suscriptor

# Crear los pipes
pipes:
	mkfifo $(PIPE_PSC)  # Crea un pipe (FIFO) llamado pipePSC para comunicación entre Publicador y SistemaComunicacion
	mkfifo $(PIPE_SSC1) # Crea un pipe (FIFO) llamado pipeSSC1 para comunicación entre Suscriptor y SistemaComunicacion
	mkfifo $(PIPE_SSC2) # Crea un pipe (FIFO) llamado pipeSSC2 para comunicación entre Suscriptor y SistemaComunicacion

# Limpieza de ejecutables y pipes
clean:
	rm -f $(SISTEMA_EXEC) $(PUBLICADOR_EXEC) $(SUSCRIPTOR_EXEC) $(PIPE_PSC) $(PIPE_SSC1) $(PIPE_SSC2)
	# Elimina todos los ejecutables y pipes generados, limpiando el directorio de trabajo



