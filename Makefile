CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -L. -lclaves
LD_LIBRARY_PATH = .

all: libclaves.so app-cliente

# Crear la biblioteca dinámica
libclaves.so: claves.o
	$(CC) -shared -o libclaves.so claves.o

claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

# Crear el cliente monolítico
app-cliente: app-cliente.o libclaves.so
	$(CC) -o app-cliente app-cliente.o $(LDFLAGS)

app-cliente.o: app-cliente.c claves.h
	$(CC) $(CFLAGS) -c app-cliente.c

clean:
	rm -f *.o *.so app-cliente