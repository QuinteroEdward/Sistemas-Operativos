/*##################################################################################################
#                            Autores: Mateo Maldonado, Edward Quintero                             #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                       Tema: Proyecto Final                                       #
#                         Tópico: Sistema de Noticias publicador/Suscriptor                        #
####################################################################################################*/


/*
 * Suscriptor.c
 * Descripción: Este código es el componente de suscriptor, el cual recibe datos del sistema de comunicación a través de un pipe específico y procesa o muestra esta información en su salida estándar. Utiliza un argumento para definir el pipe.
 */

#include <stdio.h>    // Biblioteca para entrada y salida estándar
#include <stdlib.h>   // Biblioteca para gestión de memoria y control de procesos
#include <string.h>   // Biblioteca para manipulación de cadenas
#include <unistd.h>   // Biblioteca para operaciones del sistema
#include <fcntl.h>    // Biblioteca para control de archivos
#include <sys/types.h> // Biblioteca para definir tipos específicos
#include <sys/stat.h>  // Biblioteca para constantes de estado de archivos

// Función para suscribirse a temas específicos
void suscribirse(const char *pipe_name, const char *topics) {
    int fd = open(pipe_name, O_WRONLY); // Abre el pipe en modo escritura
    if (fd == -1) { // Verifica si hubo error al abrir el pipe
        perror("Error al abrir el pipe para enviar suscripciones"); // Muestra error
        exit(EXIT_FAILURE); // Termina el programa en caso de fallo
    }

    if (write(fd, topics, strlen(topics)) == -1) { // Escribe los temas de suscripción en el pipe
        perror("Error al enviar la suscripción"); // Muestra error si la escritura falla
    } else {
        printf("Suscriptor: Suscripción enviada -> %s\n", topics); // Indica que la suscripción fue enviada
    }

    close(fd); // Cierra el pipe después de enviar la suscripción
}

// Función para recibir noticias desde el sistema de comunicación
void recibir_noticias(const char *pipe_name) {
    int fd = open(pipe_name, O_RDONLY); // Abre el pipe en modo lectura
    if (fd == -1) { // Verifica si hubo error al abrir el pipe
        perror("Error al abrir el pipe para recibir noticias"); // Muestra error
        exit(EXIT_FAILURE); // Termina el programa en caso de fallo
    }

    char buffer[100]; // Buffer para almacenar la noticia recibida
    int count = 0; // Contador de noticias recibidas

    // Leer cada línea completa como una noticia separada
    FILE *pipe_stream = fdopen(fd, "r"); // Asocia el descriptor del pipe con un flujo de archivo
    if (pipe_stream == NULL) { // Verifica si hubo error al abrir el flujo
        perror("Error al abrir el stream del pipe"); // Muestra error
        close(fd); // Cierra el descriptor de archivo
        exit(EXIT_FAILURE); // Termina el programa en caso de fallo
    }

    while (fgets(buffer, sizeof(buffer), pipe_stream) != NULL) { // Lee línea por línea del flujo del pipe
        buffer[strlen(buffer) - 1] = '\0'; // Elimina el salto de línea al final
        printf("Suscriptor: Noticia recibida -> %s\n", buffer); // Muestra la noticia recibida
        count++; // Incrementa el contador de noticias
    }

    printf("Suscriptor finalizado. Total de noticias recibidas: %d\n", count); // Indica el total de noticias recibidas
    fclose(pipe_stream); // Cierra el flujo de archivo
}

// Función principal del programa
int main(int argc, char *argv[]) {
    char *pipe_name = NULL; // Nombre del pipe de comunicación
    char topics[10]; // Temas de interés del suscriptor
    int opt;

    // Procesa los argumentos de la línea de comandos
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        if (opt == 's') pipe_name = optarg; // Asigna el pipe según el argumento
    }

    // Verifica que el nombre del pipe sea válido
    if (!pipe_name) {
        fprintf(stderr, "Uso: %s -s pipe\n", argv[0]); // Muestra el uso correcto del programa
        exit(EXIT_FAILURE); // Termina si falta el argumento
    }

    printf("Ingrese los tópicos de suscripción (ej: A,E,C): "); // Solicita los temas de interés
    fgets(topics, sizeof(topics), stdin); // Lee los temas de la entrada estándar
    topics[strlen(topics) - 1] = '\0'; // Elimina el salto de línea al final

    suscribirse(pipe_name, topics); // Llama a la función para enviar la suscripción
    recibir_noticias(pipe_name); // Llama a la función para recibir noticias

    return 0; // Retorna 0 indicando ejecución exitosa
}

