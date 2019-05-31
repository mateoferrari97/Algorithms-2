#include "pila.h"
#include <stdlib.h>
#define TAMANIO_REDIMENSION 2
#define CAPACIDAD_INICIAL 10
#define CANTIDAD_INICIAL 0

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(){
    pila_t* pila = malloc(sizeof(pila_t));
    if(pila == NULL){
        return NULL;
    }

    void** datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));

    if(datos == NULL){
        free(pila);
        return NULL;
    }

    pila->datos = datos;
    pila->cantidad = CANTIDAD_INICIAL;
    pila->capacidad = CAPACIDAD_INICIAL;

    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

void* pila_ver_tope(const pila_t *pila){
    if(pila->cantidad == 0){
        return NULL;
    }

    return pila->datos[pila->cantidad-1];
}

bool pila_redimensionar(pila_t* pila, size_t tamanio){
    void *nuevos_datos = realloc(pila->datos, (pila->capacidad * tamanio) * sizeof(void*));
    if(nuevos_datos == NULL){
        return false;
    }

    pila->datos = nuevos_datos;
    pila->capacidad = pila->capacidad * tamanio;
    return true;
}

bool pila_apilar(pila_t *pila, void* valor){
    if(pila->cantidad == pila->capacidad){
        if(!pila_redimensionar(pila, TAMANIO_REDIMENSION)){
            return false;
        }
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void* pila_desapilar(pila_t *pila){
    if(pila->cantidad == 0){
        return NULL;
    }
    
    if(pila->cantidad * 4 == pila->capacidad){
        if(!pila_redimensionar(pila, pila->cantidad / TAMANIO_REDIMENSION)){
            return NULL;
        }
    }

    pila->cantidad--;
    return pila->datos[pila->cantidad];
}
