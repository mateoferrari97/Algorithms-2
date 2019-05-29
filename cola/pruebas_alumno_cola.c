#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXIMA_CANT_ELEMENTOS 10000

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_crear_destruir(){
    cola_t* cola = cola_crear();
    print_test("Crear Cola...", cola != NULL);
    print_test("Ver si cola esta vacia...", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}

void prueba_encolar_desencolar(){
    cola_t* cola = cola_crear();
    size_t arreglo[3];

    for(size_t i = 0; i < 3; i++){
        arreglo[i] = i;
        cola_encolar(cola, &arreglo[i]);
    }

    for(size_t i = 0; i < 3; i++){
        print_test("Verificar que los valor que salen sean los primeros que entraron FIFO..", cola_ver_primero(cola) == cola_desencolar(cola));
    }
    print_test("Esta vacio...", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}

void prueba_elemento_null(){
    cola_t* cola = cola_crear();
    int* elemento_null = NULL;
    print_test("Apilar elemento NULL", cola_encolar(cola, elemento_null));
    print_test("Desencolar...", cola_desencolar(cola) == NULL);
    cola_destruir(cola, NULL);
}

void prueba_limite_encolar(){
    cola_t* cola = cola_crear();
    size_t arreglo[MAXIMA_CANT_ELEMENTOS];
    bool ok = true;

    for(size_t i = 0; i < MAXIMA_CANT_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= cola_encolar(cola, &arreglo[i]);
    }

    print_test("Ver si todo salio ok al apilar...", ok == true);
    print_test("Cola no esta vacia...", cola_esta_vacia(cola) == false);
    
    for(size_t j = 0; j < MAXIMA_CANT_ELEMENTOS; j++){
        cola_desencolar(cola);
    }

    print_test("Cola esta vacia...", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}

void prueba_desencolar_ver_tope(){
    cola_t* cola = cola_crear();
    print_test("Ver primer elemento al estar vacio...", cola_ver_primero(cola) == NULL);
    print_test("Cola desencolar al estar vacia devuelve NULL", cola_desencolar(cola) == NULL);
    cola_destruir(cola,NULL);
}

void prueba_vacia_al_crear(){
    cola_t* cola = cola_crear();
    print_test("Cola esta vacia al crear...", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}

void prueba_desencolar_al_crear(){
    cola_t* cola = cola_crear();
    print_test("Cola esta vacia al crear...", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}

void destruir_dato(void* dato){
    free(dato);
}

void prueba_destruir_dato(){
    cola_t* cola = cola_crear();
    int* elemento = malloc(sizeof(int));
    print_test("Encolar elemento...", cola_encolar(cola, elemento));
    print_test("Cola no esta vacia al crear...", cola_esta_vacia(cola) == false);
    print_test("Verifico valor del elemento insertado...", cola_ver_primero(cola) == elemento);
    cola_destruir(cola, free);
    print_test("Cola destruida...", true);
}

 //verificar que los primeros que salen, son los ultimos que entraron.
void prueba_desencolar_en_orden(){
    cola_t* cola = cola_crear();
    size_t arreglo[4];
    bool ok = true;

    for(size_t i = 0; i < 4; i++){
        arreglo[i] = i;
        ok &= cola_encolar(cola, &arreglo[i]);
    }

    print_test("Encolar salio bien en 'desencolar_en_orden'...", ok == true);

    for(size_t j = 0; j < 4; j++){
        size_t* valor = cola_desencolar(cola);
        if(*valor != j){
            ok = false;
        }
    }

    print_test("Desencolar en orden salio bien...", ok == true);    
    cola_destruir(cola, NULL);
}
void pruebas_cola_alumno(){
    prueba_crear_destruir();
    prueba_encolar_desencolar();
    prueba_elemento_null();
    prueba_limite_encolar();
    prueba_desencolar_ver_tope();
    prueba_vacia_al_crear();
    prueba_desencolar_al_crear();
    prueba_destruir_dato();
    prueba_desencolar_en_orden();
}