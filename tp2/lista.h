#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                PRIMITIVAS DE LISTA
 * *****************************************************************/

//Crea una lista
//Post: devuelve una lista vacia.
lista_t* lista_crear();

// Pre: la lista fue creada.
// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
bool lista_esta_vacia(const lista_t* lista);

// Pre: la lista fue creada.
// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al prinicipio
// de la lista.
bool lista_insertar_primero(lista_t* lista, void* dato);

// Pre: la lista fue creada.
// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Pre: la lista fue creada.
// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void* lista_borrar_primero(lista_t* lista);

// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento de la lista, en caso de no estar vacia.
void* lista_ver_primero(const lista_t* lista);

// Pre: la lista fue creada.
// Post: se devolvió el valor del ultimo elemento de la lista, en caso de no estar vacia.
void* lista_ver_primero(const lista_t* lista);

// Pre: la lista fue creada.
// Post: se devolvió el largo de la lista.
size_t lista_largo(const lista_t* lista);

//Pre: la lista fue creada.
//Post: se eliminaron todos los elementos de la lista y la lista.
void lista_destruir(lista_t* lista, void destruir_dato(void*));

/* ******************************************************************
 *          PRIMITIVAS DE ITERADORES INTERNO Y EXTERNO
 * *****************************************************************/

//Pre: la lista fue creada.
//Post: crea un iterador. Apunta a la lista, primero de la lista y ultimo de la lista.
lista_iter_t* lista_iter_crear(lista_t* lista);

//Pre: el iterador fue creado.
//Post Devuelve true si la posisicion siguiente a actual no es NULL.
bool lista_iter_avanzar(lista_iter_t* iter);

//Pre: el iterador fue creado.
//POst: devuelve el elemento actual del iterador.
void* lista_iter_ver_actual(const lista_iter_t* iter);

//Pre: el iterador fue creado.
//Post: devuelve true si el iterador recorrio toda la lista, en caso contrario false.
bool lista_iter_al_final(const lista_iter_t* iter);

//Pre: el iterador fue creado.
//Post: se eliminar el iterador.
void lista_iter_destruir(lista_iter_t* iter);

//Pre: el iterador y la lista fueron creados.
//Post:devuelve true si inserto un elemento en la lista, sino false.
bool lista_iter_insertar(lista_iter_t* iter, void* dato);

//Pre: el iterador y la lista fueron creados.
//Post: se devuelve el valor del elemento borrado.
void* lista_iter_borrar(lista_iter_t* iter);

//Pre: lista fue creada.
//Post: Itera la lista hasta el final o hasta que visitar lo detenga.
void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra);

void pruebas_lista_alumno(void);

#endif //LISTA_H
