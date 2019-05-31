#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*****************************************************************************
*******************    ALUMNO: MATEO FERRARI CORONEL   ***********************
*******************    PADRON: 102375                  ***********************
*******************    GRUPO: G26                      ***********************
*******************    CORRECTOR: JORGE COLLINET       ***********************
******************************************************************************/

/*****************************************************************************
******************************************************************************
*********************   ESTRUCTURA DE DATOS ABB    ***************************
******************************************************************************
******************************************************************************/
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
typedef struct abb_iter{
    pila_t* pila;
    nodo_t* actual;
}abb_iter_t;
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
void* nodo_destruir(abb_t* arbol, nodo_t* nodo){
    if(!nodo) return NULL;
    void* dato = nodo->dato;
    if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
    free(nodo->clave);
    free(nodo);
    arbol->cantidad--;
    return dato;
}
void destruir_todo(abb_t* arbol, nodo_t* actual){
    if(!actual) return;
    destruir_todo(arbol, actual->der);
    destruir_todo(arbol, actual->izq);
    nodo_destruir(arbol, actual);
}
void* abb_borrar_un_hijo(abb_t* arbol, nodo_t* hijo, nodo_t* padre, int lado){
    if(!hijo->der && hijo->izq){
        if(!padre) arbol->raiz = hijo->izq;
        else if(lado == -1) padre->izq = hijo->izq;
        else if(lado == 1) padre->der = hijo->izq;
        return nodo_destruir(arbol, hijo);
    } 
    if(!hijo->izq && hijo->der){
        if(!padre) arbol->raiz = hijo->der;
        else if(lado == -1) padre->izq = hijo->der;
        else if(lado == 1) padre->der = hijo->der;
        return nodo_destruir(arbol, hijo);        
    }
    return NULL;
}
void* abb_borrar_dos_hijos(abb_t* arbol, nodo_t* hijo, nodo_t* padre, int lado){
    nodo_t* padre_reemplazante = NULL;
    nodo_t* aux = hijo->izq;
    while(aux->der != NULL){
        padre_reemplazante = aux;
        aux = aux->der;
    }     
    nodo_t* reemplazante;
    if(padre_reemplazante){
        reemplazante = padre_reemplazante->der;
        padre_reemplazante->der = reemplazante->izq;
        reemplazante->der = hijo->der;
        reemplazante->izq = hijo->izq;
    }else{
        reemplazante = hijo->izq;
        reemplazante->der = hijo->der;
    }
    if(!padre) arbol->raiz = reemplazante;
    else if(lado == -1) padre->izq = reemplazante;
    else if(lado == 1) padre->der = reemplazante;
    return nodo_destruir(arbol, hijo);
}
void* _abb_borrar(abb_t* arbol, nodo_t* hijo, nodo_t* padre, const char* clave, int lado){
    if(arbol->cmp(hijo->clave, clave) < 0) return _abb_borrar(arbol, hijo->der, hijo, clave, 1);
    if(arbol->cmp(hijo->clave, clave) > 0) return _abb_borrar(arbol, hijo->izq, hijo, clave, -1);
    if(arbol->cmp(hijo->clave, clave) == 0){
        if(!hijo->izq && !hijo->der){
            if(!padre) arbol->raiz = NULL;
            else if(lado == -1) padre->izq = NULL;
            else if(lado == 1) padre->der = NULL;
            return nodo_destruir(arbol, hijo);
        }
        if((!hijo->izq && hijo->der) || (hijo->izq && !hijo->der)){
            return abb_borrar_un_hijo(arbol, hijo, padre, lado);
        }
        if(hijo->izq && hijo->der){
            return abb_borrar_dos_hijos(arbol, hijo, padre, lado);
        }
    }
    return NULL;
}
void apilar_hijos_izquierdos(pila_t* pila, nodo_t* actual){
    if(!actual->izq) return;
    pila_apilar(pila, actual->izq);
    apilar_hijos_izquierdos(pila, actual->izq);
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
    nodo_t* padre = NULL;
    return nodo_buscar(&padre, arbol->raiz, clave, arbol->cmp);
}
size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}
void abb_destruir(abb_t *arbol){
    if(abb_cantidad(arbol) != 0) destruir_todo(arbol,arbol->raiz);
    free(arbol);
}
/*****************************************************************************
******************************************************************************
********************        ITERADOR EXTERNO ABB       ***********************
******************************************************************************
******************************************************************************/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    if(!arbol) return NULL;
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;
    iter->pila = pila_crear();
    if(!iter->pila) return NULL;
    if(arbol->cantidad != 0){
        pila_apilar(iter->pila, arbol->raiz);
        apilar_hijos_izquierdos(iter->pila, arbol->raiz);
    } 
    iter->actual = pila_ver_tope(iter->pila);
    return iter;
}
bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return false;
    nodo_t* nodo = pila_desapilar(iter->pila);
    if(nodo->der){
        pila_apilar(iter->pila, nodo->der);
        apilar_hijos_izquierdos(iter->pila, nodo->der);
    }
    if(pila_ver_tope(iter->pila)) iter->actual = pila_ver_tope(iter->pila);
    return true;
}
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(pila_ver_tope(iter->pila)) return pila_ver_tope(iter->pila);
    return NULL;
}
bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}
void abb_iter_in_destruir(abb_iter_t* iter){
    if(!iter) return;
    pila_destruir(iter->pila);
    free(iter);
}
/*****************************************************************************
******************************************************************************
********************        ITERADOR INTERNO ABB       ***********************
******************************************************************************
******************************************************************************/
void _abb_in_order(abb_t* arbol, nodo_t* actual, bool visitar(const char*, void*, void*), void* extra){
    if(!actual) return;
    _abb_in_order(arbol, actual->izq, visitar, extra);
    visitar(actual->clave, actual->dato, extra);
    _abb_in_order(arbol, actual->der, visitar, extra);
}
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra){
    if(!arbol) return;
    _abb_in_order(arbol, arbol->raiz, visitar, extra);
}