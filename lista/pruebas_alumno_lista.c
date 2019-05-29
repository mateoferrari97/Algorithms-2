#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX_ELEMENTOS 10000


void prueba_crear_destruir(){
    lista_t* lista = lista_crear();
    print_test("Lista creada...", lista != NULL);
    print_test("Ver si lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
}

void prueba_agregar_eliminar_elemento(){
    lista_t* lista = lista_crear();
    int elemento_entero = 10;
    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    print_test("Agregar elemento INT...", lista_insertar_primero(lista, &elemento_entero));
    print_test("Lista no esta vacia...", !lista_esta_vacia(lista));
    print_test("Eliminar elemento...", lista_borrar_primero(lista) == &elemento_entero);
    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
}

void prueba_elemento_null(){
    lista_t* lista = lista_crear();
    int* elemento_null = NULL;
    print_test("Agregar elemento NULL", lista_insertar_primero(lista, elemento_null));
    print_test("Lista esta vacia...", !lista_esta_vacia(lista));
    print_test("Eliminar elemento...", lista_borrar_primero(lista) == NULL);
    lista_destruir(lista, NULL);
}

void prueba_agregar_max_elementos(){
    lista_t* lista = lista_crear();
    size_t arreglo[MAX_ELEMENTOS];
    bool ok = true;

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= lista_insertar_primero(lista, &arreglo[i]);
    }

    print_test("Todo salio ok al agregar max elementos...", ok);
    print_test("Lista no esta vacia...", !lista_esta_vacia(lista));
    
    for(size_t j = 0; j < MAX_ELEMENTOS; j++){
        lista_borrar_primero(lista);
    }

    print_test("Lista esta vacia...", lista_esta_vacia(lista) == true);
    lista_destruir(lista, NULL);
}

void prueba_ver_elemento_al_eliminar(){
    lista_t* lista = lista_crear();
    int elemento = 5;
    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    print_test("Agregar elemento...", lista_insertar_primero(lista, &elemento));
    print_test("Lista no esta vacia...", !lista_esta_vacia(lista));
    print_test("Lista ver elemento...", lista_ver_primero(lista) == &elemento);
    print_test("Borrar elemento...", lista_borrar_primero(lista) == &elemento);
    lista_destruir(lista, NULL);
}

void prueba_vacia_al_crear(){
    lista_t* lista = lista_crear();
    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
}

void prueba_eliminar_al_crear(){
    lista_t* lista = lista_crear();
    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    print_test("Borrar elemento...", lista_borrar_primero(lista) == NULL);
    lista_destruir(lista, NULL);
}

void prueba_destruir_dato(){
    lista_t* lista = lista_crear();
    int* elemento = malloc(sizeof(int));
    print_test("Agregar elemento...", lista_insertar_primero(lista, elemento));
    print_test("Lista no esta vacia al crear...", !lista_esta_vacia(lista));
    print_test("Verifico valor del elemento insertado...", lista_ver_primero(lista) == elemento);
    lista_destruir(lista, free);
    print_test("Lista destruida...", true);
}

/*********************************************
 ****** PRUEBAS ITERADOR EXTERNO **********
 *********************************************/

void prueba_iterador_agregar_al_inicio(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    int elemento = 5;

    print_test("Lista esta vacia al crear...", lista_esta_vacia(lista));
    lista_iter_insertar(iter, &elemento);
    print_test("Lista no esta vacia...", !lista_esta_vacia(lista));
    print_test("Lista primero == Iterador actual...", lista_ver_primero(lista) == lista_iter_ver_actual(iter));
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
}

void prueba_iterador_agregar_al_final(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t arreglo[MAX_ELEMENTOS];
    bool ok = true;

    print_test("Lista esta vacia al crear...", lista_esta_vacia(lista));

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= lista_insertar_primero(lista, &arreglo[i]);
        lista_iter_avanzar(iter);
    }

    print_test("Iterador al final...", lista_iter_al_final(iter));  
    print_test("Agregar al final del iterador...", lista_iter_al_final(iter));      
    print_test("Lista no esta vacia al crear...", !lista_esta_vacia(lista));    

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        lista_borrar_primero(lista);
    }    

    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
}

void prueba_agregar_elementos_medio(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t arreglo[MAX_ELEMENTOS];
    bool ok = true;

    print_test("Lista esta vacia al crear...", lista_esta_vacia(lista));

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= lista_iter_insertar(iter, &arreglo[i]);
    }
   
    print_test("Lista no esta vacia al crear...", !lista_esta_vacia(lista));    

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        lista_borrar_primero(lista);
    }    

    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
}

void prueba_remover_elemento_al_crear(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    int elemento = 5;
    print_test("Lista esta vacia al crear...", lista_esta_vacia(lista));
    lista_iter_insertar(iter, &elemento);
    print_test("Lista no esta vacia...", !lista_esta_vacia(lista));
    lista_iter_borrar(iter);
    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
}

void prueba_eliminar_elemento_final(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t arreglo[MAX_ELEMENTOS];
    bool ok = true;

    print_test("Lista esta vacia al crear...", lista_esta_vacia(lista));

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= lista_iter_insertar(iter, &arreglo[i]);
    }
   
    lista_iter_borrar(iter);
    print_test("Lista no esta vacia al crear...", !lista_esta_vacia(lista));    

    for(size_t i = 0; i < MAX_ELEMENTOS-1; i++){
        lista_borrar_primero(lista);
    }    

    print_test("Lista esta vacia...", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
}

void prueba_eliminar_elemento_medio(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t arreglo[MAX_ELEMENTOS];
    bool ok = true;

    print_test("Lista esta vacia al crear...", lista_esta_vacia(lista));

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= lista_insertar_primero(lista, &arreglo[i]);
    }
   
    print_test("Lista no esta vacia al crear...", !lista_esta_vacia(lista));    

    lista_iter_avanzar(iter);
    lista_iter_borrar(iter);
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);    
}

/*********************************************
 ****** PRUEBAS ITERADOR INTERNO **********
 *********************************************/

bool imprimir(void* dato, void* extra){
    size_t* contador = (size_t*)extra;
    (*contador)++;
    return true;
}

void prueba_iterador_interno(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista); 
    size_t arreglo[MAX_ELEMENTOS]; 
    size_t contador = 0;
    bool ok = true;

    for(size_t i = 0; i < MAX_ELEMENTOS; i++){
        arreglo[i] = i;
        ok &= lista_insertar_primero(lista, &arreglo[i]);
    }

    lista_iterar(lista, imprimir, &contador);
    print_test("El largo de la lista es igual al contador", lista_largo(lista) == contador);
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);    
}

void pruebas_lista_alumno(void){
    prueba_crear_destruir();
    prueba_agregar_eliminar_elemento();
    prueba_elemento_null();
    prueba_agregar_max_elementos();
    prueba_ver_elemento_al_eliminar();
    prueba_vacia_al_crear();
    prueba_eliminar_al_crear();
    prueba_destruir_dato();
    prueba_iterador_agregar_al_inicio();
    prueba_iterador_agregar_al_final();
    prueba_agregar_elementos_medio();
    prueba_remover_elemento_al_crear();
    prueba_eliminar_elemento_final();
    prueba_eliminar_elemento_medio();
    prueba_iterador_interno();
}    
