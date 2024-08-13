/*
Autor: E. Quintero
Fecha: 13/08/2024
Materia: Sistemas Operativos
Tema: Gestión de memoria 
Tópico: Reserva de memoria con la función memalloc().
----------------------------------------------------------------------------------------
la función memalloc() reserva un espacio de memoria de tamaño requerido.
los espacios de igual tamaño y memoria conigua.
la asignación es dinamica y util para complejas estructuras de datos
inicialmente los bloques contiguos que devuelve son inicializados en 0
devuelve un puntero que señala el inicio del espacio de memoria reservado anteriormente.
*/

#include <stdio.h>
#include <stdlib.h>

int main(){
/* se declaran las variables y punteros necesarios*/
int *p1 = calloc(4,sizeof(int)); // Asignación de vector de 4 tamaño int 
int *puntero = calloc(10,sizeof(int)); // Bloque de memoria de 15 int
int *p3 = calloc(5,sizeof*p3); // Bloque de memoria de 15 int

/*Bucle para llenar los espacios Primer bloque reservado (puntero)*/
  printf("construcción y calculo de la secuencia de los primeros 10 terminos \n");
  for(int i=0;i<10;i++){  
    *(puntero+i) = i; 
    printf("El valor de la secuencia es %d\n\n", *(puntero+i));
  }

/*LIBERACIÓN O RETORNO DE MEMORIA*/
free(puntero); // Liberación de memoria del bloque de 10 int  
free(p1); // Liberación de memoria del bloque de 4 int  
free(p3); // Liberación de memoria del bloque de 5  
  return 0;
}