/* claves_rpc.x */
const MAXLEN = 256;
const MAXDV = 32;

/* Definir la estructura con un nombre diferente (CoordRPC) */
struct CoordRPC {
    int x;
    int y;
};

struct Vvalue2 {
    int N_value2;
    double V_value2[MAXDV];
};

typedef string value1<MAXLEN>;

struct SetArgs {
    int key;
    value1 value1;
    Vvalue2 vect;
    CoordRPC value3;  /* Usar CoordRPC */
};

struct GetResult {
    int status;
    value1 value1;
    Vvalue2 vect;
    CoordRPC value3;  /* Usar CoordRPC */
};

program CLAVES_PROG {
    version CLAVES_VERS {
        int DESTROY(void) = 1;
        int SETVALUE(SetArgs) = 2;
        GetResult GETVALUE(int) = 3;
        int MODIFYVALUE(SetArgs) = 4;
        int DELETEKEY(int) = 5;
        int EXIST(int) = 6;
    } = 1;
} = 0x20000001;