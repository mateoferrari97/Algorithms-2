#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct nodo{
    char* clave;
    void* dato;
    struct nodo* der;
    struct nodo* izq;
}nodo_t;

typedef  struct abb{
    size_t cantidad;
    nodo_t* raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
}abb_t;

nodo_t* nodo_crear(const char* clave, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->clave = clave;
    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

/*****************************************************************************
******************************************************************************
****************        FUNCIONES AUXILIARES  ABB       **********************
******************************************************************************
******************************************************************************/

nodo_t* nodo_buscar(nodo_t** padre, nodo_t* hijo, const char* clave, abb_comparar_clave_t cmp){
    if(!hijo || (cmp(hijo->clave, clave) == 0)) return hijo;
    *padre = &hijo;
    if(cmp((*padre)->clave, clave) > 0) return nodo_buscar(&padre, (*padre)->izq, clave, cmp);
    return nodo_buscar(&padre, (*padre)->der, clave, cmp);
    
}
void destruir_todo(nodo_t* actual, abb_destruir_dato_t destruir_dato){
    if(!actual) return;
    destruir_todo(actual->der, destruir_dato);
    destruir_todo(actual->izq, destruir_dato);
    nodo_destruir(actual, destruir_dato);

}
void* nodo_destruir(nodo_t* nodo, abb_destruir_dato_t destruir_dato){
    if(!nodo) return NULL;
    void* dato = nodo->dato;
    if(destruir_dato) destruir_dato(nodo->dato);
    free(nodo->clave);
    free(nodo);
    return dato;
}

/*****************************************************************************
******************************************************************************
********************        PRIMITIVAS ABB       *****************************
******************************************************************************
******************************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->cantidad = 0;
    abb->raiz =  NULL;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol) return false;
    nodo_t* padre = NULL;
    nodo_t* nodo = buscar_nodo(&padre, arbol->raiz, clave, arbol->cmp);
    if(!nodo){
        nodo_t* nodo = nodo_crear(clave, dato);
        if(!nodo) return false;
        if(!padre){
            arbol->raiz = nodo;
        }else{
            if(arbol->cmp(padre->clave, clave) > 0){
                padre->izq = nodo;
            }else{
                padre->der = nodo;
            }            
        }
        arbol->cantidad++;        
    }else{
        void* dato = nodo->dato;
        nodo->dato = dato;
        if(arbol->destruir_dato) arbol->destruir_dato(dato);
    }
    return true;
}
void *abb_borrar(abb_t *arbol, const char *clave){
    if(!arbol) return NULL;
    nodo_t* padre = NULL;
    nodo_t* nodo = nodo_buscar(&padre, arbol->raiz, clave, arbol->cmp);
    if(!nodo) return NULL;
    void* dato = nodo_destruir(nodo, arbol->destruir_dato);
    arbol->cantidad--;
    if(arbol->cantidad == 0){
        arbol->raiz = NULL;
        return dato;
    } 
    if(arbol->cmp(padre->clave, clave) > 0){
        padre->izq = NULL;
    }else{
        padre->der = NULL;
    }
    return dato;
}
void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t* padre = NULL;
    nodo_t* nodo = nodo_buscar(&padre, arbol->raiz, clave, arbol->cmp);
    if(!nodo) return NULL;
    return nodo->dato;
}
bool abb_pertenece(const abb_t *arbol, const char *clave){
    return abb_pertenece(arbol, clave) != NULL;
}
size_t abb_cantidad(abb_t *arbol){
    if(!arbol) return arbol->cantidad;
}
void abb_destruir(abb_t *arbol){
    if(arbol->cantidad != 0) destruir_todo(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}