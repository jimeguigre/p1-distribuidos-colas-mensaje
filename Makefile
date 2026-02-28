# Definición de variables
CC = gcc
CFLAGS = -Wall -Werror -fPIC
LDFLAGS = -L. -lclaves
LD_LIBRARY_PATH_EXPORT = export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH

# Objetivos principales
all: libclaves.so app-cliente

# 1. Crear la biblioteca dinámica libclaves.so
# Se usa -shared para indicar que es una biblioteca compartida
libclaves.so: claves.o
	$(CC) -shared -o libclaves.so claves.o 

# Compilación de claves.c con -fPIC (Position Independent Code)
claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

# 2. Crear el ejecutable del cliente
# Se enlaza con libclaves.so usando -L. (directorio actual) y -lclaves
app-cliente: app-cliente.o libclaves.so
	$(CC) -o app-cliente app-cliente.o $(LDFLAGS) 

app-cliente.o: app-cliente.c claves.h
	$(CC) $(CFLAGS) -c app-cliente.c

# 3. Limpieza de archivos temporales
clean:
	rm -f *.o *.so app-cliente
	@echo "Limpieza completada."

# 4. Ayuda para ejecutar (configura el path de la biblioteca)
run:
	$(LD_LIBRARY_PATH_EXPORT); ./app-cliente