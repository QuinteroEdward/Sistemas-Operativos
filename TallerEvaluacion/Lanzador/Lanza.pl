####################################################################################################
#                                       Autor: Edward Quintero                                     #
#                                         Fecha: 11/11/2024                                        #
#                                    Materia: Sistemas Operativos                                  #
#                              Tema: Taller de Evaluación de Rendimiento                           #
#          Fichero: Lanzandor para automatizar el proceso de multiplicacion de matrices            #
####################################################################################################

# Este script en Perl automatiza la ejecución de un archivo ejecutable con diferentes configuraciones de
# tamaño de matriz y número de hilos. Para cada combinación de parámetros, el script realiza múltiples
# repeticiones, permitiendo realizar pruebas y recopilar resultados de manera estructurada.
#
# El script construye un nombre de archivo específico para cada configuración, donde guarda la salida
# generada por el ejecutable. Esto facilita el análisis de los resultados de rendimiento con distintos
# tamaños de matriz e hilos.


# Guardar la ruta actual del directorio en la variable $Path
$Path = `pwd`;
chomp($Path); # Eliminar el salto de línea que añade `pwd`

# Definir el nombre del ejecutable y configuraciones de tamaño de matriz e hilos
$Nombre_Ejecutable = "MM_ejecutable"; # Nombre del archivo ejecutable
@Size_Matriz = ("800", "1000", "1200", "1400", "1600", "1800", "2000", "2200"); # Tamaños de matriz a utilizar en la ejecución
@Num_Hilos = (1, 2, 4, 8); # Número de hilos a usar en las pruebas
$Repeticiones = 30; # Número de veces que se repetirá cada combinación de tamaño de matriz y número de hilos

# Iterar sobre cada tamaño de matriz
foreach $size (@Size_Matriz) {
    # Iterar sobre cada número de hilos
    foreach $hilo (@Num_Hilos) {
        # Crear el nombre del archivo donde se guardarán los resultados
        $file = "$Path/$Nombre_Ejecutable-" . $size . "-Hilos-" . $hilo . ".dat";
        
        # Repetir la ejecución de pruebas según el número de repeticiones
        for ($i = 0; $i < $Repeticiones; $i++) {
            # Ejecutar el archivo ejecutable con los parámetros y redirigir la salida al archivo
            system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
            
            # Imprimir en consola el comando que se ejecutaría para verificar su construcción
            #printf("$Path/$Nombre_Ejecutable $size $hilo \n");
        }
        
        # Cerrar el archivo después de cada combinación de tamaño y número de hilos
        close($file);
        
        # Incrementar el contador $p 
        $p = $p + 1;
    }
}
