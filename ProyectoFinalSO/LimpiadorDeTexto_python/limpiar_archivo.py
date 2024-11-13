# Este código limpia el contenido de un archivo de texto, eliminando todos los caracteres no ASCII.
# Esto puede ser útil cuando se trabaja con archivos que pueden contener caracteres especiales
# o de otro idioma y se necesita tener solo caracteres estándar en inglés.
# El código abre un archivo de texto, lee su contenido, aplica una expresión regular para eliminar 
# caracteres no ASCII, y luego guarda el contenido limpio en un nuevo archivo.

import re  # Importa el módulo 're' para trabajar con expresiones regulares

# Abrir el archivo original en modo de lectura
with open('noticias_nuevas.txt', 'r', encoding='utf-8', errors='ignore') as f:
    # Leer todo el contenido y eliminar caracteres extraños con una expresión regular
    contenido = f.read()  # Lee todo el contenido del archivo original

    # Reemplaza cualquier carácter no ASCII por un espacio
    contenido_limpio = re.sub(r'[^\x00-\x7F]+', '', contenido)  
    # Utiliza una expresión regular para reemplazar cualquier carácter que no esté en el rango ASCII
    # (códigos 0-127) por una cadena vacía, eliminando así los caracteres especiales

# Guardar el contenido limpio en un nuevo archivo
with open('noticiasLimpio_nuevas.txt', 'w', encoding='utf-8') as f:
    f.write(contenido_limpio)  # Escribe el contenido limpio en un archivo nuevo

print("Archivo limpiado y guardado como noticiasLimpio_nuevas.txt")  # Mensaje de confirmación

