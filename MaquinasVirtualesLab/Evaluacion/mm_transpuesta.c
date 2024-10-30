/*##################################################################################################
#                                       Autor: Edward Quintero                                     #
#                                         Fecha: 29/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                              Tema: laboratorio de Evaluación de Rendimiento                      #
#                             Fichero: transpuesta de matrices NxN por hilos.                      #
#   Objetivo: Evaluar el tiempo de ejecución del algoritmo clásico de transpuesta de matrices.     #
####################################################################################################*/

/*
 * Este programa realiza la multiplicación de dos matrices cuadradas en paralelo utilizando hilos.
 * Cada hilo multiplica una sección de la matriz A por la matriz B y guarda los resultados en la matriz C.
 * Las matrices son almacenadas en un bloque de memoria compartida, y se utiliza un mutex para la
 * sincronización entre hilos. Además, el tiempo total de ejecución se mide para evaluar el rendimiento.
 */

#include <stdio.h>       // Librería estándar para funciones de entrada/salida
#include <pthread.h>     // Librería para la creación y manejo de hilos
#include <unistd.h>      // Librería para llamadas al sistema y constantes POSIX
#include <stdlib.h>      // Librería para la conversión de datos y generación de números aleatorios
#include <time.h>        // Librería para obtener el tiempo del sistema
#include <sys/time.h>    // Librería para medición de tiempo de ejecución

#define DATA_SIZE (1024*1024*64*3)  // Define el tamaño total de la memoria compartida para matrices

// Declaración de un mutex y matrices compartidas entre hilos
pthread_mutex_t MM_mutex;            // Mutex para sincronizar hilos durante la ejecución
static double MEM_CHUNK[DATA_SIZE];  // Bloque de memoria compartida para almacenar matrices
double *mA, *mB, *mC;                // Punteros a matrices A, B y C dentro del bloque MEM_CHUNK

// Estructura de parámetros utilizada para configurar los hilos
struct parametros {
	int nH;     // Número total de hilos
	int idH;    // ID del hilo actual
	int N;      // Dimensión de la matriz cuadrada (NxN)
};

// Variables para almacenar el tiempo inicial y final
struct timeval start, stop;

// Llena las matrices A y B con valores secuenciales
void llenar_matriz(int SZ){ 
	srand48(time(NULL));         // Inicializa la semilla para números aleatorios con el tiempo actual
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1 * i;         // Llena la matriz A con valores generados
		mB[i] = 2.2 * i;         // Llena la matriz B con valores generados
		mC[i] = 0;               // Inicializa todos los elementos de C en 0
	}	
}

// Imprime la matriz si su tamaño es pequeño (para facilitar la visualización)
void print_matrix(int sz, double *matriz){
	if(sz < 12){                 // Solo imprime matrices de tamaño menor a 12x12
    	for(int i = 0; i < sz*sz; i++){
     		if(i % sz == 0) printf("\n");  // Salto de línea al final de cada fila
        	printf(" %.3f ", matriz[i]);   // Imprime cada elemento con 3 decimales
		}	
    	printf("\n>-------------------->\n");  // Marca el final de la matriz impresa
	}
}

// Captura el tiempo inicial antes de comenzar la operación
void inicial_tiempo(){
	gettimeofday(&start, NULL);  // Asigna el tiempo actual a la variable 'start'
}

// Calcula y muestra el tiempo transcurrido desde el inicio de la operación
void final_tiempo(){
	gettimeofday(&stop, NULL);   // Asigna el tiempo actual a la variable 'stop'
	stop.tv_sec -= start.tv_sec; // Calcula la diferencia de tiempo en segundos
	printf("\n:-> %9.0f µs\n", (double)(stop.tv_sec * 1000000 + stop.tv_usec));  // Muestra el tiempo en microsegundos
}

