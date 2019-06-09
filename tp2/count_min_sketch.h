#include <stdbool.h>
#include <stddef.h>
/************************************************************************
 *******************     ESTRUCTURA DE DATOS      ***********************
 ************************************************************************/
typedef struct cms cms_t;
typedef size_t (*hashing_funcion_t)(char* clave, size_t tamanio);
/************************************************************************
 *******************       PRIMITIVAS CMK       *************************
 ************************************************************************/

//Post: Devuelve el CMS creado, en caso de no poder hacerlo, devuelve NULL.
cms_t* cms_crear(hashing_funcion_t func1, hashing_funcion_t func2, hashing_funcion_t func3);

//Pre: Existan la clave y el CMS.
//Post: Devuelve TRUE si pudo aumentar el contador en cada vector dependiendo la posicion que devolvio la funcion de hashing, en caso contrario FALSE.
bool cms_insertar(cms_t* cms, char* clave);

//Pre: Existan la clave y el CMS.
//Post: Devuelve la cantidad de apariciones de una clave, en caso contrario -1.
int cms_apariciones(cms_t* cms, char* clave);

//Pre: Exista el CMS.
//Post: Destruye el CMS.
void cms_destruir(cms_t* cms);
