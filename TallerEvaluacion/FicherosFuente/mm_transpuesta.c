/*##################################################################################################
#                                       Autor: Edward Quintero                                     #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                              Tema: Taller de Evaluación de Rendimiento                           #
#                     Fichero: fuente de multiplicación de matrices NxN por hilos.                 #
#   Objetivo: Evaluar el tiempo de ejecución del algoritmo clásico de multiplicación de matrices.  #
####################################################################################################*/

/*
 * Este programa en C realiza la multiplicación de matrices transpuestas utilizando múltiples hilos para 
 * aprovechar el paralelismo y optimizar el rendimiento en el cálculo. El programa distribuye 
 * el trabajo entre varios hilos, donde cada uno procesa una sección específica de la matriz A
 * multiplicada por la matriz B y almacena el resultado en la matriz C.
 *
 * La memoria compartida se utiliza para asignar espacio para las matrices, facilitando el 
 * acceso a los datos entre los hilos. Un mutex asegura la sincronización en ciertas secciones 
 * críticas, aunque la mayoría del trabajo de cada hilo es independiente, ya que los cálculos
 * de las posiciones en la matriz de resultado no se superponen.
 *
 * El tamaño de la matriz y el número de hilos se ingresan como argumentos en la línea de 
 * comandos, y el programa mide el tiempo total de ejecución de la multiplicación, mostrando 
 * los resultados y el rendimiento alcanzado.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3)   // Definición del tamaño total del bloque de memoria compartida

// Declaración de mutex y matrices en memoria compartida
pthread_mutex_t MM_mutex;            // Mutex para la sincronización entre hilos
static double MEM_CHUNK[DATA_SIZE];  // Bloque de memoria compartida para matrices
double *mA, *mB, *mC;                // Punteros a matrices A, B y C

// Estructura para parámetros de hilos
struct parametros {
	int nH;     // Número total de hilos
	int idH;    // ID del hilo actual
	int N;      // Tamaño de la matriz
};

// Variables de tiempo para medir el rendimiento
struct timeval start, stop;

// Función para llenar las matrices A y B
void llenar_matriz(int SZ){ 
	srand48(time(NULL));  // Inicializa la semilla de números aleatorios
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1 * i;  // Llena matriz A
		mB[i] = 2.2 * i;  // Llena matriz B
		mC[i] = 0;        // Inicializa matriz C en cero
	}	
}

// Función para imprimir una matriz si su tamaño es pequeño
void print_matrix(int sz, double *matriz){
	if(sz < 12){  // Imprime solo matrices de tamaño menor a 12x12
    	for(int i = 0; i < sz*sz; i++){
     		if(i % sz == 0) printf("\n");
        	printf(" %.3f ", matriz[i]);
		}	
    	printf("\n>-------------------->\n");
	}
}

// Función para iniciar el tiempo de ejecución
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

// Función para finalizar el tiempo de ejecución y mostrarlo
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec;
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec * 1000000 + stop.tv_usec));
}

/*
 * Función de multiplicación de matrices por hilos
 * Cada hilo multiplica una sección de la matriz A por la matriz B y almacena el resultado en C.
*/
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables;
	
	int idH = data->idH;    // ID del hilo
	int nH  = data->nH;     // Número total de hilos
	int N   = data->N;      // Tamaño de la matriz

	// Rango de filas que este hilo debe procesar
	int ini = (N / nH) * idH;
	int fin = (N / nH) * (idH + 1);

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i * N);   // Apunta a la fila i de A
			pB = mB + (j * N);   // Apunta a la columna j de B
            for (int k = 0; k < N; k++, pA++, pB++){
				sumaTemp += (*pA * *pB);  // Acumula el producto
			}
			mC[i * N + j] = sumaTemp;   // Guarda el resultado en la posición de C
		}
	}

	// Bloqueo y desbloqueo del mutex para asegurar sincronización al finalizar
	pthread_mutex_lock(&MM_mutex);
	pthread_mutex_unlock(&MM_mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}

    int SZ = atoi(argv[1]);         // Tamaño de la matriz (N x N)
    int n_threads = atoi(argv[2]);  // Número de hilos a usar

    pthread_t p[n_threads];         // Vector para hilos
    pthread_attr_t atrMM;           // Atributos de los hilos

	// Asigna las matrices A, B y C en la memoria compartida MEM_CHUNK
	mA = MEM_CHUNK;
	mB = mA + SZ * SZ;
	mC = mB + SZ * SZ;

	llenar_matriz(SZ);               // Llena las matrices A y B
	print_matrix(SZ, mA);            // Imprime matriz A (si es pequeña)
	print_matrix(SZ, mB);            // Imprime matriz B (si es pequeña)

	inicial_tiempo();                // Comienza la medición de tiempo

	pthread_mutex_init(&MM_mutex, NULL);   // Inicializa el mutex
	pthread_attr_init(&atrMM);             // Inicializa los atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Crea los hilos para la multiplicación
    for (int j = 0; j < n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos);
	}

    // Espera a que todos los hilos terminen
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);
	final_tiempo();                // Finaliza la medición de tiempo
	
	print_matrix(SZ, mC);          // Imprime matriz C (resultado) si es pequeña

	pthread_attr_destroy(&atrMM);  // Destruye los atributos de los hilos
	pthread_mutex_destroy(&MM_mutex);  // Destruye el mutex
	pthread_exit(NULL);            // Finaliza el hilo principal
}
