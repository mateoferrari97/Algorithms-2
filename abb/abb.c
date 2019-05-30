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

/*****************************************************************************
******************************************************************************
****************        FUNCIONES AUXILIARES  ABB       **********************
******************************************************************************
******************************************************************************/
char* copiar_clave(const char* clave){
    char* clave_nueva =  malloc(sizeof(char) + 1);
    if(!clave_nueva) return NULL;
    strcpy(clave_nueva, clave);
    return clave_nueva;
}
nodo_t* nodo_crear(const char* clave, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->clave = copiar_clave(clave);
    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}
nodo_t* nodo_buscar(nodo_t** padre, nodo_t* hijo, const char* clave, abb_comparar_clave_t cmp){
    if(!hijo || (cmp(hijo->clave, clave) == 0)) return hijo;
    *padre = hijo;
    if(cmp((*padre)->clave, clave) > 0) return nodo_buscar(padre, (*padre)->izq, clave, cmp);
    return nodo_buscar(padre, (*padre)->der, clave, cmp);
    
}
void* nodo_destruir(abb_t* arbol, nodo_t* nodo, abb_destruir_dato_t destruir_dato){
    if(!nodo) return NULL;
    void* dato = nodo->dato;
    if(destruir_dato) destruir_dato(nodo->dato);
    free(nodo->clave);
    free(nodo);
    arbol->cantidad--;
    return dato;
}
void destruir_todo(nodo_t* actual, abb_destruir_dato_t destruir_dato){
    if(!actual) return;
    destruir_todo(actual->der, destruir_dato);
    destruir_todo(actual->izq, destruir_dato);
    //nodo_destruir(actual, destruir_dato);
}
void* _abb_borrar_un_hijo(abb_t* arbol, nodo_t* hijo, nodo_t* padre, const char* clave, int lado){
    if(!(hijo->der) && (hijo->izq)){
        if(!padre) arbol->raiz = hijo->der;
        else if(lado == 1) padre->der = hijo->der;
        else if(lado == 0) padre->izq = hijo->izq;
    }
}
void* abb_borrar_un_hijo(abb_t* arbol, nodo_t* hijo, nodo_t* padre, const char* clave, int lado){
    return;
}
void* _abb_borrar(abb_t* arbol, nodo_t* hijo, nodo_t* padre, const char* clave, int lado){
    if(arbol->cmp(hijo->clave, clave) > 0) return _abb_borrar(arbol, hijo->izq, hijo, clave, -1);
    if(arbol->cmp(hijo->clave, clave) < 0) return _abb_borrar(arbol, hijo->der, hijo, clave, 1);
    if(arbol->cmp(hijo->clave, clave) == 0){
        if(!(hijo->izq) && !(hijo->der)){
            if(!padre) arbol->raiz = NULL;
            else if(lado == -1) padre->izq = NULL;
            else if(lado == 1) padre->der = NULL;
            return nodo_destruir(arbol, hijo, arbol->destruir_dato);
        }
        if(!(hijo->izq) && (hijo->der) || (hijo->izq) && !(hijo->der)){
            return abb_borrar_un_hijo(arbol, hijo, padre, clave, lado);
        }
        if((hijo->izq) && (hijo->der)){
            return;
        }
    }
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
    nodo_t* nodo = nodo_buscar(&padre, arbol->raiz, clave, arbol->cmp);
    if(!nodo){
        nodo_t* nodo_nuevo = nodo_crear(clave, dato);
        if(!nodo_nuevo) return false;
        if(!padre){
            arbol->raiz = nodo_nuevo;
        }else{
            if(arbol->cmp(padre->clave, clave) > 0){
                padre->izq = nodo_nuevo;
            }else{
                padre->der = nodo_nuevo;
            }            
        }
        arbol->cantidad++;        
    }else{
        void* dato_anterior = nodo->dato;
        nodo->dato = dato;
        if(arbol->destruir_dato) arbol->destruir_dato(dato_anterior);
    }
    return true;
}
void *abb_borrar(abb_t *arbol, const char *clave){
    if(!arbol) return NULL;
    if(!abb_pertenece(arbol, clave)) return NULL;
    return _abb_borrar(arbol, arbol->raiz, NULL, clave, 0);
}
void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t* padre = NULL;
    nodo_t* nodo = nodo_buscar(&padre, arbol->raiz, clave, arbol->cmp);
    if(!nodo) return NULL;
    return nodo->dato;
}
bool abb_pertenece(const abb_t *arbol, const char *clave){
    return abb_obtener(arbol, clave) != NULL;
}
size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}
void abb_destruir(abb_t *arbol){
    if(arbol->cantidad != 0) destruir_todo(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}