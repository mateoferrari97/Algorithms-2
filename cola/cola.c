#include "cola.h"
#include <stdlib.h>

/*Definicion de struct de cola y nodo_cola*/
typedef struct nodo_cola{
    void** dato;
    struct nodo_cola *siguiente;
}nodo_cola_t;

struct cola{ 
    nodo_cola_t *primero;
    nodo_cola_t *ultimo;
};

cola_t* cola_crear(void){
    cola_t* cola = malloc(sizeof(nodo_cola_t));
    if(cola == NULL) return NULL;
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->primero == NULL;
}

void* cola_ver_primero(const cola_t *cola){
    if(cola_esta_vacia(cola)) return NULL;
    return cola->primero->dato;
}

bool cola_encolar(cola_t *cola, void* valor){
    nodo_cola_t *nodo = malloc(sizeof(nodo_cola_t));
    if(nodo == NULL) return false;
    
    nodo->dato = valor;
    nodo->siguiente = NULL;

    if(cola_esta_vacia(cola)){
        cola->primero = nodo;
    }else{
        cola->ultimo->siguiente = nodo;
    }

    cola->ultimo = nodo;

    return true;
}

void* cola_desencolar(cola_t *cola){
    if(cola_esta_vacia(cola)) return NULL;
    void *datos_primer_nodo = cola->primero->dato; 
    nodo_cola_t *nodo_siguiente = cola->primero;
    cola->primero = nodo_siguiente->siguiente; 
    free(nodo_siguiente);
    return datos_primer_nodo;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
    while(!cola_esta_vacia(cola)){
        void* dato = cola_desencolar(cola);
        if(destruir_dato){
            destruir_dato(dato);
        }
    }
    free(cola);
}

