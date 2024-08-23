/*############################################################################################                                                                                            
#                                    Autor: Edward Andres Quintero                           #                                                                                            
#                                         Fecha: 30/07/2024                                  #                                                                                            
#                                    Materia: Sistemas Operativos                            #                                                                                            
#                                  Tema: Argumentos de entrada en C                          #                                                                                            
#                                                                                            #                                                                                            
##############################################################################################*/
/*Al inicio debe enviar el programa con el valor de la edad*/
/* Se presentara una serie de preguntas que el usuario debe responder */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

int main(int argc, char *argv[]){

    printf("\n Ha ingresado [%d]: \n\n", argc);
    printf("Ha ingresado la edad: %s\n\n", argv[1]);
    printf("Ha ingresado el mes: %s \n\n", argv[2]);
    printf("Ha ingresado el nombre: %s \n\n", argv[3]);


    int nacimiento;
    int dia;
    char colorFav[100];
    char comidaFav[100];
    int altura;

  
    printf("¿Cuál es el año de nacimiento?: ");
    scanf("%d", &nacimiento);
    printf("\n Ha ingresado el año de nacimiento: %d \n\n", nacimiento);

  
    printf("¿Cuál es el día de nacimiento?: ");
    scanf("%d", &dia);
    printf("\n Ha ingresado el día de nacimiento: %d \n\n", dia);

   
    printf("¿Cuál es el color favorito?: ");
    scanf("%s", colorFav); 
    printf("\n Ha ingresado el color favorito: %s \n\n", colorFav);

   
    printf("¿Cuál es la comida favorita?: ");
    scanf("%s", comidaFav); 
    printf("\n Ha ingresado la comida favorita: %s \n\n", comidaFav);

   
    printf("¿Cuál es su altura?: ");
    scanf("%d", &altura);
    printf("\n Ha ingresado la altura: %d \n\n", altura);

  return 0;
}

