/*##################################################################################################
#                                    Autor: Edward Quintero                                        #
#                                         Fecha: 29/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                               Tema: Laboratorio Maquinas Virtuales                               #
#                                     Tópico: ficheros Posix                                       #
####################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>// Biblioteca para el manejo de hilos

/*
 * Este programa en C implementa un modelo productor-consumidor usando múltiples hilos y variables de condición.
 * En este modelo, múltiples hilos productores generan recursos y aumentan un contador global, mientras que 
 * múltiples hilos consumidores consumen recursos y disminuyen este contador. El programa utiliza mutexes y 
 * variables de condición para sincronizar la operación de los hilos y evitar condiciones de carrera.
 * 
 * Detalle:
 * - Cuando el contador alcanza un límite máximo, los productores esperan hasta que el contador disminuya.
 * - Cuando el contador es cero, los consumidores esperan hasta que el contador aumente.
 * - `pthread_mutex_lock` y `pthread_mutex_unlock` aseguran la exclusión mutua en secciones críticas, mientras que 
 *   `pthread_cond_wait` y `pthread_cond_signal` manejan la espera y notificación entre los hilos.
 */


int counter = 0;                                        // Contador global que representa el recurso compartido
int max = 4;                                            // Máxima cantidad de hilos productores y consumidores

// Mutexes para la sincronización del contador y las condiciones
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condc_mutex = PTHREAD_MUTEX_INITIALIZER;

// Variables de condición para productores y consumidores
pthread_mutex_t condVarProd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condVarCons = PTHREAD_MUTEX_INITIALIZER;

// Prototipos de funciones para los hilos productor y consumidor
void *consumidor();
void *productor();

int main(int argc, char *argv[]) {
    pthread_t prodHilo[max];                            // Vector para los hilos productores
    pthread_t consHilo[max];                            // Vector para los hilos consumidores

    // Bucle para crear hilos productores y consumidores
    for(int i = 0; i < max; i++) {
        // Creación de hilos y asignación de las funciones productor y consumidor
        pthread_create(&prodHilo[i], NULL, &productor, NULL);
        pthread_create(&consHilo[i], NULL, &consumidor, NULL);
    }

    // Bucle para esperar a que todos los hilos terminen
    for(int i = 0; i < max; i++) {
        pthread_join(prodHilo[i], NULL);                // Espera a que el hilo productor finalice
        pthread_join(consHilo[i], NULL);                // Espera a que el hilo consumidor finalice
    }
    return 0;
}

/*
 * Función del hilo productor.
 * Produce incrementando el contador y notifica a los consumidores cuando el contador cambia.
 */
void *productor() {
    while(1) {
        pthread_mutex_lock(&condp_mutex);               // Bloquea el mutex de condición para productores

        while(counter >= 10)                            // Espera si el contador alcanzó el límite
            pthread_cond_wait(&condVarProd, &condp_mutex);

        pthread_mutex_unlock(&condp_mutex);             // Libera el mutex de condición para productores

        pthread_mutex_lock(&counter_mutex);             // Bloquea el mutex del contador
        counter++;                                      // Incrementa el contador
        pthread_cond_signal(&condVarCons);              // Señala a los consumidores que hay un recurso disponible
        printf("Soy el PRODUCTOR %d || Contador = %d\n", pthread_self(), counter);
        pthread_mutex_unlock(&condp_mutex);             // Libera el mutex de condición para productores
        pthread_mutex_unlock(&counter_mutex);           // Libera el mutex del contador

        if(counter > 7)                                 // Si el contador está alto, hace una pausa
            sleep(4);
    }
}

/*
 * Función del hilo consumidor.
 * Consume decrementando el contador y notifica a los productores cuando el contador cambia.
 */
void *consumidor() {
    while(1) {
        sleep(1);                                       // Pausa de 1 segundo antes de consumir
        pthread_mutex_lock(&condc_mutex);               // Bloquea el mutex de condición para consumidores

        while(counter <= 0) {                           // Espera si no hay recursos disponibles
            pthread_cond_signal(&condVarProd);          // Señala a los productores que pueden producir
            pthread_cond_wait(&condVarCons, &condc_mutex); // Espera a que el contador aumente
        }

        pthread_mutex_unlock(&condc_mutex);             // Libera el mutex de condición para consumidores

        pthread_mutex_lock(&counter_mutex);             // Bloquea el mutex del contador
        if(counter > 0) {                               // Consume si el contador es mayor a cero
            printf("Soy el CONSUMIDOR %d || Contador = %d\n", pthread_self(), counter);
            counter--;                                  // Decrementa el contador
            pthread_cond_signal(&condVarProd);          // Señala a los productores que hay espacio
        }

        pthread_mutex_unlock(&counter_mutex);           // Libera el mutex del contador
        pthread_mutex_unlock(&condc_mutex);             // Libera el mutex de condición para consumidores
    }
}



