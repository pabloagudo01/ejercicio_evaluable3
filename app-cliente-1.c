#include <stdio.h>
#include "claves.h"

int main() {
    int key = 99;
    char value1[256] = "Valor inicial";
    double V_value2[2] = {3.14, 1.61};
    struct Coord value3 = {5, 10};
    int N_value2 = 2;

    printf("Cl1: Iniciando cliente...\n");

    // Intentar obtener una tupla inexistente
    char r_value1[256];
    double r_V_value2[32];
    int r_N_value2;
    struct Coord r_value3;

    if (get_value(key, r_value1, &r_N_value2, r_V_value2, &r_value3) == 0) {
        printf("Cl1: Error: se obtuvo una tupla que no debería existir.\n");
    } else {
        printf("Cl1: Confirmado: la tupla con clave %d no existe.\n", key);
    }

    // Crear la tupla
    if (set_value(key, value1, N_value2, V_value2, value3) == 0) {
        printf("Cl1: Tupla creada correctamente.\n");
    } else {
        printf("Cl1: Error al crear la tupla.\n");
    }

    // Modificar la tupla
    char mod_value1[256] = "Valor modificado";
    double mod_V_value2[1] = {42.0};
    struct Coord mod_value3 = {7, 14};
    int mod_N_value2 = 1;

    if (modify_value(key, mod_value1, mod_N_value2, mod_V_value2, mod_value3) == 0) {
        printf("Cl1: Tupla modificada correctamente.\n");
    } else {
        printf("Cl1: Error al modificar la tupla.\n");
    }

    // Buscar una clave errónea
    int wrong_key = 999;
    if (get_value(wrong_key, r_value1, &r_N_value2, r_V_value2, &r_value3) == 0) {
        printf("Cl1: Error: se obtuvo una tupla con clave incorrecta.\n");
    } else {
        printf("Cl1: Confirmado: la clave %d no existe.\n", wrong_key);
    }

    // Buscar la clave correcta y mostrar contenido
    if (get_value(key, r_value1, &r_N_value2, r_V_value2, &r_value3) == 0) {
        printf("Cl1: Tupla encontrada: %s, [%d valores], (%d, %d)\n",
               r_value1, r_N_value2, r_value3.x, r_value3.y);
    } else {
        printf("Cl1: Error al recuperar la tupla final.\n");
    }

    // Borrar la tupla
    if (delete_key(key) == 0) {
        printf("Cl1: Tupla eliminada correctamente.\n");
    } else {
        printf("Cl1: Error al eliminar la tupla.\n");
    }

    printf("Cl1: Cliente finalizado correctamente.\n");
    return 0;
}
