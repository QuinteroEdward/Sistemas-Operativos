/*
Autor: E. Quintero
Fecha: 13/08/2024
Materia: Sistemas Operativos
Tema: Gestión de memoria 
Tópico: Reserva de memoria con la función memalloc().
----------------------------------------------------------------------------------------
la función memalloc() reserva un espacio de memoria de tamaño requerido y 
devuelve un puntero que señala el inicio del espacio de memoria reservado anteriormente.
*/

#include <stdio.h>
#include <stdlib.h>

int main(){
/* se declaran las variables y punteros necesarios*/
int *bloque00 = malloc(4*sizeof(int)); // Asignación de vector de 4 tamaño int 
int *puntero = malloc(15*sizeof(*puntero)); // Bloque de memoria de 15 int

/*Bucle para llenar los espacios Primer cloque reservado (Bloque00)*/
for(int i=0; i<4; i++){
  bloque00[i] = i*2;
  printf("El valor de memoria reservada de Bloque00[%d] = %d\n\n", i, bloque00[i]);
}

/*Bucle para llenar los espacios Primer bloque reservado (puntero)*/
  if(puntero!= NULL){
    *(puntero+8) = 9889; 
    printf("El valor de la 9na posición de puntero[%d] = %d\n\n", *(puntero+8));
    printf("El valor de la 9na posición de puntero[%d] = %d\n\n", puntero[8]);
  }

/*LIBERACIÓN O RETORNO DE MEMORIA*/
free(bloque00); // Liberación de memoria del vector de 4 int
free(puntero); // Liberación de memoria del bloque de 15 int  
  return 0;
}