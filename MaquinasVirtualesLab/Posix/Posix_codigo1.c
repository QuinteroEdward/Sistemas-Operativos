/*##################################################################################################
#                                    Autor: Edward Quintero                                        #
#                                         Fecha: 29/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                               Tema: Laboratorio Maquinas Virtuales                               #
#                                     Tópico: ficheros Posix                                       #
####################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>// Biblioteca para el manejo de hilos

/*
 * Este programa en C crea múltiples hilos usando la biblioteca pthread.
 * Su funcionalidad es inicializar N trabajos (o tareas) y asignar cada uno
 * a un hilo que imprimirá un mensaje con el número de trabajo correspondiente.
 * 
 * Uso:
 * El programa recibe un argumento en la línea de comandos que especifica 
 * la cantidad de trabajos (hilos) que debe crear y manejar. Cada hilo
 * se ejecuta en paralelo y procesa un trabajo, imprimiendo su identificador.
 * 
 * Detalle:
 * - Cada hilo se crea con `pthread_create`, y se le asigna un índice de trabajo.
 * - `pthread_join` asegura que el programa espera a que todos los hilos finalicen.
 */



/* 
 * Función que se ejecuta en cada hilo. 
 * Recibe un argumento que indica el número del trabajo e imprime ese número.
 */
static void *funcion(void *argumentos) {
    size_t job = *(size_t *)argumentos;       // Extrae el número de trabajo desde el argumento
    printf("\n\t Job %zu \n", job);           // Imprime el número de trabajo
    return NULL;                              // Retorna NULL al finalizar
}

int main(int argc, char *argv[]) {
    int N = (int)atoi(argv[1]);               // Convierte el argumento de entrada (cantidad de hilos) en entero
    size_t jobs[N];                           // Vector para almacenar los identificadores de trabajos
    pthread_t hilos[N];                       // Vector para almacenar los identificadores de hilos

    // Bucle para inicializar y crear cada hilo
    for(int i = 0; i < N; i++) {
        jobs[i] = i;                          // Asigna el índice del trabajo al vector `jobs`
        // Crea un hilo y le pasa la función `funcion`, junto con el número de trabajo
        pthread_create(&hilos[i], NULL, funcion, jobs + i);
    }
    
    // Bucle para esperar a que cada hilo termine su ejecución
    for(int i = 0; i < N; i++)
        pthread_join(hilos[i], NULL);         // Espera a que el hilo `hilos[i]` finalice

    return 0;                                 // Retorna 0 indicando que el programa finalizó correctamente
}


