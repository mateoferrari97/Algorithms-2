#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prueba_crear_abb(){
    printf("\n********Prueba crear ABB*******\n");
    abb_t* abb = abb_crear(NULL,NULL);
    print_test("Crear abb--->", abb);
    abb_destruir(abb);
}

void pruebas_abb_alumno(){
    prueba_crear_abb();
}
