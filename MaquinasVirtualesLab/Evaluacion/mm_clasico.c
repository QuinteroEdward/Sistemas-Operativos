/*##################################################################################################
#                                       Autor: Edward Quintero                                     #
#                                         Fecha: 29/10/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                              Tema: Laboratorio de Evaluación de Rendimiento                      #
#                     	    Fichero: multiplicación de matrices NxN por hilos.                     #
#   Objetivo: Evaluar el tiempo de ejecución del algoritmo clásico de multiplicación de matrices.  #
####################################################################################################*/


/*
 * Programa en C para realizar la multiplicación de matrices cuadradas grandes utilizando múltiples hilos para 
 * dividir el trabajo. Cada hilo multiplica una sección de la matriz A por la matriz B y almacena el resultado en C.
 * El tamaño de la matriz y el número de hilos son parámetros que se ingresan al ejecutar el programa.
 *
 * Funcionamiento:
 * - Asigna bloques de memoria contiguos para las matrices A, B y C.
 * - Llena las matrices A y B con valores iniciales y crea n hilos para realizar la multiplicación.
 * - Cada hilo procesa una sección específica de filas de la matriz A, multiplicándolas por columnas de la matriz B.
 * - Al finalizar el procesamiento, el programa muestra la matriz resultante C si es lo suficientemente pequeña.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3)   // Tamaño del bloque de memoria para almacenar las matrices

// Variables de sincronización y memoria compartida
pthread_mutex_t MM_mutex;             // Mutex para controlar el acceso concurrente
static double MEM_CHUNK[DATA_SIZE];   // Espacio en memoria compartida para las matrices
double *mA, *mB, *mC;                 // Punteros a matrices A, B y C dentro de MEM_CHUNK

// Estructura para pasar parámetros a cada hilo
struct parametros {
	int nH;     // Número total de hilos
	int idH;    // ID del hilo
	int N;      // Dimensión de la matriz N x N
};

// Variables para medir el tiempo de ejecución
struct timeval start, stop;

// Función para inicializar las matrices A y B con valores predeterminados
void llenar_matriz(int SZ){ 
	srand48(time(NULL));                      // Inicializa la semilla para generar valores
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1 * i;                      // Asigna valores a la matriz A
		mB[i] = 2.2 * i;                      // Asigna valores a la matriz B
		mC[i] = 0;                            // Inicializa la matriz C en 0
	}	
}

// Función para imprimir una matriz de forma limitada si su tamaño es pequeño
void print_matrix(int sz, double *matriz){
	if(sz < 12){                              // Solo imprime si la matriz es pequeña (tamaño < 12x12)
    	for(int i = 0; i < sz*sz; i++){
     		if(i % sz == 0) printf("\n");      // Nueva línea al inicio de cada fila
        	printf(" %.3f ", matriz[i]);       // Imprime cada elemento con 3 decimales
		}	
    	printf("\n>-------------------->\n");
	}
}

// Función para registrar el tiempo de inicio de ejecución
void inicial_tiempo(){
	gettimeofday(&start, NULL);               // Obtiene y guarda el tiempo actual
}

// Función para calcular y mostrar el tiempo de ejecución en microsegundos
void final_tiempo(){
	gettimeofday(&stop, NULL);                // Obtiene el tiempo actual
	stop.tv_sec -= start.tv_sec;              // Calcula la diferencia en segundos
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec * 1000000 + stop.tv_usec)); // Muestra el tiempo en µs
}

/*
 * Función que ejecutan los hilos para realizar la multiplicación de matrices.
 * Cada hilo recibe una estructura de parámetros y procesa un subconjunto de filas.
 */
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables; // Convierte los parámetros a la estructura definida
	
	int idH = data->idH;       // ID del hilo
	int nH  = data->nH;        // Número total de hilos
	int N   = data->N;         // Dimensión de la matriz

	// Cálculo del rango de filas que el hilo debe procesar
	int ini = (N / nH) * idH;              // Índice inicial
	int fin = (N / nH) * (idH + 1);        // Índice final

    for (int i = ini; i < fin; i++){       // Bucle sobre las filas asignadas al hilo
        for (int j = 0; j < N; j++){       // Bucle sobre cada columna de B
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i * N);             // Puntero a la fila i de A
			pB = mB + j;                   // Puntero a la columna j de B
            for (int k = 0; k < N; k++, pA++, pB += N){
				sumaTemp += (*pA * *pB);   // Acumula el producto de elementos
			}
			mC[i * N + j] = sumaTemp;      // Guarda el resultado en la matriz C
		}
	}

	// Bloquea y desbloquea el mutex para evitar conflictos al salir
	pthread_mutex_lock(&MM_mutex);
	pthread_mutex_unlock(&MM_mutex);
	pthread_exit(NULL);                    // Finaliza el hilo
}

int main(int argc, char *argv[]){
	if (argc < 2){                         // Comprueba que haya argumentos suficientes
		printf("Uso: $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
    
    int SZ = atoi(argv[1]);                // Tamaño de la matriz cuadrada (N x N)
    int n_threads = atoi(argv[2]);         // Número de hilos

    pthread_t p[n_threads];                // Arreglo para los hilos
    pthread_attr_t atrMM;                  // Atributos de los hilos

    // Asigna espacio dentro de MEM_CHUNK para matrices A, B y C
	mA = MEM_CHUNK;                        
	mB = mA + SZ * SZ;                     // mB comienza después de mA
	mC = mB + SZ * SZ;                     // mC comienza después de mB

	llenar_matriz(SZ);                     // Inicializa las matrices A y B
	print_matrix(SZ, mA);                  // Muestra la matriz A si es pequeña
	print_matrix(SZ, mB);                  // Muestra la matriz B si es pequeña

	inicial_tiempo();                      // Inicia la medición de tiempo

	pthread_mutex_init(&MM_mutex, NULL);   // Inicializa el mutex
	pthread_attr_init(&atrMM);             // Inicializa los atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Crea los hilos para realizar la multiplicación de matrices
    for (int j = 0; j < n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;                    // Asigna el ID del hilo
		datos->nH  = n_threads;            // Asigna el número total de hilos
		datos->N   = SZ;                   // Asigna el tamaño de la matriz
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea el hilo
	}

    // Espera a que todos los hilos terminen
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);          // Espera la finalización del hilo j

	final_tiempo();                        // Finaliza la medición de tiempo
	
	print_matrix(SZ, mC);                  // Muestra la matriz C resultante si es pequeña

	pthread_attr_destroy(&atrMM);          // Libera los recursos de los atributos de hilo
	pthread_mutex_destroy(&MM_mutex);      // Libera el mutex
	pthread_exit(NULL);                    // Termina el hilo principal
}
