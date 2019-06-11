#define _GNU_SOURCE
#include "count_min_sketch.h"
#include "strutil.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tweet{
    char* nombre;
    int cantidad;
}hashtag_t;
/************************************************************************
 *******************     FUNCIONES AUXILIARES      **********************
 ************************************************************************/
int comparar_hashtags(hashtag_t* hashtag1, hashtag_t* hashtag2){
    return (hashtag1->cantidad > hashtag2->cantidad) ? hashtag1->cantidad : hashtag2->cantidad;
}
/************************************************************************
 *******************     FUNCIONES DE HASHING      **********************
 ************************************************************************/
size_t hashing1(char* clave, size_t tam){
	size_t hashval;
    for(hashval = 0; *clave != '\0'; clave++){
        hashval = *clave + 31 * hashval;
    }
    return hashval % tam;
}
size_t hashing2(char* clave, size_t tam) {
	size_t hash = 0;
	size_t i = 0;
	while (clave[i] != '\0') {
		hash = (hash << 2) ^ (hash >> 14) ^ (unsigned long int) clave[i];
		i++;
	}
	return hash%tam;
}
size_t hashing3(char* clave, size_t tam) {
	unsigned int num1 = 378551;
	unsigned int num2 = 63689;
	unsigned int clave_numerica = 0;
	for (int i = 0; *clave; clave++, i++) {
		clave_numerica = clave_numerica * num2 + (*clave);
		num2 = num2 * num1;
	}
	return clave_numerica%tam;
}

void procesar_tweets(FILE* archivo, size_t cant_lineas, size_t tt){
    if(!archivo) return;
    cms_t* cms = cms_crear(hashing1, hashing2, hashing3);
    if(!cms) return;
    size_t lines = 1;
    //Relleno el hash con cada string obtenido del .txt y aplico lo mismo con cms.
    while(!feof(archivo)){
        printf("-------------------- %zd\n", lines);
        lines++;
        hash_t* hash = hash_crear(NULL);
        if(!hash) return;   
        for(size_t i = 0; i < cant_lineas; i++){
            char* buffer;
            size_t size = 0;
            ssize_t line = getline(&buffer, &size, archivo);
            if(line == EOF) continue;
            char** vector = split(buffer, ','); //Devuelve un malloc. Acordarse de un free.
            size_t i = 0;
            while(vector[i]){ 
                if(!hash_pertenece(hash, vector[i])) if(hash_guardar(hash, vector[i], NULL));
                cms_insertar(cms, vector[i]);
                free(vector[i]);
                i++;
            }
            free(vector);
        }
        //Recorro el hash con un iterador para hacer la fusion con el cms y generar el heap
        hash_iter_t* hash_iter = hash_iter_crear(hash);
        if(!hash_iter){
            hash_destruir(hash);
            return;
        }
        size_t cantidad_elementos = hash_cantidad(hash);
        hashtag_t** vector_hashtags = malloc(sizeof(hashtag_t*) * cantidad_elementos);
        if(!vector_hashtags) return;
        for(size_t i = 0; i < cantidad_elementos; i++){
            hashtag_t* hashtag = malloc(sizeof(hashtag_t));
            char* actual = hash_iter_ver_actual(hash_iter);
            hashtag->nombre = actual;
            hashtag->cantidad = cms_apariciones(cms, actual);
            vector_hashtags[i] = hashtag;
            hash_iter_avanzar(hash_iter);
        }
        heap_t* heap = heap_crear_arr((void**)vector_hashtags, cantidad_elementos, (cmp_func_t)comparar_hashtags);
        while(!heap_esta_vacio(heap)){
            hashtag_t* elemento = heap_desencolar(heap);
            printf("Cantidad: %d - Hashtag: %s\n", elemento->cantidad, elemento->nombre);
        }
        hash_iter_destruir(hash_iter);
        hash_destruir(hash);
    }
    cms_destruir(cms);
}
int main(int argc, char const *argv[]){
    if(argc != 3) return -1;
    procesar_tweets(stdin, atoi(argv[1]), atoi(argv[2])); //procesar_tweets(n,k)
    return 0;
}
