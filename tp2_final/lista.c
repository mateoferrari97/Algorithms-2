#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>


//DEFINICION DE TIPOS//
typedef struct nodo{
	void *dato;
	struct nodo *prox;
}nodo_t;

struct lista{
	nodo_t *primero;
	nodo_t *ultimo;
	size_t largo;
};

struct lista_iter{
	nodo_t *anterior;
	nodo_t *actual;
	lista_t * lista;
};

//FUNCION AUX

nodo_t *nodo_crear(void *valor){
	nodo_t *nodo=malloc(sizeof(nodo_t));
	if(!nodo) return NULL;
	nodo->dato=valor;
	nodo->prox=NULL;
	return nodo;
}

//PRIMITIVAS DE LA LISTA

lista_t *lista_crear(void){
	lista_t *lista=malloc(sizeof(lista_t));
	if(!lista) return NULL;
	lista->primero=NULL;
	lista->ultimo=NULL;
	lista->largo=0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return (lista->largo==0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t *nuevo_nodo = nodo_crear(dato);
	if(!nuevo_nodo) return false;
	if(lista_esta_vacia(lista)){
		lista->ultimo=nuevo_nodo;
	}
	nuevo_nodo->prox=lista->primero;
	lista->primero=nuevo_nodo;
	lista->largo+=1;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nuevo_nodo = nodo_crear(dato);
	if(!nuevo_nodo) return false;
	if(lista_esta_vacia(lista)){
		lista->primero=nuevo_nodo;	
	}
	else{
		lista->ultimo->prox=nuevo_nodo;
	}
	lista->ultimo=nuevo_nodo;
	lista->largo+=1;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista)) return NULL;
	nodo_t *elemento=lista->primero;
	void *dato_del_primero=lista->primero->dato;
	if(lista->primero == lista->ultimo){  //SI LISTA TIENE UN SOLO ELEMENTO
		lista->primero= NULL;
		lista->ultimo= NULL;
	}
	else{
		lista->primero=elemento->prox;
	}
	lista->largo-=1;
	free(elemento);
	return dato_del_primero;
}

void *lista_ver_primero(const lista_t* lista){
	if(lista_esta_vacia(lista)) return NULL;
	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista)) return NULL;
	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while(!(lista_esta_vacia(lista))){
		void *eliminar_dato=lista_borrar_primero(lista);
		if(destruir_dato!=NULL){
			destruir_dato(eliminar_dato);	
		}
	}
	free(lista);
}

//PRIMITIVAS DEL ITERADOR XTERNO

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t * lista_iter=malloc(sizeof(lista_iter_t));
	if(!lista_iter) return NULL;
	lista_iter->actual=lista->primero;
	lista_iter->anterior=NULL;
	lista_iter->lista=lista;
	return lista_iter;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->actual==NULL;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(lista_iter_al_final(iter)) return false;
	iter->anterior=iter->actual;
	iter->actual=iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(lista_iter_al_final(iter)) return NULL;
	return iter->actual->dato; 
}



void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if(iter->actual == iter->lista->primero){
		if(!(lista_insertar_primero(iter->lista,dato))) return false;
		iter->actual=iter->lista->primero;
	}
	else if(lista_iter_al_final(iter)){
		if(!(lista_insertar_ultimo(iter->lista,dato))) return false;
		iter->actual=iter->lista->ultimo;
	}
	else {
	nodo_t* insertar_nodo = nodo_crear(dato);
	if(!insertar_nodo) return false;
	iter->anterior->prox=insertar_nodo;
	insertar_nodo->prox=iter->actual;
	iter->actual=insertar_nodo;
	iter->lista->largo+=1;
	}
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
	if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) return NULL;
	nodo_t *elemento=iter->actual;
	void *dato_a_eliminar=iter->actual->dato;
	if(iter->actual==iter->lista->primero){ 
		iter->actual=elemento->prox;
		iter->lista->primero=iter->actual;
	}
	else {
		if(iter->actual==iter->lista->ultimo){
		iter->lista->ultimo=iter->anterior;
		}
	iter->anterior->prox=elemento->prox;
	iter->actual=elemento->prox;
	}
	iter->lista->largo-=1;
	free(elemento);
	return dato_a_eliminar;
}

//PRIMITIVAS DEL ITERADOR INTERNO

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if(visitar == NULL) return;
	nodo_t *act=lista->primero;
	bool visitar_todo_ok = true;
	while(act != NULL && visitar_todo_ok){
		visitar_todo_ok=visitar(act->dato,extra);
		act=act->prox;
	}
	return;
}
