#ifndef LISTA_H
#define LISTA_H

#include<stdbool.h>
#include<stddef.h>


//DEFINICION DE TIPOS
struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;


//PRIMITIVAS DE LA LISTA//

/* Crea una lista.
Post: devuelve una nueva lista vacía.*/
lista_t *lista_crear(void);

/*Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
Pre: la lista fue creada. */
bool lista_esta_vacia(const lista_t *lista);

/* Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
Pre: la lista fue creada.
Post: se agrego el dato al principio de la lista. */
bool lista_insertar_primero(lista_t *lista, void *dato);

/* Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
Pre: la lista fue creada.
Post: se agrego el dato al final de la lista. */
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/*Saca el primer elemento de la lista y se devuelve su valor; 
si está vacía, devuelve NULL.
Pre: la lista fue creada.
Post: se devolvió el valor del primer elemento anterior, la lista
contiene un elemento menos, si la lista no estaba vacía. */
void *lista_borrar_primero(lista_t *lista);

/*Obtiene el valor del primer elemento de la lista. 
Si la lista está vacía devuelve NULL.
Pre: la lista fue creada.
Post: se devolvió el primer elemento de la lista. */
void *lista_ver_primero(const lista_t *lista);

/*Obtiene el valor del ultimo elemento de la lista. 
Si la lista está vacía devuelve NULL.
Pre: la lista fue creada.
Post: se devolvió el ultimo elemento de la lista. */
void *lista_ver_ultimo(const lista_t* lista);

/* Devuelve el Largo de la lista.
Pre: Lista creada.
Post: Devuelve el largo. */
size_t lista_largo(const lista_t *lista);

/* Destruye la lista. Si se recibe la función destruir_dato por parámetro,
para cada uno de los elementos de la lista llama a destruir_dato.
Pre: la lista fue creada. destruir_dato es una función capaz de destruir
los datos de la lista, o NULL en caso de que no se la utilice.
Post: se eliminaron todos los elementos de la cola. */
void lista_destruir(lista_t *lista, void destruir_dato(void *));


//PRIMITIVAS ITERADOR EXTERNO//

/*Crea un iterador
Pre: la lista fue creada
Post: devuelve un nuevo iterador que esta apuntando al primer nodo de la lista. */
lista_iter_t *lista_iter_crear(lista_t *lista);

/*EL iterador avanza en una posicion en la lista.
Pre: el iterador fue creado.
Post: Devuele true si la posicion siguiente no es NULL, sino false. */
bool lista_iter_avanzar(lista_iter_t *iter);

/* Se obtiene el valor de donde esta apuntando el iterador.
Pre: el iterador fue creado
Post: se devuelve el valor al que apunta el iterador, 
si esta al final de lista devuelve NULL. */
void *lista_iter_ver_actual(const lista_iter_t *iter);

/*Verifica si el iterador llego al final de la lista.
Pre: el iterador fue creado.
Post: si se recorrio todo el iterador devuelve true, si no, false. */
bool lista_iter_al_final(const lista_iter_t *iter);

/*Se elimina el iterador.
Pre: el iterador fue creado. */
void lista_iter_destruir(lista_iter_t *iter);

/*Inserta un elemento en la lista.
Pre: la lista y el iterador fueron creados.
Post: Si pudo inserta un elemento en la posicion actual del iterador devuelve true, sino false. */
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/* Borra un elemento de la lista. Si no se llego a final de la lista, 
se quita el nodo al que apunta el iterador, y se devuelve su valor. Si se llega a final de la lista devuelve NULL.
Pre: la lista y el iterador fueron creados.
Post: Borra y devuelve un elemento de la lista, 
actual pasa a ser el siguiente del elemento borrado. Devuelve NULL si paso el final o esta vacia. */
void *lista_iter_borrar(lista_iter_t *iter);


//PRIMITIVAS ITERADOR INTERNO//

/* Itera la lista, mientras que la funcion booleana no es NULL, se la aplica
a cada elemento.
Pre: la lista fue creada.
Post: Itera la lista de principio a fin o hasta que visitar lo detiene. */
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);



//PRUEBAS UNITARIAS//

void pruebas_alumno(void);



#endif // LISTA_H
