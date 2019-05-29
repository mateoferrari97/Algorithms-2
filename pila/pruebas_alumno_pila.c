#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>

#define MAXIMA_CANT_ELEMENTOS 10000

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_crear_destruir(){
    printf("-----Prueba N.1 Crear-Destruir-----\n");
    pila_t* pila = pila_crear();
    print_test("Prueba crear pila...", pila != NULL);
    print_test("Esta vacia...", pila_esta_vacia(pila));

    pila_destruir(pila);
}

void prueba_apilar_desapilar(){
    printf("\n-----Prueba N.2 Apilar-Desapilar-----\n");
    pila_t* pila = pila_crear();
    int elemento = 10;
    
    print_test("Apilar elemento...", pila_apilar(pila, &elemento));
    print_test("Ver ultimo elemento...", pila_ver_tope(pila) == &elemento);
    print_test("Desapilar elemento...", pila_desapilar(pila));
    print_test("Ver ultimo elemento == NULL...", pila_ver_tope(pila) == NULL);
    print_test("Ver si esta vacia...", pila_esta_vacia(pila));

    pila_destruir(pila);
}

void prueba_elemento_null(){
    printf("\n-----Pruebas N.4 Elemento NULL -----\n");
    pila_t* pila = pila_crear();
    void* valor_null = NULL;

    print_test("Apilar elemento NULL...", pila_apilar(pila, valor_null));
    pila_destruir(pila);
}

void prueba_limite_desapilar(){
    printf("\n-----Pruebas N.5 Condicion limite apilar y desapilar -----\n");
    pila_t* pila = pila_crear();
    size_t arreglo[MAXIMA_CANT_ELEMENTOS];
    bool ok = true;

    for(int i = 0; i < MAXIMA_CANT_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= pila_apilar(pila, &arreglo[i]);
    }

    for(int j = 0; j < MAXIMA_CANT_ELEMENTOS; j++){
        pila_desapilar(pila);
    }
    
    print_test("Pila vacia...", pila_esta_vacia(pila) == true);
    pila_destruir(pila);    
}

void prueba_desapilar_ver_tope(){
    printf("\n -----Prueba N.6 Acciones desapilar y ver_tope en una pila creada son invalidas-----\n");
    pila_t* pila = pila_crear();

    print_test("Ver tope en pila recien creada...", pila_ver_tope(pila) == NULL);
    pila_destruir(pila);
}

void prueba_vacia_al_crear(){
    printf("\n -----Prueba N.7 esta_vacia al crear pila.-----\n");
    pila_t* pila = pila_crear();

    print_test("Ver pila si esta vacia al crearla...", pila_esta_vacia(pila) == true);
    pila_destruir(pila);
}

void prueba_desapilar_al_crear(){
    printf("\n -----Prueba N.8 esta_vacia al crear pila.-----\n");
    pila_t* pila = pila_crear();

    print_test("Desapilar pila cuando esta vacia...", pila_desapilar(pila) == NULL);
    pila_destruir(pila);    
}

void prueba_desapilar_en_orden(){
    pila_t* pila = pila_crear();
    size_t arreglo[4];
    bool ok = true;

    for(size_t i = 0; i < 4; i++){
        arreglo[i] = i;
        ok &= pila_apilar(pila, &arreglo[i]);
    }

    print_test("Apilar salio bien en 'desapilar_en_orden'...", ok == true);

    for(size_t j = 3; j == 0; j--){
        size_t* valor = pila_desapilar(pila);
        if(*valor != j){
            ok = false;
        }
    }

    print_test("Desapilar en orden salio bien...", ok == true);    
    pila_destruir(pila);
}

void pruebas_pila_alumno() {
    prueba_crear_destruir();
    prueba_apilar_desapilar();
    prueba_desapilar_al_crear();
    prueba_desapilar_ver_tope();
    prueba_elemento_null();
    prueba_limite_desapilar();
    prueba_vacia_al_crear();
    prueba_desapilar_en_orden();
}