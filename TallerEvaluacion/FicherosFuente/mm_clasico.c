/*##################################################################################################
#                                       Autor: Edward Quintero                                     #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                              Tema: Taller de Evaluación de Rendimiento                           #
#                     Fichero: fuente de multiplicación de matrices NxN por hilos.                 #
#   Objetivo: Evaluar el tiempo de ejecución del algoritmo clásico de multiplicación de matrices.  #
####################################################################################################*/

/*
 * Este programa en C implementa la multiplicación de matrices utilizando programación multihilo
 * para mejorar el rendimiento en matrices grandes. El programa divide la carga de trabajo entre
 * múltiples hilos, donde cada hilo es responsable de calcular un subconjunto de filas de la
 * matriz resultante.
 *
 * La memoria compartida se utiliza para almacenar las matrices, optimizando el uso de memoria
 * y facilitando el acceso entre los hilos. Se emplea un mutex para la sincronización, aunque
 * su uso es limitado debido a que cada hilo trabaja en su propia sección de la matriz de
 * salida, evitando conflictos de escritura.
 *
 * La ejecución del programa permite especificar el tamaño de la matriz (N x N) y el número
 * de hilos mediante argumentos en la línea de comandos. Además, el programa mide y muestra
 * el tiempo de ejecución, y permite imprimir las matrices para verificar el cálculo en
 * casos de matrices pequeñas.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3)   // Tamaño total del bloque de memoria compartida

// Declaración de variables y estructuras para concurrencia
pthread_mutex_t MM_mutex;             // Mutex para sincronización entre hilos
static double MEM_CHUNK[DATA_SIZE];   // Memoria compartida para las matrices
double *mA, *mB, *mC;                 // Punteros a las matrices A, B y C

// Estructura de parámetros para pasar información a cada hilo
struct parametros {
	int nH;     // Número total de hilos
	int idH;    // ID del hilo actual
	int N;      // Tamaño de la matriz
};

// Variables para medir el tiempo de ejecución
struct timeval start, stop;

// Función para llenar las matrices A y B con valores
void llenar_matriz(int SZ){ 
	srand48(time(NULL));
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1*i; // Valor para matriz A
		mB[i] = 2.2*i; // Valor para matriz B
		mC[i] = 0;     // Inicializa matriz C en 0
	}	
}

// Función para imprimir una matriz si su tamaño es pequeño
void print_matrix(int sz, double *matriz){
	if(sz < 12){
    	for(int i = 0; i < sz*sz; i++){
     		if(i % sz == 0) printf("\n");
        	printf(" %.3f ", matriz[i]);
		}	
    	printf("\n>-------------------->\n");
	}
}

// Inicializa el tiempo de ejecución
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

// Finaliza y muestra el tiempo de ejecución en microsegundos
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec;
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}

/*
 * Función de multiplicación de matrices por hilos.
 * Cada hilo multiplica una parte de la matriz A por la matriz B y almacena el resultado en C.
 * La función utiliza parámetros de la estructura 'parametros' para dividir el trabajo entre hilos.
 */
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables;
	
	int idH = data->idH;       // ID del hilo
	int nH  = data->nH;        // Número total de hilos
	int N   = data->N;         // Tamaño de la matriz

	// Calcula el rango de filas que este hilo procesará
	int ini = (N / nH) * idH;
	int fin = (N / nH) * (idH + 1);

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i * N);  // Puntero a la fila i de A
			pB = mB + j;        // Puntero a la columna j de B
            for (int k = 0; k < N; k++, pA++, pB += N){
				sumaTemp += (*pA * *pB);  // Acumula el producto en sumaTemp
			}
			mC[i * N + j] = sumaTemp;    // Asigna el valor calculado en la posición de C
		}
	}

	// Bloquea el mutex antes de salir
	pthread_mutex_lock(&MM_mutex);
	pthread_mutex_unlock(&MM_mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
    
    int SZ = atoi(argv[1]);         // Tamaño de la matriz (N x N)
    int n_threads = atoi(argv[2]);  // Número de hilos

    pthread_t p[n_threads];         // Vector de hilos
    pthread_attr_t atrMM;           // Atributos de los hilos

    // Asigna memoria de 'MEM_CHUNK' a las matrices
	mA = MEM_CHUNK;
	mB = mA + SZ * SZ;
	mC = mB + SZ * SZ;

	llenar_matriz(SZ);               // Llenar matrices A y B con valores iniciales
	print_matrix(SZ, mA);            // Imprime matriz A (si es pequeña)
	print_matrix(SZ, mB);            // Imprime matriz B (si es pequeña)

	inicial_tiempo();                // Comienza la medición de tiempo

	pthread_mutex_init(&MM_mutex, NULL);        // Inicializa el mutex
	pthread_attr_init(&atrMM);                  // Inicializa los atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Crea los hilos de multiplicación
    for (int j = 0; j < n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos);
	}

    // Espera la finalización de todos los hilos
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);
	final_tiempo();                // Finaliza la medición de tiempo
	
	print_matrix(SZ, mC);          // Imprime matriz C (resultado) si es pequeña

	pthread_attr_destroy(&atrMM);  // Destruye los atributos de hilo
	pthread_mutex_destroy(&MM_mutex);  // Destruye el mutex
	pthread_exit(NULL);            // Finaliza el hilo principal
}
