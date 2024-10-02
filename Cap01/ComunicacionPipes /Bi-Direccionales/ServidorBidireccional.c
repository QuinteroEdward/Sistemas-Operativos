/*##################################################################################################
#                                      Autor: Edward Quintero                                      #
#                                         Fecha: 01/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                 Tema: Pipe Named o FIFO o Tuberia                                #
#                               Topico: Comunicacion Bi-Direccional                                #
#                                                Servidor                                          #
####################################################################################################*/

/*
Este código implementa un servidor simple que utiliza un pipe con nombre (FIFO) para recibir cadenas de texto de un cliente, invertirlas y enviarlas de vuelta.

Funcionamiento:
1. Creación del FIFO: Se crea un archivo FIFO si no existe, permitiendo la comunicación bidireccional entre procesos.
2. Apertura del FIFO: Se abre el FIFO en modo lectura/escritura.
3. Bucle principal:
   - Se lee una cadena del FIFO.
   - Se imprime la cadena recibida.
   - Se verifica si la cadena es "end" para finalizar el programa.
   - Se invierte la cadena.
   - Se imprime la cadena invertida.
   - Se escribe la cadena invertida en el FIFO.
   - Se espera un tiempo antes de la siguiente iteración para permitir que el cliente lea los datos.
*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway"  // Define el nombre del archivo FIFO

// Función para invertir una cadena de caracteres
void reverse_string(char *str) {
    int last = strlen(str) - 1;  // Índice del último carácter
    int first = 0;  // Índice del primer carácter

    // Intercambia los caracteres hasta llegar al medio de la cadena
    while (first < last) {
        char temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }
}

int main() {
    int fd;  // Descriptor de archivo para el FIFO
    char readbuf[80];  // Buffer para leer datos del FIFO
    char end[10] = "end";  // Cadena para indicar el final
    int to_end;  // Resultado de la comparación de cadenas

    // Crea el archivo FIFO si no existe
    mkfifo(FIFO_FILE, S_IFIFO | 0640);  // Permisos de lectura/escritura para el propietario y grupo

    // Abre el FIFO en modo lectura/escritura
    fd = open(FIFO_FILE, O_RDWR);

    // Bucle infinito hasta recibir la señal de finalización
    while (1) {
        // Lee datos del FIFO
        int read_bytes = read(fd, readbuf, sizeof(readbuf));
        readbuf[read_bytes] = '\0';  // Agrega el carácter nulo al final de la cadena

        // Imprime la cadena recibida
        printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

        // Compara la cadena recibida con "end"
        to_end = strcmp(readbuf, end);

        // Si se recibió "end", cierra el FIFO y termina el bucle
        if (to_end == 0) {
            close(fd);
            break;
        }

        // Invierte la cadena
        reverse_string(readbuf);

        // Imprime la cadena invertida
        printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

        // Escribe la cadena invertida en el FIFO
        write(fd, readbuf, strlen(readbuf));

        // Espera 2 segundos (puede ajustarse)
        sleep(2);  // Permite que el otro proceso lea los datos
    }

    return 0;
}

