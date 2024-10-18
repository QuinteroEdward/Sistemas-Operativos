/*##################################################################################################
#                             Autores: Mateo Maldonado y Edward Quintero                           #
#                                         Fecha: 17/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                            Tema: Procesos y comunicación entre procesos                          #
#   Apertura de ficheros y valores con el formato "./taller_procesos N1 archivo00 N2 archivo01"    #
####################################################################################################*/

/*
  Descripción del programa:
  Este programa lee dos arreglos de enteros desde dos archivos diferentes,
  calcula la suma de los elementos en cada uno utilizando procesos separados,
  y luego suma ambos resultados. Para la comunicación entre los procesos hijos 
  y el proceso padre se utilizan pipes (tuberías). El proceso padre espera la 
  finalización de ambos hijos y, una vez obtenidos los resultados, imprime las 
  sumas de cada archivo y la suma total en la consola.

  Parámetros del programa:
  - N1: Cantidad de enteros a leer del primer archivo.
  - archivo00: Nombre del archivo que contiene el primer conjunto de enteros.
  - N2: Cantidad de enteros a leer del segundo archivo.
  - archivo01: Nombre del archivo que contiene el segundo conjunto de enteros.

  Funcionalidad:
  1. Lee los enteros desde los dos archivos proporcionados.
  2. Crea dos procesos hijos, cada uno calculando la suma de los enteros de su respectivo archivo.
  3. Usa pipes para pasar los resultados de los hijos al proceso padre.
  4. Imprime las sumas individuales y la suma total de ambos arreglos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Función para leer un archivo y cargar el arreglo dinámico
int *leer_fichero(const char *nombre_fichero, int N) {
  FILE *fichero = fopen(nombre_fichero, "r");
  if (!fichero) {
    perror("Error al abrir el fichero");
    exit(EXIT_FAILURE);
  }

  // Reservar memoria para el arreglo dinámico
  int *arreglo = (int *)malloc(N * sizeof(int));
  if (!arreglo) {
    perror("Error al reservar memoria");
    fclose(fichero);  // Asegurarse de cerrar el archivo antes de salir
    exit(EXIT_FAILURE);
  }

  // Leer el contenido del fichero en el arreglo
  for (int i = 0; i < N; i++) {
    if (fscanf(fichero, "%d", &arreglo[i]) != 1) {
      perror("Error al leer del fichero");
      free(arreglo);  // Liberar la memoria reservada
      fclose(fichero);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fichero);  // Cerrar el fichero
  return arreglo;
}

// Función para calcular la suma de un arreglo
int calcular_suma(int *arreglo, int N) {
  int suma = 0;
  for (int i = 0; i < N; i++) {
    suma += arreglo[i];
  }
  return suma;
}

int main(int argc, char *argv[]) {
  // Validar el número de argumentos proporcionados
  if (argc != 5) {
    fprintf(stderr, "Uso: %s N1 archivo00 N2 archivo01\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Parsear los argumentos
  int N1 = atoi(argv[1]);
  char *archivo00 = argv[2];
  int N2 = atoi(argv[3]);
  char *archivo01 = argv[4];

  // Leer los arreglos desde los ficheros
  int *arreglo00 = leer_fichero(archivo00, N1);
  int *arreglo01 = leer_fichero(archivo01, N2);

  // Crear pipes para la comunicación entre procesos
  int pipe_sumaA[2], pipe_sumaB[2];
  if (pipe(pipe_sumaA) == -1 || pipe(pipe_sumaB) == -1) {
    perror("Error al crear el pipe");
    exit(EXIT_FAILURE);
  }

  pid_t pid1, pid2;

  // Primer hijo: calcula la suma del primer arreglo (arreglo00)
  if ((pid1 = fork()) == 0) {
    close(pipe_sumaA[0]);  // Cerrar el lado de lectura del pipe
    int sumaA = calcular_suma(arreglo00, N1);
    write(pipe_sumaA[1], &sumaA, sizeof(sumaA));
    close(pipe_sumaA[1]);  // Cerrar el lado de escritura después de usarlo
    exit(0);
  } else if (pid1 < 0) {
    perror("Error en fork");
    exit(EXIT_FAILURE);
  }

  // Segundo hijo: calcula la suma del segundo arreglo (arreglo01)
  if ((pid2 = fork()) == 0) {
    close(pipe_sumaB[0]);  // Cerrar el lado de lectura del pipe
    int sumaB = calcular_suma(arreglo01, N2);
    write(pipe_sumaB[1], &sumaB, sizeof(sumaB));
    close(pipe_sumaB[1]);  // Cerrar el lado de escritura después de usarlo
    exit(0);
  } else if (pid2 < 0) {
    perror("Error en fork");
    exit(EXIT_FAILURE);
  }

  // Proceso padre: espera los resultados de los hijos
  int sumaA, sumaB;
  close(pipe_sumaA[1]);  // Cerrar el lado de escritura del pipe
  close(pipe_sumaB[1]);

  read(pipe_sumaA[0], &sumaA, sizeof(sumaA));
  read(pipe_sumaB[0], &sumaB, sizeof(sumaB));

  close(pipe_sumaA[0]);  // Cerrar el lado de lectura después de usarlo
  close(pipe_sumaB[0]);

  // Imprime los resultados
  printf("Suma del archivo00: %d\n", sumaA);
  printf("Suma del archivo01: %d\n", sumaB);
  printf("Suma total: %d\n", sumaA + sumaB);

  // Esperar a que los procesos hijos terminen
  wait(NULL);
  wait(NULL);

  // Liberar la memoria reservada
  free(arreglo00);
  free(arreglo01);

  return 0;
}