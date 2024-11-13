import re

# Abrir el archivo original en modo de lectura
with open('noticias_nuevas.txt', 'r', encoding='utf-8', errors='ignore') as f:
    # Leer todo el contenido y eliminar caracteres extraños con una expresión regular
    contenido = f.read()
    # Reemplaza cualquier carácter no ASCII por un espacio
    contenido_limpio = re.sub(r'[^\x00-\x7F]+', '', contenido)

# Guardar el contenido limpio en un nuevo archivo
with open('noticiasLimpio_nuevas.txt', 'w', encoding='utf-8') as f:
    f.write(contenido_limpio)

print("Archivo limpiado y guardado como noticiasLimpio.txt")
