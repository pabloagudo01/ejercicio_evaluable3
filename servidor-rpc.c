#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "claves.h"
#include "claves_rpc.h"

// Funciones de servicio RPC
int *destroy_1_svc(void *argp, struct svc_req *rqstp) {
    static int result;
    result = destroy();
    return &result;
}

int *setvalue_1_svc(SetArgs *args, struct svc_req *rqstp) {
    static int result;
    struct Coord value3 = { args->value3.x, args->value3.y };
    result = set_value(args->key, args->value1, args->vect.N_value2, args->vect.V_value2, value3);
    return &result;
}

GetResult *getvalue_1_svc(int *key, struct svc_req *rqstp) {
    static GetResult result;
    memset(&result, 0, sizeof(GetResult)); 
    struct Coord value3;
    result.status = get_value(*key, result.value1, &result.vect.N_value2, result.vect.V_value2, &value3);
    result.value3.x = value3.x;
    result.value3.y = value3.y;
    return &result;
}

int *modifyvalue_1_svc(SetArgs *args, struct svc_req *rqstp) {
    static int result;
    struct Coord value3 = { args->value3.x, args->value3.y };
    result = modify_value(args->key, args->value1, args->vect.N_value2, args->vect.V_value2, value3);
    return &result;
}

int *deletekey_1_svc(int *key, struct svc_req *rqstp) {
    static int result;
    result = delete_key(*key);
    return &result;
}

int *exist_1_svc(int *key, struct svc_req *rqstp) {
    static int result;
    result = exist(*key);
    return &result;
}

//Dispatcher generado por rpcgen
extern void claves_prog_1(struct svc_req *rqstp, register SVCXPRT *transp);

//main para registrar y lanzar el servicio
int main() {
    SVCXPRT *transp;

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
        fprintf(stderr, "Error: no se pudo crear el transporte TCP.\n");
        exit(1);
    }

    if (!svc_register(transp, CLAVES_PROG, CLAVES_VERS, claves_prog_1, IPPROTO_TCP)) {
        fprintf(stderr, "Error: no se pudo registrar el servicio CLAVES_PROG.\n");
        exit(1);
    }

    printf("[servidor] Esperando peticiones RPC (TCP)...\n");
    svc_run();
    fprintf(stderr, "Error: svc_run terminó inesperadamente.\n");
    return 1;
}
