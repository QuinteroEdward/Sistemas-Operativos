/*############################################################################################                                                                                            
#                                    Autor: Edward Andres Quintero                           #                                                                                            
#                                         Fecha: 30/07/2024                                  #                                                                                            
#                                    Materia: Sistemas Operativos                            #                                                                                            
#                                  Tema: Argumentos de entrada en C                          #                                                                                            
#                                                                                            #                                                                                            
##############################################################################################*/
/*se crea una tabla que contenga todos los tipos de datos que existen en C*/
#include <stdio.h>

int main(int argc, char *argv[]){
  
 /* int entero = 7;
  float flotante= 4.67567;
  double doble = 3.54;
  bool boleano = false;
  char caracter = 'E';
  char cadena[100] = "cadena de caracteres";
  
  printf("////////////////////////////// \n")
  printf("/tipo de dato entero:    %d  / \n")
  printf("/tipo de dato flotante:  %d  / \n")
  printf("/tipo de dato doble:     %d  / \n")
  printf("/tipo de dato boleano:   %d  / \n")
  printf("/tipo de dato caracter:  %d  / \n")
  printf("/tipo de dato entero:    %d  / \n") */
    
    printf("+-----------------+---------------------+-----------------------+\n");
    printf("| Tipo de dato    | Ejemplo de valor    | Tamaño (en bytes)     |\n");
    printf("+-----------------+---------------------+-----------------------+\n");

    // Tipo de dato char
    printf("| %-15s | %-19c | %-21lu |\n", "char", 'A', sizeof(char));

    // Tipo de dato int
    printf("| %-15s | %-19d | %-21lu |\n", "int", 123456, sizeof(int));

    // Tipo de dato float
    printf("| %-15s | %-19.2f | %-21lu |\n", "float", 3.14f, sizeof(float));

    // Tipo de dato double
    printf("| %-15s | %-19.2f | %-21lu |\n", "double", 3.141592653589793, sizeof(double));

    // Tipo de dato long
    printf("| %-15s | %-19ld | %-21lu |\n", "long", 123456789L, sizeof(long));

    // Tipo de dato long long
    printf("| %-15s | %-19lld | %-21lu |\n", "long long", 123456789012345LL, sizeof(long long));

    printf("+-----------------+---------------------+-----------------------+\n");
  
  return 0;
}

