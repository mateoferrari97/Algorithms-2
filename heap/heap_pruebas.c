#include "heap.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_ELEMENTOS 1000

int comparar_numeros(int x, int y){
    return (x > y) ?  1 : -1;
}

static void prueba_crear_heap_vacio(){
    heap_t* heap = heap_crear(NULL);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap desencolar clave A, es NULL, esta vacio", !heap_desencolar(heap));
    print_test("Prueba heap desencolar clave A, es NULL, esta vacio", !heap_desencolar(heap));
    print_test("Prueba heap ver maximo es null", !heap_ver_max(heap));

    heap_destruir(heap, NULL);
}
static void prueba_heap_agregar(){
    heap_t* heap = heap_crear((cmp_func_t)strcmp);
    char* clave1 = "Mateo";
    char* clave2 = "Puntero";
    char* clave3 = "Hambre";

    print_test("Prueba heap insertar clave1", heap_encolar(heap, clave1));
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);  
    print_test("Prueba heap insertar clave2", heap_encolar(heap, clave2));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2); 
    print_test("Prueba heap insertar clave3", heap_encolar(heap, clave3));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);         
    print_test("Prueba heap desencolar", heap_desencolar(heap));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2); 
    print_test("Prueba heap desencolar", heap_desencolar(heap));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap desencolar", heap_desencolar(heap));
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);  
}
static void prueba_heap_destruir_free(){
    heap_t* heap = heap_crear((cmp_func_t)strcmp);

    char* clave1 = malloc(sizeof(char) * 10);
    char* clave2 = malloc(sizeof(char )* 10);
    char* clave3 = malloc(sizeof(char) * 10);
    strcpy(clave1, "gracias");
    strcpy(clave2, "por tanto");
    strcpy(clave3, "valgrind");

    print_test("Prueba heap insertar clave1", heap_encolar(heap, clave1));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);  
    print_test("Prueba heap insertar clave2", heap_encolar(heap, clave2));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2); 
    print_test("Prueba heap insertar clave3", heap_encolar(heap, clave3));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);             

    heap_destruir(heap, free);  
}
static void prueba_heap_volumen(){
    heap_t* heap = heap_crear((cmp_func_t)comparar_numeros);
    bool ok = true;
    size_t vector[MAX_ELEMENTOS];
    size_t i;
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    for(i = 0; i < MAX_ELEMENTOS; i++){
        vector[i] = i;
        ok &= heap_encolar(heap, &vector[i]);
    }
    print_test("Prueba heap todo ok volumen", ok);
    print_test("Prueba heap el maximo es 999", *(size_t*)heap_ver_max(heap) == 999);
    print_test("Prueba heap la cantidad de elementos es 1000", heap_cantidad(heap) == 1000);
    for(size_t j = 0; j < MAX_ELEMENTOS; j++){
        heap_desencolar(heap);
    }    
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0); 
    heap_destruir(heap, NULL);
}
void pruebas_heap_alumno(){
    prueba_crear_heap_vacio();
    prueba_heap_agregar();
    prueba_heap_destruir_free();
    prueba_heap_volumen();
}
