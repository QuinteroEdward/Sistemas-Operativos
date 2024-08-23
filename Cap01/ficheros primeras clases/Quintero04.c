/*Autor: E. Quintero
Fecha: 30/07/2024
Materia: Sistemas Operativos
Tema: Argumentos de entrada en C

*/
/*Al inicio debe enviar el programa con el valor de la edad*/
/* Se presentara una serie de preguntas que el usuario debe responder */

#include <stdio.h>

int main(int argc, char *argv[]){

    printf("\n Ha ingresado [%d]: \n\n", argc);
    printf("Ha ingresado la edad: %s\n\n", argv[1]);
    printf("Ha ingresado el mes: %s \n\n", argv[2]);
    printf("Ha ingresado el nombre: %s \n\n", argv[3]);

  
  int vector[] = {5,10,20,30,40,50,80};
  int *p1 = vector;
  int *p2 = vector+20;
  int diferencia = p2 - p1;
  
  printf("\n El valor p1 es de: %p \n",p1);  
  printf("\n El valor p2 es de: %p \n",p2);
  printf("\n El valor de la resta es de: %d \n",diferencia);
 
  return 0;
}

