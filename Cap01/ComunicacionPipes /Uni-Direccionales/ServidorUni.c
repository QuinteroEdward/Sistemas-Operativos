/*##################################################################################################
#                                       Autor:Edward Quintero                                      #
#                                         Fecha: 24/09/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                 Tema: Pipe Named o FIFO o Tuberia                                #
#                               Topico: Comunicacion Uni-Direccional                               #
#                                               Servidor                                           #
####################################################################################################*/

/*
Este código implementa un servidor simple que utiliza un pipe con nombre (FIFO) para recibir mensajes de un cliente. 

Funcionamiento:
1. Creación del FIFO: Se crea un archivo FIFO (pipe con nombre) llamado "MYFIFO" si no existe. Este archivo servirá como canal de comunicación entre el servidor y el cliente.
2. Bucle infinito: El servidor entra en un bucle infinito para escuchar continuamente mensajes del cliente.
3. Apertura de la FIFO: En cada iteración del bucle, se abre el FIFO en modo de solo lectura para recibir los mensajes del cliente.
4. Lectura de datos: Se leen datos del FIFO y se almacenan en un búfer.
5. Impresión de datos: Se imprime en la consola la cadena recibida y la cantidad de bytes leídos.
6. Comparación con "fin": Se compara la cadena recibida con la palabra "fin". Si coincide, significa que el cliente desea finalizar la comunicación.
7. Finalización: Si se detecta la palabra "fin", se cierra el FIFO y se sale del bucle, terminando la ejecución del servidor.
*/


#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>


#define FIFO_FILE "MYFIFO" 

int main() {
  int fd;          // Descriptor de archivo para abrir la FIFO
  char leerbuf[80];// Buffer para almacenar la cadena leída de la FIFO
  char fin[10];    // Buffer para la palabra "fin" que finaliza el servidor
  int final;       // Variable para almacenar el resultado de la comparación de cadenas
  int leer_bytes;  // Variable que almacena la cantidad de bytes leídos

  printf("\n \t\t >>>>>> Inicio Servidor <<<<<<< \n");

  /* Crear la FIFO (named pipe) si no existe */
  mknod(FIFO_FILE, S_IFIFO | 0640, 0);

  /* Inicializamos la cadena que usaremos para comparar si el mensaje es "fin" */
  strcpy(fin, "fin");

  /* Bucle infinito para leer desde la FIFO hasta que recibamos la palabra "fin" */
  while (1) {
    /* Abrir la FIFO en modo de solo lectura */
    fd = open(FIFO_FILE, O_RDONLY);

    /* Leer datos de la FIFO y almacenarlos en el buffer leerbuf */
    leer_bytes = read(fd, leerbuf, sizeof(leerbuf));

    /* Añadir un carácter nulo al final de la cadena para asegurar que esté terminada */
    leerbuf[leer_bytes] = '\0';

    /* Imprimir la cadena recibida y la cantidad de bytes leídos */
    printf("Cadena recibida: \"%s\" y cantidad de bytes %d\n", leerbuf, (int)strlen(leerbuf));

    /* Comparar la cadena leída con "fin" para determinar si el servidor debe finalizar */
    final = strcmp(leerbuf, fin);

    /* Si la cadena es "fin", cerrar el descriptor de archivo y salir del bucle */
    if (final == 0) {
      close(fd);  // Cerrar la FIFO
      break;      // Salir del bucle y finalizar el programa
    }
  }

  return 0;  // Finalizar la ejecución del programa
}