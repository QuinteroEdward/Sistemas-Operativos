/*############################################################################################ #                                                                                            #
#                                    Autor: Edward Andres Quintero                           # #                                                                                            #
#                                         Fecha: 23/08/2024                                  # #                                                                                            #
#                                    Materia: Sistemas Operativos                            # #                                                                                            #
#                                      Tema: Gestión de memoria                              # #                                                                                            #
#                         Tópico: Reasignación de memoria con la función rmalloc()           # #                                                                                            #
##############################################################################################*/

/*
   La función realloc() se utiliza para cambiar el tamaño de un bloque de memoria previamente asignado con malloc().
Recibe dos argumentos:
* ptr: Un puntero al bloque de memoria existente.
* size: El nuevo tamaño deseado para el bloque, en bytes.

Si hay suficiente memoria disponible para expandir el bloque, realloc() devuelve un puntero al nuevo bloque de memoria.
El contenido original del bloque se conserva hasta el mínimo entre el tamaño antiguo y el nuevo.
Si no hay suficiente memoria, realloc() devuelve NULL y el bloque original permanece sin cambios.
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    // Declaración de variables
    int *numeros; // Puntero para almacenar la dirección del arreglo dinámico
    int i, tamano_inicial = 5, nuevo_tamano = 10;

    // Asignación de memoria inicial para 5 enteros
    numeros = (int *)malloc(tamano_inicial * sizeof(int));

    // Verificar si la asignación fue exitosa
    if (numeros == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1; // Terminar el programa con un código de error
    }

    // Inicializar los elementos del arreglo con valores
    for (i = 0; i < tamano_inicial; i++) {
        numeros[i] = i * 2;
    }

    // Imprimir el arreglo original
    printf("Arreglo original:\n");
    for (i = 0; i < tamano_inicial; i++) {
        printf("%d ", numeros[i]);
    }
    printf("\n");

    // Reasignar memoria para 10 enteros
    numeros = (int *)realloc(numeros, nuevo_tamano * sizeof(int));

    // Verificar si la reasignación fue exitosa
    if (numeros == NULL) {
        printf("Error: No se pudo reasignar memoria.\n");
        // Liberar la memoria asignada previamente (importante)
        free(numeros);
        return 1;
    }

    // Inicializar los nuevos elementos del arreglo (a partir del índice donde se amplió)
    for (i = tamano_inicial; i < nuevo_tamano; i++) {
        numeros[i] = i * 3;
    }

    // Imprimir el arreglo después de realloc()
    printf("Arreglo después de realloc:\n");
    for (i = 0; i < nuevo_tamano; i++) {
        printf("%d ", numeros[i]);
    }
    printf("\n");

    // Liberar la memoria asignada
    free(numeros);

    return 0;
}