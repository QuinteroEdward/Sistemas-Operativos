/*##################################################################################################
#                                        Autor: Edward Quintero                                    #
#                                         Fecha: 01/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                 Tema: Pipe Named o FIFO o Tuberia                                #
#                               Topico: Comunicacion Bi-Direccional                                #
#                                               Cliente                                            #
####################################################################################################*/

/*
Este código implementa un cliente simple que utiliza un pipe con nombre (FIFO) para enviar cadenas de texto a un servidor, recibir las cadenas invertidas y continuar el proceso hasta que se ingrese la cadena "end".

Funcionamiento:
1. Apertura del FIFO: Se abre el archivo FIFO en modo lectura/escritura.
2. Bucle principal:
   - Se solicita al usuario que ingrese una cadena.
   - Se lee la cadena y se elimina el carácter de nueva línea.
   - Se verifica si la cadena es "end" para finalizar el programa.
   - Se envía la cadena al servidor.
   - Se recibe la cadena invertida del servidor.
   - Se imprimen las cadenas enviadas y recibidas.
*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway"
int main() {
    int fd; // Descriptor de archivo para el FIFO
    int end_process; // Variable para indicar si se ha recibido la cadena "end"
    int stringlen; // Longitud de la cadena ingresada
    int read_bytes; // Número de bytes leídos del FIFO
    char readbuf[80]; // Buffer para leer datos del FIFO
    char end_str[5]; // Cadena "end" para indicar la finalización

    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

    // Abre el FIFO en modo lectura/escritura
    fd = open(FIFO_FILE, O_CREAT | O_RDWR, 0666);

    // Copia la cadena "end" en end_str
    strcpy(end_str, "end");

    // Bucle infinito hasta que se ingrese la cadena "end"
    while (1) {
        printf("Enter string: ");

        // Lee una cadena del teclado
        fgets(readbuf, sizeof(readbuf), stdin);

        // Obtiene la longitud de la cadena y elimina el carácter de nueva línea
        stringlen = strlen(readbuf);
        readbuf[stringlen - 1] = '\0';

        // Compara la cadena con "end"
        end_process = strcmp(readbuf, end_str);

        if (end_process != 0) {
            // Si la cadena no es "end", la envía al servidor
            write(fd, readbuf, strlen(readbuf));
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n",
                   readbuf, (int)strlen(readbuf));

            // Lee la cadena invertida del servidor
            read_bytes = read(fd, readbuf, sizeof(readbuf));
            readbuf[read_bytes] = '\0';
            printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n",
                   readbuf, (int)strlen(readbuf));
        } else   
 {
            // Si la cadena es "end", la envía al servidor y cierra el FIFO
            write(fd, readbuf, strlen(readbuf));
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n",
                   readbuf, (int)strlen(readbuf));
            close(fd);
            break;
        }
    }

    return   
 0;
}