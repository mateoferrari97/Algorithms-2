#include "lista.h"
#include "hash.h"
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#define TAMANIO_INICIAL 97
#define TAMANIO_AUMENTO_REDIMENSION 2
#define TAMANIO_REDUCCION_REDIMENSION 2

struct hash{
    lista_t** arreglo;
    size_t cantidad;
    size_t capacidad;
    hash_destruir_dato_t f_destruir;
};

struct hash_iter{
    const hash_t* hash;
    size_t actual;
    lista_iter_t* lista_iter;
};

typedef struct nodo_hash{
    void* dato;
    char* clave;
}nodo_hash_t;

/**************************
***************************
    FUNCIONES AUXILIARES
***************************
***************************
***************************/

void inicializar_hash(hash_t* hash, size_t tamanio){
    for(size_t i = 0; i < tamanio; i++){
        hash->arreglo[i] = lista_crear();
    }
}
size_t hashing(const char* clave, size_t tam){
	//unsigned char *p = clave;
	size_t hashval;
    for(hashval = 0; *clave != '\0'; clave++){
        hashval = *clave + 31 * hashval;
    }
    return hashval % tam;
}
char* copiar_clave(const char* clave){
    char* copia = malloc(strlen(clave) + 1);
    if(!copia) return NULL;
    strcpy(copia, clave);
    return copia;
}
nodo_hash_t* crear_nodo(const char* clave, void* dato){
    nodo_hash_t* nodo_nuevo = malloc(sizeof(nodo_hash_t));
    if(!nodo_nuevo) return NULL;
    nodo_nuevo->clave = copiar_clave(clave);
    nodo_nuevo->dato = dato;
    return nodo_nuevo;
}
nodo_hash_t* nodo_buscar(const hash_t* hash,const char* clave){
    size_t pos = hashing(clave, hash->capacidad);
    lista_iter_t* lista_iter = lista_iter_crear(hash->arreglo[pos]);
    if(!lista_iter) return NULL;
    nodo_hash_t* nodo_aux;
    while(!lista_iter_al_final(lista_iter)){
        nodo_aux = lista_iter_ver_actual(lista_iter);
        if(strcmp(nodo_aux->clave, clave) == 0){
            lista_iter_destruir(lista_iter);
            return nodo_aux;
        }
        lista_iter_avanzar(lista_iter);
    }
    lista_iter_destruir(lista_iter);
    return NULL;
}
bool pasar_siguiente_arreglo(const hash_t* hash, size_t* pos){
    while(*pos != hash->capacidad){
        if(!lista_esta_vacia(hash->arreglo[*pos])) return true;
        (*pos)++;
    }
    return false;
}
float factor_carga(hash_t* hash){
    return (float) hash->cantidad / (float) hash->capacidad;
}

bool hash_redimensionar(hash_t* hash, bool tipo_redimension){
    size_t nueva_capacidad;
    if(tipo_redimension) nueva_capacidad = hash->capacidad * TAMANIO_AUMENTO_REDIMENSION;
    else nueva_capacidad = hash->capacidad / TAMANIO_REDUCCION_REDIMENSION;
    lista_t** arreglo_nuevo = malloc(nueva_capacidad * sizeof(lista_t*));
    if(!arreglo_nuevo) return false;
    for(size_t i = 0; i < nueva_capacidad; i++) arreglo_nuevo[i] = lista_crear();
    for(size_t i = 0; i < hash->capacidad; i++){
        while(!lista_esta_vacia(hash->arreglo[i])){
            nodo_hash_t* nodo = lista_borrar_primero(hash->arreglo[i]);
            size_t pos = hashing(nodo->clave, nueva_capacidad);
            lista_insertar_primero(arreglo_nuevo[pos], nodo);
        }
        lista_destruir(hash->arreglo[i], NULL);
    }
    free(hash->arreglo);
    hash->capacidad = nueva_capacidad;
    hash->arreglo = arreglo_nuevo;
    return true;
}

/**************************
***************************
    PRIMITIVAS DE HASH
***************************
***************************
***************************/
hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    hash->arreglo = malloc(TAMANIO_INICIAL * sizeof(lista_t*));
    if(!hash->arreglo){
        free(hash);
        return NULL;
    }
    inicializar_hash(hash, TAMANIO_INICIAL);
    hash->capacidad = TAMANIO_INICIAL;
    hash->cantidad = 0;
    hash->f_destruir = destruir_dato;
    return hash;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato){
    if(!hash) return false;
    if(factor_carga(hash) >= 0.4){
        if(!hash_redimensionar(hash, true)) return false;
    }
    if(hash_pertenece(hash,clave)){
        nodo_hash_t* nodo_existente = nodo_buscar(hash, clave);
        if(hash->f_destruir) hash->f_destruir(nodo_existente->dato);
        nodo_existente->dato = dato;
    }else{
        nodo_hash_t* nodo = crear_nodo(clave, dato);
        size_t pos = hashing(clave, hash->capacidad);
        if(!lista_insertar_primero(hash->arreglo[pos], nodo)){
            free(nodo->clave);
            return false;
        }
        hash->cantidad++;
    }
    return true;
}

