/*##################################################################################################
#                                    Autor: Edward Quintero                                        #
#                                         Fecha: 17/09/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                         Tema: Funcion fork                                       #
#                               Tópico: Creacion de multiples procesos                             #
####################################################################################################*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork(); // Crea un nuevo proceso

    if (pid < 0) {
        // Si fork() falla
        perror("fork falló");
        return 1;
    }

    // Ambos procesos (padre e hijo) hacen el ciclo para contar
    for (int i = 1; i <= 5; i++) {
        if (pid == 0) {
            // Proceso hijo
            printf("Hijo: %d\n", i);
            sleep(1); // Espera 1 segundo
        } else {
            // Proceso padre
            printf("Padre: %d\n", i);
            sleep(1); // Espera 1 segundo
        }
    }

    return 0;
}
