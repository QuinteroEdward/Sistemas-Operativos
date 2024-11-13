/*##################################################################################################
#                            Autores: Mateo Maldonado, Edward Quintero                             #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                       Tema: Proyecto Final                                       #
#                         Tópico: Sistema de Noticias publicador/Suscriptor                        #
####################################################################################################*/

/*
 * SistemaComunicacion.c
 * Descripción: Este archivo representa el sistema central de comunicación que recibe datos de un publicador y los distribuye a uno o más suscriptores a través de pipes. Controla la transmisión mediante argumentos específicos que definen el comportamiento de los pipes.
 */

// Incluye las bibliotecas necesarias
#include <stdio.h>      // Biblioteca para entrada y salida
#include <stdlib.h>     // Biblioteca para gestión de memoria y control de procesos
#include <string.h>     // Biblioteca para manipulación de cadenas
#include <unistd.h>     // Biblioteca para operaciones del sistema
#include <fcntl.h>      // Biblioteca para control de archivos
#include <sys/types.h>  // Biblioteca para definir tipos específicos
#include <sys/stat.h>   // Biblioteca para constantes de estado de archivos

#define MAX_SUSCRIPTORES 10 // Número máximo de suscriptores permitidos
#define MAX_NOTICIAS 100     // Número máximo de noticias almacenadas

// Definición de estructura para los suscriptores
typedef struct {
    int id;               // Identificador del suscriptor
    char topics[10];      // Temas de interés del suscriptor
} Suscriptor;

// Definición de estructura para las noticias
typedef struct {
    char noticia[100];    // Texto de la noticia
    char topic;           // Tema de la noticia
} Noticia;

// Variables globales para almacenar suscriptores y noticias
Suscriptor suscriptores[MAX_SUSCRIPTORES];
Noticia noticias[MAX_NOTICIAS];
int total_noticias = 0;      // Contador de noticias almacenadas
int total_suscriptores = 0;   // Contador de suscriptores registrados

// Función para almacenar una noticia en el sistema
void almacenar_noticia(const char *linea) {
    if (total_noticias < MAX_NOTICIAS) { // Verifica si hay espacio para más noticias
        strncpy(noticias[total_noticias].noticia, linea, sizeof(noticias[total_noticias].noticia) - 1); // Copia la noticia
        noticias[total_noticias].topic = linea[0]; // Almacena el tema (primer carácter)
        total_noticias++; // Incrementa el contador de noticias
    }
}

// Función para procesar las suscripciones de los suscriptores
void procesar_suscripciones(const char *pipe_suscriptores) {
    int fd = open(pipe_suscriptores, O_RDONLY); // Abre el pipe de suscriptores en modo lectura
    if (fd == -1) { // Verifica si hubo error al abrir el pipe
        perror("Error al abrir pipe de suscriptores"); // Muestra error en caso de fallo
        exit(EXIT_FAILURE); // Termina el programa
    }

    char buffer[10]; // Buffer para almacenar los temas de suscripción
    while (read(fd, buffer, sizeof(buffer) - 1) > 0) { // Lee datos del pipe
        buffer[strlen(buffer)] = '\0'; // Asegura que el buffer termine en carácter nulo
        if (total_suscriptores < MAX_SUSCRIPTORES) { // Verifica si hay espacio para más suscriptores
            strcpy(suscriptores[total_suscriptores].topics, buffer); // Copia los temas en el suscriptor
            suscriptores[total_suscriptores].id = total_suscriptores + 1; // Asigna un ID único
            printf("Sistema de Comunicación: Suscriptor %d registrado para temas -> %s\n", suscriptores[total_suscriptores].id, buffer);
            total_suscriptores++; // Incrementa el contador de suscriptores
        }
    }
    close(fd); // Cierra el pipe de suscriptores
}

