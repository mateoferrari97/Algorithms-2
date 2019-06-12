#include "count_min_sketch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define TAMANIO_INICIAL 500
#define CANTIDAD_FUNCIONES_HASHING 3
/************************************************************************
 *******************     ESTRUCTURA DE DATOS      ***********************
 ************************************************************************/
struct cms{
    int** vector;
    size_t tamanio;
    hashing_funcion_t func1;
    hashing_funcion_t func2;
    hashing_funcion_t func3;
};
/************************************************************************
 *******************     FUNCIONES AUXILIARES      **********************
 ************************************************************************/
bool rellenar_vector_cms(int** vector_vectores){
    if(!vector_vectores) return false;
    for(size_t i = 0; i < CANTIDAD_FUNCIONES_HASHING; i++){
        int* vector = calloc(TAMANIO_INICIAL, sizeof(int));
        if(!vector) return false;
        vector_vectores[i] = vector;
    }
    return true;
}
void aumentar_contador(int* vector, char* clave, hashing_funcion_t func){
    size_t pos_obtenida = func(clave, TAMANIO_INICIAL);
    vector[pos_obtenida]++;
}
void encontrar_minima_aparicion(char* clave, size_t tamanio, int* min, int* vector, hashing_funcion_t func){
    int contador = vector[func(clave, tamanio)];
    if(contador < *min) *min = contador;
}
/************************************************************************
 *******************       PRIMITIVAS CMK       *************************
 ************************************************************************/
cms_t* cms_crear(hashing_funcion_t func1, hashing_funcion_t func2, hashing_funcion_t func3){
    if(!func1 || !func2 || !func3) return NULL;
    cms_t* cms = malloc(sizeof(cms_t));
    if(!cms) return NULL;
    int** vector = malloc(sizeof(int*) * CANTIDAD_FUNCIONES_HASHING);
    if(!rellenar_vector_cms(vector)) return NULL;
    cms->vector = vector;
    cms->tamanio = TAMANIO_INICIAL;
    cms->func1 = func1;
    cms->func2 = func2;
    cms->func3 = func3;
    return cms;
}
bool cms_insertar(cms_t* cms, char* clave){
    if(!cms || !clave) return false;
    aumentar_contador(cms->vector[0], clave, cms->func1);
    aumentar_contador(cms->vector[1], clave, cms->func2);
    aumentar_contador(cms->vector[2], clave, cms->func3);
    return true;
}
int cms_apariciones(cms_t* cms, char* clave){
    if(!cms || !clave) return -1;
    size_t tamanio = cms->tamanio;
    int min = cms->vector[0][cms->func1(clave, tamanio)];
    encontrar_minima_aparicion(clave, tamanio, &min,  cms->vector[1], cms->func2);
    encontrar_minima_aparicion(clave, tamanio, &min,  cms->vector[2], cms->func3);
    return min;
}
void cms_destruir(cms_t* cms){
    if(!cms) return;
    for(size_t i = 0; i < CANTIDAD_FUNCIONES_HASHING; i++) free(cms->vector[i]);
    free(cms->vector);
    free(cms);
}


