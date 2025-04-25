#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "claves.h"
#include "claves_rpc.h"

#define TIMEOUT_SEC 25
static CLIENT *clnt_handle = NULL;

/* Inicializa el handle RPC usando la variable de entorno IP_TUPLAS */
static void init_client(void)
{
    if (clnt_handle == NULL) {
        char *server_ip = getenv("IP_TUPLAS");
        if (!server_ip) {
            fprintf(stderr,
                "Error: variable de entorno IP_TUPLAS no definida. Usando 127.0.0.1\n");
            server_ip = "127.0.0.1";
        }
        clnt_handle = clnt_create(server_ip, CLAVES_PROG, CLAVES_VERS, "udp");
        if (clnt_handle == NULL) {
            clnt_pcreateerror("Error al crear cliente RPC");
            exit(1);
        }
    }
}

int destroy(void)
{
    init_client();
    int *res = destroy_1(NULL, clnt_handle);
    return res ? *res : -1;
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3)
{
    init_client();
    SetArgs args;
    args.key = key;
    args.value1 = strdup(value1);
    args.vect.N_value2 = N_value2;
    memcpy(args.vect.V_value2, V_value2, N_value2 * sizeof(double));
    args.value3.x = value3.x;
    args.value3.y = value3.y;

    int *res = setvalue_1(&args, clnt_handle);
    free(args.value1);
    return res ? *res : -1;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3)
{
    init_client();
    GetResult *res = getvalue_1(&key, clnt_handle);
    if (!res || res->status != 0) return -1;

    /* Copiamos la cadena */
    strncpy(value1, res->value1, 256);
    /* N.º de elementos */
    *N_value2 = res->vect.N_value2;
    /* CORRECCIÓN: campo correcto con guión bajo */
    memcpy(V_value2, res->vect.V_value2, (*N_value2) * sizeof(double));
    /* Coordenadas */
    value3->x = res->value3.x;
    value3->y = res->value3.y;
    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3)
{
    init_client();
    SetArgs args;
    args.key = key;
    args.value1 = strdup(value1);
    args.vect.N_value2 = N_value2;
    memcpy(args.vect.V_value2, V_value2, N_value2 * sizeof(double));
    args.value3.x = value3.x;
    args.value3.y = value3.y;

    int *res = modifyvalue_1(&args, clnt_handle);
    free(args.value1);
    return res ? *res : -1;
}

int delete_key(int key)
{
    init_client();
    int *res = deletekey_1(&key, clnt_handle);
    return res ? *res : -1;
}

int exist(int key)
{
    init_client();
    int *res = exist_1(&key, clnt_handle);
    return res ? *res : -1;
}
