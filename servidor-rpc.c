#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <rpc/xdr.h>
#include "claves_rpc.h"  // <- debe ir antes que claves.h, porque define GetResult
#include "claves.h"


/* Dispatcher generado por rpcgen */
extern void claves_prog_1(struct svc_req *rqstp, SVCXPRT *transp);

/* === Implementación de los servicios === */

int *
destroy_1_svc(void *argp, struct svc_req *rqstp)
{
    static int result;
    result = destroy();
    return &result;
}

int *
setvalue_1_svc(SetArgs *args, struct svc_req *rqstp)
{
    static int result;
    struct Coord c = { args->value3.x, args->value3.y };
    result = set_value(
        args->key,
        args->value1,
        args->vect.N_value2,
        args->vect.V_value2,
        c
    );
    return &result;
}
GetResult *getvalue_1_svc(int *keyp, struct svc_req *rqstp){
    static GetResult result;
    static char buffer_value1[256];  // <-- memoria para value1

    memset(&result, 0, sizeof(GetResult));
    result.value1 = buffer_value1;


    struct Coord c;

    printf("[servidor] getvalue_1_svc: recibida petición para clave %d\n", *keyp);
    if (!result.value1) {
        fprintf(stderr, "Error: value1 es NULL antes de get_value\n");
        exit(1);
    }
    
    result.status = get_value(
        *keyp,
        result.value1,
        &result.vect.N_value2,
        result.vect.V_value2,
        &c
    );

    if (result.status == 0) {
        result.value3.x = c.x;
        result.value3.y = c.y;
        printf("[servidor] getvalue_1_svc: clave %d encontrada\n", *keyp);
    } else {
        printf("[servidor] getvalue_1_svc: clave %d no encontrada\n", *keyp);
    }

    return &result;
}



int *
modifyvalue_1_svc(SetArgs *args, struct svc_req *rqstp)
{
    static int result;
    struct Coord c = { args->value3.x, args->value3.y };
    result = modify_value(
        args->key,
        args->value1,
        args->vect.N_value2,
        args->vect.V_value2,
        c
    );
    return &result;
}

int *
deletekey_1_svc(int *keyp, struct svc_req *rqstp)
{
    static int result;
    result = delete_key(*keyp);
    return &result;
}

int *
exist_1_svc(int *keyp, struct svc_req *rqstp)
{
    static int result;
    result = exist(*keyp);
    return &result;
}

/* === Freeresult: libera la memoria que XDR haya asignado === */
bool_t
claves_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
    xdr_free(xdr_result, result);
    return TRUE;
}

/* === Main del servidor === */
int main(void)
{
    SVCXPRT *transp;

    /* Quita cualquier registro previo del programa */
    pmap_unset(CLAVES_PROG, CLAVES_VERS);

    /* Crea el transporte UDP */
    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (!svc_create(claves_prog_1, CLAVES_PROG, CLAVES_VERS, "udp")) {
        fprintf(stderr, "Error: no se pudo registrar CLAVES_PROG con svc_create.\n");
        exit(1);
    }
    printf("[servidor] Esperando peticiones RPC (UDP)...\n");
    svc_run();  /* Bucle principal */
    fprintf(stderr, "Error: svc_run() retornó inesperadamente.\n");
    return 1;
}
