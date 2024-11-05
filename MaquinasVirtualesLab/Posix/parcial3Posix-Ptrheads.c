
/*##################################################################################################
#                                    Autor: Edward Quintero                                        #
#                                         Fecha: 5/11/2024                                         #
#                                    Materia: Sistemas Operativos                                  #
#                               Tema: Parcial 3 sistemas operativos                                #
#                                     Tópico: Posix-Pthreads                                       #
####################################################################################################*/


#include <stdio.h>
#include <pthread.h>

/*
Este programa crea 5 hilos de ejecución. Cada hilo calcula el factorial de números del 1 al 300 y luego termina.
Se utiliza la biblioteca pthread para la gestión de hilos en C.
*/

void* hilo_ID_funcion(void* arg) {
    // Se obtiene el ID del hilo a partir del argumento
    int hilo_id = *(int*)arg;

    // Se imprime un mensaje indicando que el hilo ha iniciado
    printf("Hilo %d iniciado\n", hilo_id);

    // Se calcula el factorial de números del 1 al 300 (un bucle intensivo para simular trabajo)
    for (int i = 0; i < 300; i++) {
        int factorial = 1;
        for (int j = 1; j <= i; j++) {
            factorial *= j;
        }
    }

    // Se imprime un mensaje indicando que el hilo ha finalizado
    printf("Hilo %d finalizado\n", hilo_id);

    // Se termina la ejecución del hilo
    pthread_exit(NULL);
}

int main() {
    // Se declaran arreglos para almacenar los identificadores de los hilos
    pthread_t hilos[5];
    int id[5];

    // Se crean 5 hilos y se les asigna un ID único
    for (int i = 0; i < 5; i++) {
        id[i] = i + 1;
        // pthread_create: Crea un nuevo hilo
        // &hilos[i]: Puntero al atributo del hilo
        // NULL: Atributos por defecto
        // hilo_ID_funcion: Función que ejecutará el hilo
        // &id[i]: Argumento para la función del hilo (ID del hilo)
        pthread_create(&hilos[i], NULL, hilo_ID_funcion, &id[i]);
    }

    // Se espera a que todos los hilos terminen
    for (int i = 0; i < 5; i++) {
        // pthread_join: Espera a que un hilo termine
        // hilos[i]: Identificador del hilo
        // NULL: No se recupera ningún valor de retorno
        pthread_join(hilos[i], NULL);
    }

    return 0;
}