// Función para enviar noticias a los suscriptores
void enviar_noticias_a_suscriptores(const char *pipe_suscriptor1, const char *pipe_suscriptor2) {
    int fd_sub1 = open(pipe_suscriptor1, O_WRONLY); // Abre el pipe del suscriptor 1 en modo escritura
    int fd_sub2 = open(pipe_suscriptor2, O_WRONLY); // Abre el pipe del suscriptor 2 en modo escritura

    if (fd_sub1 == -1 || fd_sub2 == -1) { // Verifica si hubo error al abrir alguno de los pipes
        perror("Error al abrir pipe para enviar a suscriptores"); // Muestra error en caso de fallo
        return; // Termina la función si hay error
    }

    for (int i = 0; i < total_noticias; i++) { // Itera sobre cada noticia almacenada
        for (int j = 0; j < total_suscriptores; j++) { // Itera sobre cada suscriptor
            if (strchr(suscriptores[j].topics, noticias[i].topic)) { // Verifica si el tema coincide
                char mensaje[105]; // Buffer para almacenar el mensaje
                snprintf(mensaje, sizeof(mensaje), "%s\n", noticias[i].noticia); // Formatea el mensaje con la noticia

                // Envía la noticia al suscriptor correspondiente según su ID
                if (suscriptores[j].id == 1) {  // Suscriptor 1
                    if (write(fd_sub1, mensaje, strlen(mensaje)) == -1) { // Envía mensaje y verifica si hubo error
                        perror("Error al enviar noticia al suscriptor 1"); // Muestra error
                    } else {
                        printf("Noticia Enviada al suscriptor 1 categoria -> %s\n", suscriptores[j].topics);
                    }
                } else if (suscriptores[j].id == 2) {  // Suscriptor 2
                    if (write(fd_sub2, mensaje, strlen(mensaje)) == -1) { // Envía mensaje y verifica si hubo error
                        perror("Error al enviar noticia al suscriptor 2"); // Muestra error
                    } else {
                        printf("Noticia Enviada al suscriptor 2 categoria -> %s\n", suscriptores[j].topics);
                    }
                }

                usleep(50000);  // Pausa breve entre envíos (50 ms)
            }
        }
    }

    close(fd_sub1); // Cierra el pipe del suscriptor 1
    close(fd_sub2); // Cierra el pipe del suscriptor 2
}

// Procesa las noticias recibidas desde el publicador y envía a los suscriptores
void procesar_noticias(const char *pipe_publicadores, const char *pipe_suscriptor1, const char *pipe_suscriptor2) {
    procesar_suscripciones(pipe_suscriptor1); // Procesa suscripciones del suscriptor 1
    procesar_suscripciones(pipe_suscriptor2); // Procesa suscripciones del suscriptor 2

    int fd_pub = open(pipe_publicadores, O_RDONLY); // Abre el pipe de publicadores en modo lectura
    if (fd_pub == -1) { // Verifica si hubo error al abrir el pipe
        perror("Error al abrir pipe de publicadores"); // Muestra error en caso de fallo
        exit(EXIT_FAILURE); // Termina el programa
    }

    char linea[100]; // Buffer para almacenar cada línea de noticia
    while (read(fd_pub, linea, sizeof(linea) - 1) > 0) { // Lee del pipe publicador
        linea[strlen(linea)] = '\0'; // Asegura que el buffer termine en carácter nulo
        printf("Sistema de Comunicación: Noticia recibida del publicador -> %s\n", linea); // Muestra la noticia recibida
        almacenar_noticia(linea); // Almacena la noticia en el sistema
    }

    close(fd_pub); // Cierra el pipe de publicadores
    enviar_noticias_a_suscriptores(pipe_suscriptor1, pipe_suscriptor2); // Envía las noticias a suscriptores
    printf("Sistema de comunicación finalizado\n"); // Indica fin del proceso
}

// Función principal del programa
int main(int argc, char *argv[]) {
    char *pipe_publicadores = NULL;
    char *pipe_suscriptor1 = NULL, *pipe_suscriptor2 = NULL;
    int timeF = 0, opt;

    // Procesa los argumentos de la línea de comandos
    while ((opt = getopt(argc, argv, "p:s:2:t:")) != -1) {
        switch (opt) {
            case 'p': pipe_publicadores = optarg; break;
            case 's': pipe_suscriptor1 = optarg; break;
            case '2': pipe_suscriptor2 = optarg; break;
            case 't': timeF = atoi(optarg); break;
            default:
                fprintf(stderr, "Uso: %s -p pipePub -s pipeSub1 -2 pipeSub2 -t time\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verifica que los argumentos sean válidos
    if (!pipe_publicadores || !pipe_suscriptor1 || !pipe_suscriptor2 || timeF <= 0) {
        fprintf(stderr, "Parámetros faltantes o incorrectos\n");
        exit(EXIT_FAILURE);
    }

    procesar_noticias(pipe_publicadores, pipe_suscriptor1, pipe_suscriptor2); // Procesa las noticias
    sleep(timeF); // Pausa el programa por el tiempo especificado
    printf("Sistema de comunicación terminado\n");

    return 0; // Retorna 0 indicando ejecución exitosa
}