/*
 * Función ejecutada por cada hilo para realizar la multiplicación parcial de matrices
 * Cada hilo multiplica una sección de la matriz A por la matriz B y acumula el resultado en C.
*/
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables;  // Convierte y asigna los parámetros de entrada
	
	int idH = data->idH;        // ID único del hilo actual
	int nH  = data->nH;         // Total de hilos en ejecución
	int N   = data->N;          // Dimensión de la matriz cuadrada

	// Calcula el rango de filas de la matriz A que este hilo procesará
	int ini = (N / nH) * idH;   // Índice inicial de la fila
	int fin = (N / nH) * (idH + 1); // Índice final de la fila (exclusivo)

    for (int i = ini; i < fin; i++){ // Recorre las filas asignadas al hilo
        for (int j = 0; j < N; j++){ // Recorre cada columna en la fila
			double *pA, *pB, sumaTemp = 0.0;  // Inicializa los punteros y la suma temporal
			pA = mA + (i * N);    // Puntero a la posición inicial de la fila i en A
			pB = mB + (j * N);    // Puntero a la posición inicial de la columna j en B
            for (int k = 0; k < N; k++, pA++, pB++){  // Recorre la fila y columna simultáneamente
				sumaTemp += (*pA * *pB);  // Acumula el producto de los elementos actuales
			}
			mC[i * N + j] = sumaTemp;  // Guarda el resultado acumulado en la posición correspondiente de C
		}
	}

	// Bloquea y desbloquea el mutex para sincronización antes de salir
	pthread_mutex_lock(&MM_mutex);
	pthread_mutex_unlock(&MM_mutex);
	pthread_exit(NULL); // Termina la ejecución del hilo
}

// Función principal que organiza la configuración y ejecución de la multiplicación de matrices
int main(int argc, char *argv[]){
	if (argc < 3){  // Verifica que se hayan ingresado los argumentos necesarios
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;  // Termina si no hay suficientes argumentos
	}

    int SZ = atoi(argv[1]);         // Convierte el argumento de tamaño de matriz a entero
    int n_threads = atoi(argv[2]);  // Convierte el argumento de número de hilos a entero

    pthread_t p[n_threads];         // Arreglo de hilos
    pthread_attr_t atrMM;           // Atributos de los hilos

	// Asigna las matrices A, B y C dentro del bloque de memoria compartida MEM_CHUNK
	mA = MEM_CHUNK;                  // mA apunta al inicio de MEM_CHUNK
	mB = mA + SZ * SZ;               // mB comienza justo después de mA
	mC = mB + SZ * SZ;               // mC comienza después de mB

	llenar_matriz(SZ);               // Llama a la función para llenar las matrices A y B con valores
	print_matrix(SZ, mA);            // Imprime matriz A si es lo suficientemente pequeña
	print_matrix(SZ, mB);            // Imprime matriz B si es lo suficientemente pequeña

	inicial_tiempo();                // Inicia la medición del tiempo de ejecución

	pthread_mutex_init(&MM_mutex, NULL);   // Inicializa el mutex para los hilos
	pthread_attr_init(&atrMM);             // Inicializa los atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);  // Configura los hilos como unibles

    // Crea cada hilo y le pasa su conjunto de parámetros
    for (int j = 0; j < n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Reserva memoria para parámetros del hilo
		datos->idH = j;              // Asigna ID al hilo actual
		datos->nH  = n_threads;      // Asigna el número total de hilos
		datos->N   = SZ;             // Asigna el tamaño de la matriz
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea el hilo con sus atributos y función
	}

    // Espera a que todos los hilos terminen antes de continuar
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);   // Espera que cada hilo complete su tarea
	final_tiempo();                // Finaliza la medición del tiempo de ejecución
	
	print_matrix(SZ, mC);          // Imprime matriz C (resultado) si es lo suficientemente pequeña

	pthread_attr_destroy(&atrMM);  // Libera los recursos de los atributos de los hilos
	pthread_mutex_destroy(&MM_mutex);  // Libera los recursos del mutex
	pthread_exit(NULL);            // Finaliza la ejecución del hilo principal
}
