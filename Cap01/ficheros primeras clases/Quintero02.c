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
  int n1, n2;
  printf("digite el pimer entero entero: \n");
  scanf("%d",&n1);
  printf("digite el segundo entero: \n");
  scanf("%d",&n2);
  int suma = n1 + n2;
  printf("El resultado es: %d\n",suma);
}



void areaCircunferencia(){
      int radio;
      float pi = 3.14;
      printf("digite el radio: \n");
      scanf("%d",&radio);
      float area = radio * radio * pi;
      printf("El area es: %f\n",area);
 }


int main (int argc, char *argv[]){
  
  bienvenida();
  sumaDosEnteros();
  areaCircunferencia();
  return 0; 
}

