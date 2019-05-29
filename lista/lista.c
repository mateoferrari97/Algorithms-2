#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct nodo{
    struct nodo* siguiente;
    void* dato;
}nodo_t;

typedef struct lista{
    size_t largo;
    nodo_t* primero;
    nodo_t* ultimo;
}lista_t;

typedef struct lista_iter{
    lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior;
}lista_iter_t;

/*********************************************
 ************ FUNCIONES AUXILIARES ***********
 *********************************************/

nodo_t* nodo_crear(void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->dato = dato;
    return nodo;
}

/*********************************************
 ************ PRIMITIVAS LISTA ***************
 *********************************************/

lista_t* lista_crear(){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista) return NULL;
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t* lista){
    return !lista->primero;
}

bool lista_insertar_primero(lista_t* lista, void* dato){
    nodo_t* nodo = nodo_crear(dato);
    if(!nodo) return false;
    if(lista_esta_vacia(lista)){
        lista->ultimo = nodo;
        nodo->siguiente = NULL;
    }
    nodo->siguiente = lista->primero;
    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato){
    if(lista_esta_vacia(lista)) return lista_insertar_primero(lista, dato);
    nodo_t* nodo = nodo_crear(dato);
    if(!nodo) return false;
    nodo->siguiente = NULL;
    lista->ultimo->siguiente = nodo;
    lista->ultimo = nodo;
    lista->largo++;        
    return true;    
}

void* lista_borrar_primero(lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    nodo_t* nodo = lista->primero;
    void* dato = nodo->dato;
    lista->primero = lista->primero->siguiente;
    free(nodo);
    if(lista_esta_vacia(lista)) lista->ultimo = lista->primero;
    lista->largo--;
    return dato;
}

void* lista_ver_primero(const lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    return lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    return lista->ultimo->dato;
}

void lista_destruir(lista_t* lista, void destruir_dato(void*)){
    while(!lista_esta_vacia(lista)){
        void* dato = lista_borrar_primero(lista);
        if(destruir_dato) destruir_dato(dato);
    }

    free(lista);
}

size_t lista_largo(const lista_t* lista){
    return lista->largo;
}

/*********************************************
 ****** PRIMITIVAS ITERADOR EXTERNO **********
 *********************************************/

lista_iter_t* lista_iter_crear(lista_t* lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if(iter == NULL)return NULL;
    iter->anterior = NULL;
    iter->actual = lista->primero;
    iter->lista = lista;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(!iter->actual) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->siguiente;
    return true;
}
bool lista_iter_al_final(const lista_iter_t *iter){
    return !iter->actual;
}
void* lista_iter_ver_actual(const lista_iter_t* iter){
    if(lista_iter_al_final(iter)) return NULL;
    return iter->actual->dato;
}
void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    if(!iter->anterior){
        if(!lista_insertar_primero(iter->lista, dato)) return false;
        iter->actual = iter->lista->primero;
        iter->anterior = NULL;
        return true;
    }
    if(lista_iter_al_final(iter)){
        if(!lista_insertar_ultimo(iter->lista, dato)) return false;
        iter->actual = iter->lista->ultimo;
        return true;
    }     
    nodo_t* nodo = nodo_crear(dato);
    if(!nodo) return false;
    nodo->siguiente = iter->actual;
    iter->anterior->siguiente = nodo;
    iter->actual = nodo;
    iter->lista->largo++;
    return true;
}

void* lista_iter_borrar(lista_iter_t* iter){
    if(lista_iter_al_final(iter) || lista_esta_vacia(iter->lista)) return NULL;
    if(!iter->anterior){
        void* dato = lista_borrar_primero(iter->lista);
        iter->actual = iter->lista->primero;
        return dato;
    }
    nodo_t* actual = iter->actual;
    nodo_t* anterior = iter->anterior;
    void* dato = actual->dato;
    anterior->siguiente = actual->siguiente;
    iter->actual = actual->siguiente;
    if(!actual->siguiente) iter->lista->ultimo = iter->anterior;
    free(actual);
    iter->lista->largo--;
    return dato; 
} 

/*********************************************
 ************* ITERADOR INTERNO **************
 *********************************************/

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra){
    nodo_t* nodo = lista->primero;
    bool ok = true;
    while(ok && nodo != NULL){
        ok = visitar(nodo->dato, extra);
        nodo = nodo->siguiente;
    }
}