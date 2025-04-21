# === Configuración del compilador y flags ===
CC       = gcc
RPCGEN   = rpcgen
CFLAGS   = -Wall -g -fPIC -I/usr/include/tirpc
LDFLAGS  = -shared
LIBS     = -ltirpc -lpthread
RPCFLAGS = -C

# === Archivos fuente RPC y propios ===
X_FILE     = claves_rpc.x
RPC_HDR    = claves_rpc.h
RPC_CLNT   = claves_rpc_clnt.c
RPC_XDR    = claves_rpc_xdr.c
RPC_SVC    = claves_rpc_svc.c

LIB_SRC    = claves.c
LIB_OBJ    = claves.o

CLIENT_OBJS = app-cliente.o proxy-rpc.o claves_rpc_clnt.o claves_rpc_xdr.o
SERVER_OBJS = servidor-rpc.o claves_rpc_svc.o claves_rpc_xdr.o

# === Targets principales ===
.PHONY: all clean

all: libclaves.so cliente servidor

# === Generación de archivos RPC ===
$(RPC_HDR): $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -h -o $@ $<

$(RPC_CLNT): $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -l -o $@ $<

$(RPC_XDR): $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -c -o $@ $<

$(RPC_SVC): $(X_FILE)
	$(RPCGEN) $(RPCFLAGS) -m -o $@ $<

# === Biblioteca compartida ===
libclaves.so: $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -lpthread

claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c $< -o $@

# === Cliente ===
cliente: $(CLIENT_OBJS) libclaves.so
	$(CC) -o $@ $(CLIENT_OBJS) -L. -lclaves -Wl,-rpath,'$$ORIGIN' $(LIBS)

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
	       $(RPC_HDR) $(RPC_CLNT) $(RPC_XDR) $(RPC_SVC)
