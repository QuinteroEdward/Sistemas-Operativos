/*##################################################################################################
#                                       Autor: Edward Quintero                                     #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                              Tema: Taller de Evaluación de Rendimiento                           #
#                     Fichero: fuente de multiplicación de matrices NxN por hilos.                 #                                             #
####################################################################################################*/

/*
 * Este programa realiza la multiplicación de matrices en paralelo utilizando hilos (threads).
 * Divide el cálculo de la matriz resultante en segmentos, asignando a cada hilo una porción de 
 * filas de la matriz de salida. Cada hilo calcula su segmento de forma independiente, optimizando
 * el rendimiento en sistemas de múltiples núcleos.
 * 
 * Las matrices se almacenan en un bloque de memoria compartida para facilitar el acceso y reducir
 * la sobrecarga de gestión de memoria. Un mutex se utiliza para controlar el acceso a secciones 
 * críticas, aunque en este caso es mínimo debido a la naturaleza paralela de las operaciones.
 * 
 * El programa también mide el tiempo de ejecución de la multiplicación de matrices y muestra
 * matrices pequeñas en la salida para verificación. El tamaño de la matriz y el número de hilos
 * se ingresan como argumentos al ejecutar el programa.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3) // Definición del tamaño máximo de datos en memoria para las matrices

pthread_mutex_t MM_mutex; // Mutex para control de sincronización entre hilos
static double MEM_CHUNK[DATA_SIZE]; // Bloque de memoria para las matrices
double *mA, *mB, *mC; // Punteros a matrices A, B, y C

// Estructura para almacenar los parámetros de cada hilo
struct parametros{
	int nH;   // Número total de hilos
	int idH;  // ID del hilo
	int N;    // Tamaño de la matriz
};

struct timeval start, stop; // Variables para medir el tiempo de ejecución

// Función para llenar matrices A y B con valores iniciales
void llenar_matriz(int SZ){ 
	srand48(time(NULL)); // Inicializar la semilla del generador de números aleatorios
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1 * i; // Llenar mA y mB con valores secuenciales
		mB[i] = 2.2 * i;
		mC[i] = 0; // Inicializar mC en cero
	}
}

// Función para imprimir matrices si son de tamaño pequeño
void print_matrix(int sz, double *matriz){
	if(sz < 12){ // Solo imprime matrices pequeñas para evitar saturación en pantalla
		for(int i = 0; i < sz*sz; i++){
			if(i % sz == 0) printf("\n"); // Nueva línea por cada fila
			printf(" %.3f ", matriz[i]); // Imprimir valor con tres decimales
		}
	}
	printf("\n>-------------------->\n");
}

// Función para registrar el tiempo de inicio
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

// Función para calcular y mostrar el tiempo transcurrido desde el inicio
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec; // Diferencia de segundos
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Mostrar tiempo en microsegundos
}

// Función de multiplicación de matrices, ejecutada por cada hilo
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables;
	
	int idH = data->idH;   // ID del hilo actual
	int nH  = data->nH;    // Número total de hilos
	int N   = data->N;     // Tamaño de la matriz
	int ini = (N / nH) * idH; // Índice inicial de filas para este hilo
	int fin = (N / nH) * (idH + 1); // Índice final de filas para este hilo

	// Bucle de multiplicación de matrices
	for (int i = ini; i < fin; i++){
		for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0; // Variables auxiliares
			pA = mA + (i * N); // Puntero a la fila i de mA
			pB = mB + j;       // Puntero a la columna j de mB
			for (int k = 0; k < N; k++, pA++, pB += N){
				sumaTemp += (*pA * *pB); // Suma temporal para el producto
			}
			mC[i * N + j] = sumaTemp; // Asignar el resultado a mC
		}
	}

	// Sincronización del hilo con el mutex
	pthread_mutex_lock(&MM_mutex);
	pthread_mutex_unlock(&MM_mutex);
	pthread_exit(NULL); // Finalización del hilo
}

int main(int argc, char *argv[]){
	// Verificar que se ingresaron argumentos para el tamaño de la matriz y el número de hilos
	if (argc < 2){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}

	int SZ = atoi(argv[1]); // Tamaño de la matriz ingresado por el usuario
	int n_threads = atoi(argv[2]); // Número de hilos ingresado por el usuario

	pthread_t p[n_threads]; // Declaración de hilos
	pthread_attr_t atrMM;   // Atributos de hilos

	// Inicialización de punteros de matrices dentro del bloque de memoria compartida
	mA = MEM_CHUNK;
	mB = mA + SZ * SZ;
	mC = mB + SZ * SZ;

	// Llenar las matrices con valores iniciales y mostrar
	llenar_matriz(SZ);
	print_matrix(SZ, mA);
	print_matrix(SZ, mB);

	inicial_tiempo(); // Inicio del cronómetro
	pthread_mutex_init(&MM_mutex, NULL); // Inicialización del mutex
	pthread_attr_init(&atrMM); // Inicialización de atributos de hilo
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

	// Crear hilos para realizar la multiplicación en paralelo
	for (int j = 0; j < n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;        // ID del hilo
		datos->nH = n_threads; // Número de hilos
		datos->N = SZ;         // Tamaño de la matriz
		pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crear hilo
	}

	// Esperar a que todos los hilos terminen su ejecución
	for (int j = 0; j < n_threads; j++)
		pthread_join(p[j], NULL);

	final_tiempo(); // Mostrar tiempo transcurrido

	print_matrix(SZ, mC); // Imprimir la matriz resultado

	// Liberación de recursos
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit(NULL); // Finalización del programa
}
