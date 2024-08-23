/*Autor: E. Quintero
Fecha: 23/07/2024
Materia: Sistemas Operativos
Tema: Programación en C : Primera parte
Requerimientos:
    -Función que sume 2 enteros
    -Función que calcule el area de una Circunferencia
    -Función que de la bienvenida 
*/

#include <stdio.h>

void bienvenida(){
    
  printf("Hola Mundo........\n");
  printf("\t\t Bienvenidos a mi clase de Sistemas Operativos :) \n");

}

void sumaDosEnteros(){
  printf("primer entero es: 5 \n");
  printf("segundo entero es: 16 \n");
  int suma = 5 + 16;
  printf("El resultado es: %d\n",suma);
}



void areaCircunferencia(){
      int radio = 5;
      float pi = 3.14;
      printf("El radio es: %d\n",radio);
      float area = radio * radio * pi;
      printf("El area es: %f\n",area);
 }


int main (int argc, char *argv[]){
  
  bienvenida();
  sumaDosEnteros();
  areaCircunferencia();
  return 0; 
}

