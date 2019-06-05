#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define TAMANIO_INICIAL 20
#define TAMANIO_REDIMENSION 2
#define TAMANIO_MINIMO_PARA_REDIMENSION 0.4
/**********************************************************************************
 *********************      NOMBRE: MATEO FERRARI CORONEL       *******************
 *********************      PADRON: 102375                      *******************
 *********************      GRUPO: G26                          *******************
 *********************      CORRECTOR: JORGE COLLINET           *******************
 *********************************************************************************/

/**********************************************************************************
 *********************      ESTRUCTURA DE DATOS       *****************************
 *********************************************************************************/
struct heap{
    void** vector;
    size_t cantidad;
    size_t tamanio;
    cmp_func_t cmp;
};
typedef struct heap heap_t;
typedef int (*cmp_func_t) (const void *a, const void *b);
/**********************************************************************************
 *********************      FUNCIONES AUXILIARES       ****************************
 *********************************************************************************/
bool heap_redimensionar(heap_t* heap){
    void** vector = malloc((heap->tamanio * TAMANIO_REDIMENSION) * sizeof(void*));
    if(!vector) return false;
    heap->vector = vector;
    heap->tamanio = (heap->tamanio * TAMANIO_REDIMENSION);
    return true;
}
bool check_redimension(heap_t* heap){
    return (heap->cantidad / heap->tamanio) >= TAMANIO_MINIMO_PARA_REDIMENSION;
}
void swap(void** x, void** y){
    void* aux = *x;
    *x = *y;
    *y = aux;
}
void upheap(heap_t* heap, size_t pos){
    if(pos == 0) return;
    size_t padre = (pos - 1) / 2;
    if(heap->cmp(heap->vector[pos], heap->vector[padre]) > 0){
        swap(&heap->vector[pos], &heap->vector[padre]);
        upheap(heap, padre);
    }
}
void downheap(heap_t* heap, size_t pos){
    size_t limite = heap->cantidad;
    if (pos >= limite) return;
    cmp_func_t cmp = heap->cmp;
    size_t max = pos;
    size_t izq = (pos * 2) + 1;
    size_t der = (pos * 2) + 2;
    if(izq < limite && cmp(heap->vector[max], heap->vector[izq]) < 0) max = izq;
    if(der < limite && cmp(heap->vector[max], heap->vector[der]) < 0) max = der;
    if(max != pos){
        swap(&heap->vector[max], &heap->vector[pos]);
        downheap(heap, max);
    }
}
bool _heap_encolar(heap_t* heap, void* elem){
    //if(check_redimension(heap)) if(!heap_redimensionar(heap)) return false;
    heap->vector[heap->cantidad] = elem;
    upheap(heap, heap->cantidad);
    heap->cantidad++;
    return true;
}
/**********************************************************************************
 *********************      PRIMITIVAS HEAP       *********************************
 *********************************************************************************/
heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    heap->vector = malloc(sizeof(void*) * TAMANIO_INICIAL);
    if(!heap->vector){
        free(heap); 
        return NULL;
    } 
    heap->tamanio = TAMANIO_INICIAL;
    heap->cantidad = 0;
    heap->cmp = cmp;
    return heap;
}
bool heap_encolar(heap_t *heap, void *elem){
    if(!heap || !elem) return false;
    return _heap_encolar(heap, elem);
}
void *heap_desencolar(heap_t *heap){
    if(!heap || heap_esta_vacio(heap)) return NULL;
    void* dato = heap->vector[0];
    heap->vector[0] = heap->vector[heap->cantidad - 1];
    heap->vector[heap->cantidad - 1] = NULL;
    downheap(heap, heap->cantidad);
    heap->cantidad--;
    return dato;
}
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = heap_crear(cmp);
    if(!heap) return NULL;
    for(size_t i = 0; i < n; i++) if(!heap_encolar(heap, arreglo[i])) return NULL;
    return heap;
}
size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}
bool heap_esta_vacio(const heap_t *heap){
    return heap_cantidad(heap) == 0;
}
void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    return heap->vector[0];
}
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
    if(!heap) return;
    if(!heap_esta_vacio(heap) && destruir_elemento){
        for(size_t i = 0; i < heap->cantidad; i++) destruir_elemento(heap->vector[i]);
    }
    free(heap->vector);
    free(heap);
}
/**********************************************************************************
 ****************************      HEAPSORT       *********************************
 *********************************************************************************/
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    if(!elementos || !cant) return;
    heap_t* heap = heap_crear_arr(elementos, cant, cmp);
    size_t cantidad = heap->cantidad;
    while(cantidad != 0){
        elementos[cantidad] = heap_desencolar(heap);
        cantidad--;
    }
    heap_destruir(heap, NULL);
}
