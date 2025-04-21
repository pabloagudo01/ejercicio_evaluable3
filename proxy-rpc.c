#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "claves.h"
#include "claves_rpc.h"

#define TIMEOUT_SEC 25
static CLIENT *clnt_handle = NULL;

static void init_client()
{
    if (clnt_handle == NULL) {
        char *server_ip = getenv("IP_TUPLAS");
        if (!server_ip) {
            fprintf(stderr, "[proxy] IP_TUPLAS no definida, usando 127.0.0.1 por defecto\n");
            server_ip = "127.0.0.1";
        } else {
            printf("[proxy] Cliente RPC inicializado con IP_TUPLAS=%s\n", server_ip);
        }

        clnt_handle = clnt_create(server_ip, CLAVES_PROG, CLAVES_VERS, "tcp");
        if (clnt_handle == NULL) {
            clnt_pcreateerror("[proxy] Error al crear cliente RPC");
            exit(1);
        }
    }
}

int destroy(void)
{
    init_client();
    int *result = destroy_1(NULL, clnt_handle);
    if (!result) {
        fprintf(stderr, "[proxy] Fallo en destroy RPC\n");
        return -1;
    }
    return *result;
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

    int *result = setvalue_1(&args, clnt_handle);
    free(args.value1);
    if (!result) {
        fprintf(stderr, "[proxy] Fallo en set_value RPC\n");
        return -1;
    }
    return *result;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3)
{
    init_client();
    GetResult *result = getvalue_1(&key, clnt_handle);
    if (!result || result->status != 0) {
        fprintf(stderr, "[proxy] Fallo en get_value RPC\n");
        return -1;
    }
    strncpy(value1, result->value1, 256);
    *N_value2 = result->vect.N_value2;
    memcpy(V_value2, result->vect.V_value2, (*N_value2) * sizeof(double));
    value3->x = result->value3.x;
    value3->y = result->value3.y;
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

    int *result = modifyvalue_1(&args, clnt_handle);
    free(args.value1);
    if (!result) {
        fprintf(stderr, "[proxy] Fallo en modify_value RPC\n");
        return -1;
    }
    return *result;
}

int delete_key(int key)
{
    init_client();
    int *result = deletekey_1(&key, clnt_handle);
    if (!result) {
        fprintf(stderr, "[proxy] Fallo en delete_key RPC\n");
        return -1;
    }
    return *result;
}

int exist(int key)
{
    init_client();
    int *result = exist_1(&key, clnt_handle);
    if (!result) {
        fprintf(stderr, "[proxy] Fallo en exist RPC\n");
        return -1;
    }
    return *result;
}