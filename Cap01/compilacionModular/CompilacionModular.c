/*##################################################################################################
#                                      Autores: Edward Quintero                                    #
#                                         Fecha: 17/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                                     Tema: Compilación modular                                    #
#                                   Apertura de ficheros y valores                                 #
####################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
    Descripción del programa:
    Este programa lee dos arreglos de enteros desde dos archivos diferentes,
    calcula la suma modular de cada uno mediante procesos separados,
    y luego calcula la suma modular total de ambos resultados usando comunicación 
    a través de pipes. Finalmente, muestra los resultados en la consola.

    Parámetros:
    - tamaño1: Cantidad de enteros a leer desde el primer archivo.
    - archivoA: Nombre del archivo que contiene el primer conjunto de enteros.
    - tamaño2: Cantidad de enteros a leer desde el segundo archivo.
    - archivoB: Nombre del archivo que contiene el segundo conjunto de enteros.
    - mod: Valor del módulo para calcular la suma modular.
*/

// Función para leer un archivo y almacenar los enteros en un arreglo dinámico
int *leer_datos_archivo(const char *nombre_archivo, int num_elementos) {
    FILE *archivo = fopen(nombre_archivo, "r");  // Abre el archivo en modo lectura
    if (!archivo) {  // Verifica si el archivo se pudo abrir correctamente
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Reserva memoria dinámica para el arreglo
    int *arreglo = (int *)malloc(num_elementos * sizeof(int));
    if (!arreglo) {  // Si falla la reserva de memoria
        perror("Error al asignar memoria para el arreglo");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    // Lee los elementos del archivo y los almacena en el arreglo
    for (int i = 0; i < num_elementos; i++) {
        if (fscanf(archivo, "%d", &arreglo[i]) != 1) {
            perror("Error al leer los datos del archivo");
            free(arreglo);  // Libera la memoria si hay un error
            fclose(archivo);
            exit(EXIT_FAILURE);
        }
    }

    fclose(archivo);  // Cierra el archivo después de leer los datos
    return arreglo;  // Devuelve el puntero al arreglo cargado
}

// Función para calcular la suma modular de un arreglo
int suma_modular(int *arreglo, int cantidad_elementos, int modulo) {
    int resultado_suma = 0;
    // Itera sobre cada elemento del arreglo y realiza la suma modular
    for (int i = 0; i < cantidad_elementos; i++) {
        resultado_suma = (resultado_suma + arreglo[i]) % modulo;  // Aplica suma modular
    }
    return resultado_suma;  // Devuelve el valor de la suma modular
}

int main(int argc, char *argv[]) {
    if (argc != 6) {  // Verifica que se hayan pasado los argumentos correctos
        fprintf(stderr, "Uso: %s tamaño1 archivoA tamaño2 archivoB mod\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Procesa los argumentos del programa
    int tam_arreglo1 = atoi(argv[1]);  // Obtiene el tamaño del primer arreglo
    char *nombre_archivo1 = argv[2];  // Obtiene el nombre del primer archivo
    int tam_arreglo2 = atoi(argv[3]);  // Obtiene el tamaño del segundo arreglo
    char *nombre_archivo2 = argv[4];  // Obtiene el nombre del segundo archivo
    int valor_mod = atoi(argv[5]);  // Obtiene el valor del módulo para la suma modular

    // Lee los datos de los archivos y los almacena en arreglos
    int *datos_arreglo1 = leer_datos_archivo(nombre_archivo1, tam_arreglo1);
    int *datos_arreglo2 = leer_datos_archivo(nombre_archivo2, tam_arreglo2);

    // Crear pipes para la comunicación entre los procesos padre e hijos
    int canal_arreglo1[2], canal_arreglo2[2];
    if (pipe(canal_arreglo1) == -1 || pipe(canal_arreglo2) == -1) {
        perror("Error al crear los canales de comunicación");
        exit(EXIT_FAILURE);
    }

    pid_t proceso_hijo1, proceso_hijo2;

    // Primer proceso hijo: calcula la suma modular del primer arreglo
    if ((proceso_hijo1 = fork()) == 0) {
        close(canal_arreglo1[0]);  // Cierra el lado de lectura del pipe
        int resultado_modular1 = suma_modular(datos_arreglo1, tam_arreglo1, valor_mod);
        write(canal_arreglo1[1], &resultado_modular1, sizeof(resultado_modular1));  // Envía el resultado al padre
        close(canal_arreglo1[1]);  // Cierra el lado de escritura del pipe después de usarlo
        exit(0);  // Termina el proceso hijo
    } else if (proceso_hijo1 < 0) {
        perror("Error en la creación del proceso hijo");
        exit(EXIT_FAILURE);
    }

    // Segundo proceso hijo: calcula la suma modular del segundo arreglo
    if ((proceso_hijo2 = fork()) == 0) {
        close(canal_arreglo2[0]);  // Cierra el lado de lectura del pipe
        int resultado_modular2 = suma_modular(datos_arreglo2, tam_arreglo2, valor_mod);
        write(canal_arreglo2[1], &resultado_modular2, sizeof(resultado_modular2));  // Envía el resultado al padre
        close(canal_arreglo2[1]);  // Cierra el lado de escritura del pipe después de usarlo
        exit(0);  // Termina el proceso hijo
    } else if (proceso_hijo2 < 0) {
        perror("Error en la creación del proceso hijo");
        exit(EXIT_FAILURE);
    }

    // Proceso padre: espera los resultados de ambos hijos
    int resultado_modular1, resultado_modular2;
    close(canal_arreglo1[1]);  // Cierra el lado de escritura del pipe
    close(canal_arreglo2[1]);

    // Lee los resultados de los hijos
    read(canal_arreglo1[0], &resultado_modular1, sizeof(resultado_modular1));
    read(canal_arreglo2[0], &resultado_modular2, sizeof(resultado_modular2));

    close(canal_arreglo1[0]);  // Cierra el lado de lectura del pipe
    close(canal_arreglo2[0]);

    // Calcula la suma modular total
    int suma_total_modular = (resultado_modular1 + resultado_modular2) % valor_mod;

    // Imprime los resultados de las sumas modulares
    printf("Suma modular del primer archivo: %d\n", resultado_modular1);
    printf("Suma modular del segundo archivo: %d\n", resultado_modular2);
    printf("Suma total modular: %d\n", suma_total_modular);

    // Espera a que los procesos hijos terminen
    wait(NULL);
    wait(NULL);

    // Libera la memoria asignada dinámicamente
    free(datos_arreglo1);
    free(datos_arreglo2);

    return 0;  // Fin del programa
}

