/*##################################################################################################
#                            Autores: Mateo Maldonado, Edward Quintero                             #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                       Tema: Proyecto Final                                       #
#                         Tópico: Sistema de Noticias publicador/Suscriptor                        #
####################################################################################################*/

/*
 * Publicador.c
 * Descripción: Este código representa el componente de publicador en un sistema de comunicación. Su función es leer información de un archivo y enviar estos datos al sistema de comunicación a través de un pipe. Utiliza los argumentos para definir el pipe y el archivo de entrada.
 */

// Incluye las bibliotecas estándar necesarias
#include <stdio.h>    // Biblioteca para entrada y salida estándar
#include <stdlib.h>   // Biblioteca para gestión de memoria y control de procesos
#include <string.h>   // Biblioteca para manipulación de cadenas de caracteres
#include <unistd.h>   // Biblioteca para operaciones del sistema (e.g., sleep)
#include <fcntl.h>    // Biblioteca para control de archivos
#include <sys/types.h> // Biblioteca para definir tipos de datos específicos
#include <sys/stat.h>  // Biblioteca para definir constantes de estado de archivos
#include <errno.h>     // Biblioteca para control de errores

// Función que envía noticias a través de un pipe
void enviar_noticias(const char *pipe_name, const char *file_name, int interval) {
    int fd; // Descriptor de archivo para el pipe
    FILE *file = fopen(file_name, "r"); // Abre el archivo de noticias en modo de lectura
    if (file == NULL) { // Verifica si hubo error al abrir el archivo
        perror("Error al abrir el archivo de noticias"); // Muestra el error
        exit(EXIT_FAILURE); // Finaliza el programa si no se puede abrir el archivo
    }

    // Intentar abrir el pipe en modo no bloqueante en un bucle
    while ((fd = open(pipe_name, O_WRONLY | O_NONBLOCK)) == -1) { // Intenta abrir el pipe en modo escritura y no bloqueante
        if (errno == ENXIO) {  // Si no hay lectores en el otro extremo
            printf("Publicador: Esperando a que el sistema de comunicación esté listo para leer...\n");
            sleep(1);  // Espera 1 segundo antes de intentar de nuevo
        } else {
            perror("Error al abrir el pipe"); // Muestra el error si no es ENXIO
            fclose(file); // Cierra el archivo
            exit(EXIT_FAILURE); // Finaliza el programa si no se puede abrir el pipe
        }
    }

    char linea[100]; // Buffer para almacenar una línea de noticias
    while (fgets(linea, sizeof(linea), file)) { // Lee línea por línea del archivo
        size_t len = strlen(linea); // Obtiene la longitud de la línea
        if (linea[len - 1] == '\n') { // Si la línea termina en salto de línea
            linea[len - 1] = '\0';  // Elimina el salto de línea
        }

        if (write(fd, linea, strlen(linea)) == -1) { // Escribe la línea en el pipe
            perror("Error al enviar la noticia"); // Muestra error si la escritura falla
        } else {
            printf("Publicador: Noticia enviada -> %s\n", linea); // Muestra la noticia enviada
        }
        sleep(interval); // Espera el intervalo especificado antes de enviar la siguiente
    }

    close(fd); // Cierra el pipe
    fclose(file); // Cierra el archivo de noticias
    printf("Publicador finalizado\n"); // Indica que el publicador ha terminado
}

// Función principal del programa
int main(int argc, char *argv[]) {
    char *pipe_name = NULL, *file_name = NULL; // Variables para el nombre del pipe y del archivo
    int interval = 0, opt; // Intervalo de envío y opción de argumento

    // Procesa los argumentos de la línea de comandos
    while ((opt = getopt(argc, argv, "p:f:t:")) != -1) { // Lee las opciones de los argumentos
        switch (opt) {
            case 'p': pipe_name = optarg; break; // Opción para el nombre del pipe
            case 'f': file_name = optarg; break; // Opción para el nombre del archivo de noticias
            case 't': interval = atoi(optarg); break; // Opción para el intervalo en segundos
            default:
                fprintf(stderr, "Uso: %s -p pipe -f archivo -t intervalo\n", argv[0]); // Muestra el uso correcto
                exit(EXIT_FAILURE); // Finaliza si hay un error de argumento
        }
    }

    if (!pipe_name || !file_name || interval <= 0) { // Verifica que todos los argumentos sean válidos
        fprintf(stderr, "Parámetros faltantes o incorrectos\n"); // Muestra error si faltan parámetros
        exit(EXIT_FAILURE); // Finaliza si faltan argumentos
    }

    enviar_noticias(pipe_name, file_name, interval); // Llama a la función para enviar noticias
    return 0; // Retorna 0 indicando ejecución exitosa
}

