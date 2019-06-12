#define _GNU_SOURCE
#include "count_min_sketch.h"
#include "strutil.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char* nombre;
    int cantidad;
}hashtag_t;
/************************************************************************
 *******************     FUNCIONES AUXILIARES      **********************
 ************************************************************************/
int comparar_hashtags(hashtag_t* hashtag1, hashtag_t* hashtag2){
    if(hashtag1->cantidad > hashtag2->cantidad) return 1;
    if(hashtag1->cantidad < hashtag2->cantidad) return -1;
    return 0;
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
    while(!feof(archivo)){
        printf("-------------------- %zd\n", lines);
        lines++;
        hash_t* hash = hash_crear(NULL);
        if(!hash) return;   
        //Leo cada linea del archivo .txt y relleno el hash y el cms.
        for(size_t i = 0; i < cant_lineas; i++){
            char* buffer;
            size_t size = 0;
            ssize_t line = getline(&buffer, &size, archivo);
            buffer[line-1]= '\0';
            if(line == EOF) break;
            char** vector = split(buffer, ',');
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
        size_t cantidad_elementos = hash_cantidad(hash);
        //Me creo un vector de hashtag_t para almacenar el nombre del hashtag (obtenido del hash) y la cantidad de apariciones de ese hashtag (obtenido del cms)
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
        //Una vez obtenido el vector, lo convierto a heap e imprimir el maximo cada vez que voy desencolando.
        heap_t* heap = heap_crear_arr((void**)vector_hashtags, cantidad_elementos, (cmp_func_t)comparar_hashtags);
        size_t contador = tt;
        while(!heap_esta_vacio(heap) && contador != 0){
            hashtag_t* elemento = heap_desencolar(heap);
            printf("Cantidad: %d - Hashtag: %s\n", elemento->cantidad, elemento->nombre);
            contador--;
        }
        for(size_t i = 0; i < cantidad_elementos; i++) free(vector_hashtags[i]);
        free(vector_hashtags);
        heap_destruir(heap, NULL);
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
