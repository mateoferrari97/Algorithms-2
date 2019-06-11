#include "heap.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define TAMANIO_INICIAL 20
#define MODIFICACION_REDIMENSION 2
#define TAMANIO_MINIMO_AUMENTAR_REDIMENSION 1
#define TAMANIO_MINIMO_REDUCIR_REDIMENSION 0.25
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
/**********************************************************************************
 *********************      FUNCIONES AUXILIARES       ****************************
 *********************************************************************************/
bool copiar_arreglo(heap_t* heap, void** arreglo, size_t n){
    void** arreglo_nuevo = malloc(sizeof(void*) * n);
    if(!arreglo_nuevo) return false;
    free(heap->vector);
    heap->vector = arreglo_nuevo;
    for(size_t i = 0; i < n; i++) arreglo_nuevo[i] = arreglo[i];
    return true;
}
bool heap_redimensionar(heap_t* heap, size_t tamanio){
    void** vector = realloc(heap->vector, tamanio * sizeof(void*));
    if(!vector) return false;
    heap->vector = vector;
    heap->tamanio = tamanio;
    return true;
}
float calcular_redimension(heap_t* heap){
    return (float) heap->cantidad / (float) heap->tamanio;
}
void swap(void** x, void** y){
    void* aux = *x;
    *x = *y;
    *y = aux;
}
void upheap(void** arr,size_t pos, cmp_func_t cmp){
    if(pos == 0) return;
    size_t padre = (pos - 1) / 2;
    if(cmp(arr[pos], arr[padre]) > 0){
        swap(&arr[pos], &arr[padre]);
        upheap(arr, padre, cmp);
    }
}
void downheap(void** arr, size_t tam, size_t pos, cmp_func_t cmp){
    if (pos >= tam) return;
    size_t max = pos;
    size_t izq = (pos * 2) + 1;
    size_t der = (pos * 2) + 2;
    if(izq < tam && cmp(arr[max], arr[izq]) < 0) max = izq;
    if(der < tam && cmp(arr[max], arr[der]) < 0) max = der;
    if(max != pos){
        swap(&arr[pos], &arr[max]);
        downheap(arr, tam, max, cmp);
    }
}
void heapify(void** arr, size_t cantidad, cmp_func_t cmp){
    for(size_t i = (cantidad/2) - 1; i + 1 > 0; i--) downheap(arr, cantidad, i, cmp);
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
    if(calcular_redimension(heap) >= TAMANIO_MINIMO_AUMENTAR_REDIMENSION) if(!heap_redimensionar(heap, heap->tamanio * MODIFICACION_REDIMENSION)) return false;
    heap->vector[heap->cantidad] = elem;
    heap->cantidad++;
    upheap(heap->vector,heap->cantidad - 1, heap->cmp);
    return true;    
}
void *heap_desencolar(heap_t *heap){
    if(!heap || heap_esta_vacio(heap)) return NULL;
    if(calcular_redimension(heap) <= TAMANIO_MINIMO_REDUCIR_REDIMENSION && (heap->tamanio / MODIFICACION_REDIMENSION) > TAMANIO_INICIAL){
        if(!heap_redimensionar(heap, heap->tamanio / MODIFICACION_REDIMENSION)) return NULL;
    }
    void* dato = heap->vector[0];
    heap->vector[0] = heap->vector[heap->cantidad - 1];
    heap->vector[heap->cantidad - 1] = NULL;
    heap->cantidad--;
    downheap(heap->vector, heap->cantidad, 0, heap->cmp);
    return dato;
}
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = heap_crear(cmp);
    if(!heap) return NULL;
    if(!copiar_arreglo(heap, arreglo, n)){
        free(heap);
        return NULL;
    }
    heap->cantidad = n;
    heap->tamanio = n;
    heap->cmp = cmp;
    heapify(heap->vector, heap->cantidad, heap->cmp);
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
    if(destruir_elemento){
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
    for(size_t i = cant - 1; (i + 1) > 0; i--) elementos[i] = heap_desencolar(heap);
    heap_destruir(heap, NULL);
}