void* hash_obtener(const hash_t* hash, const char* clave){
    if(hash->cantidad == 0) return NULL;
    size_t clave_pos = hashing(clave, hash->capacidad);
    if(lista_esta_vacia(hash->arreglo[clave_pos])) return NULL;
    lista_iter_t* lista_iter = lista_iter_crear(hash->arreglo[clave_pos]);
    if(!lista_iter) return NULL;
    nodo_hash_t* hash_nodo;
    while(!lista_iter_al_final(lista_iter)){
        hash_nodo = lista_iter_ver_actual(lista_iter);
        if(strcmp(hash_nodo->clave, clave) == 0){
            lista_iter_destruir(lista_iter);
            return hash_nodo->dato;
        }
        lista_iter_avanzar(lista_iter);      
    }
    lista_iter_destruir(lista_iter);
    return NULL;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if(!hash) return NULL;   
    if(factor_carga(hash) <= 0.3){
        if(!hash_redimensionar(hash, false)) return NULL;
    }
    nodo_hash_t* nodo_hash = nodo_buscar(hash, clave);
    if(!nodo_hash) return NULL;
    size_t pos = hashing(clave, hash->capacidad);
    lista_iter_t* lista_iter = lista_iter_crear(hash->arreglo[pos]);
    if(!lista_iter) return NULL;
    while(lista_iter_ver_actual(lista_iter) != nodo_hash){
        lista_iter_avanzar(lista_iter);
    } 
    void* dato = nodo_hash->dato;
    free(nodo_hash->clave);
    free(nodo_hash);
    lista_iter_borrar(lista_iter);
    lista_iter_destruir(lista_iter);
    hash->cantidad--;
    return dato;
}

void hash_destruir(hash_t *hash){
    nodo_hash_t* nodo_aux;
    for(size_t i = 0; i < hash->capacidad; i++){
        while(!lista_esta_vacia(hash->arreglo[i])){
            nodo_aux = lista_borrar_primero(hash->arreglo[i]);
            free(nodo_aux->clave);
            if(hash->f_destruir) hash->f_destruir(nodo_aux->dato);
            free(nodo_aux);
        }
        lista_destruir(hash->arreglo[i], NULL);
    }
    free(hash->arreglo);
    free(hash);
}

bool hash_pertenece(const hash_t* hash, const char* clave){
    return nodo_buscar(hash, clave);
}

size_t hash_cantidad(const hash_t* hash){
    return hash->cantidad;
}
//ITERADOR DE HASH

hash_iter_t *hash_iter_crear(const hash_t *hash){
    if(!hash) return NULL;
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;
	iter->hash = hash;
	iter->actual = 0;
    if(pasar_siguiente_arreglo(hash, &iter->actual)){
        iter->lista_iter = lista_iter_crear(hash->arreglo[iter->actual]);
    }else{
        iter->lista_iter = NULL;
    }
	return iter;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    if(!iter || iter->hash->cantidad == 0) return true;
    size_t pos = iter->actual;
    return (!pasar_siguiente_arreglo(iter->hash, &pos) && lista_iter_al_final(iter->lista_iter));
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if(!iter || !iter->lista_iter) return false;
	if(hash_iter_al_final(iter)) return false;
    lista_iter_avanzar(iter->lista_iter);
    if(!lista_iter_al_final(iter->lista_iter)) return true;
    iter->actual++;
    if(pasar_siguiente_arreglo(iter->hash, &iter->actual)){
        lista_iter_destruir(iter->lista_iter);
        iter->lista_iter = lista_iter_crear(iter->hash->arreglo[iter->actual]);     
        return true;
    };
	return false;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(hash_iter_al_final(iter)) return NULL;
	nodo_hash_t *nodo=lista_iter_ver_actual(iter->lista_iter);
	return nodo->clave;
}

void hash_iter_destruir(hash_iter_t* iter){
    if(iter){
        if(iter->lista_iter) lista_iter_destruir(iter->lista_iter);
        free(iter);
    }
}
