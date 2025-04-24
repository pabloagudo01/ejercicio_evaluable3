# Makefile para sistema cliente-servidor con RPC

CC       = gcc
RPCGEN   = rpcgen
CFLAGS   = -Wall -g -fPIC -I/usr/include/tirpc
LDFLAGS  = -shared
LIBS     = -ltirpc -lpthread
RPCFLAGS = -C

X_FILE      = claves_rpc.x
HDR         = claves_rpc.h
XDR_SRC     = claves_rpc_xdr.c
CLNT_SRC    = claves_rpc_clnt.c
SVC_SRC     = claves_rpc_svc.c

LIB_OBJS    = claves.o
CLIENT_OBJS = app-cliente.o proxy-rpc.o claves_rpc_clnt.o claves_rpc_xdr.o
SERVER_OBJS = servidor-rpc.o claves_rpc_svc.o claves_rpc_xdr.o

.PHONY: all clean

all: libclaves.so cliente servidor

# === Generación de archivos RPC ===
$(HDR) $(XDR_SRC) $(CLNT_SRC) $(SVC_SRC): $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -h -o $(HDR)   $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -c -o $(XDR_SRC) $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -l -o $(CLNT_SRC) $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -m -o $(SVC_SRC) $(X_FILE)

# === Biblioteca compartida ===
libclaves.so: $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c $< -o $@

# === Cliente ===
cliente: $(CLIENT_OBJS)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath,'$$ORIGIN' $(LIBS)

app-cliente.o: app-cliente.c claves.h
	$(CC) $(CFLAGS) -c $< -o $@

proxy-rpc.o: proxy-rpc.c claves_rpc.h claves.h
	$(CC) $(CFLAGS) -c $< -o $@

claves_rpc_clnt.o: claves_rpc_clnt.c claves_rpc.h
	$(CC) $(CFLAGS) -c $< -o $@

claves_rpc_xdr.o: claves_rpc_xdr.c claves_rpc.h
	$(CC) $(CFLAGS) -c $< -o $@

# === Servidor ===
servidor: $(SERVER_OBJS) libclaves.so
	$(CC) -o $@ $(SERVER_OBJS) -L. -lclaves -Wl,-rpath,'$$ORIGIN' $(LIBS)

servidor-rpc.o: servidor-rpc.c claves_rpc.h claves.h
	$(CC) $(CFLAGS) -c $< -o $@

claves_rpc_svc.o: claves_rpc_svc.c claves_rpc.h
	$(CC) $(CFLAGS) -c $< -o $@

# === Limpieza ===
clean:
	rm -f *.o libclaves.so cliente servidor \
	       $(HDR) $(XDR_SRC) $(CLNT_SRC) $(SVC_SRC)
