#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "strutil.h"
#include "lista.h"
#include "heap.h"
#include "hash.h"
#include "cola.h"
#define TIME_FORMAT "%FT%T%z"
#define CANTIDAD_POSIBLE_DOS 5

typedef struct{
    size_t cantidad;
    cola_t* tiempos;
}cola_ips_t;

time_t iso8601_to_time(char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

void agregar_archivo(char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return;
    char* buffer = NULL;
    char** buffer_spliteado;
    size_t tamanio = 0;
    hash_t* ips = hash_crear(NULL);
    hash_t* ips_dos = hash_crear(NULL);
    while(!feof(archivo)){
        ssize_t linea = getline(&buffer, &tamanio, archivo);
        if(linea == EOF) break;
        buffer[linea - 1] = '\0';
        buffer_spliteado = split(buffer, '\t');

        char* ip_actual = buffer_spliteado[0];
        time_t tiempo_actual = iso8601_to_time(buffer_spliteado[1]);

        if(!hash_pertenece(ips, ip_actual)){
            cola_ips_t* cola_ip = malloc(sizeof(cola_ips_t));
            cola_ip->tiempos = cola_crear();
            cola_encolar(cola_ip->tiempos, &tiempo_actual);
            cola_ip->cantidad = 1;    

            hash_guardar(ips, ip_actual, cola_ip);
        }else{
            cola_ips_t* cola_ip = hash_obtener(ips, ip_actual); 
            cola_encolar(cola_ip->tiempos, &tiempo_actual);
            cola_ip->cantidad++;
            if(cola_ip->cantidad >= 5){
                if(difftime(tiempo_actual, (time_t)cola_ver_primero(cola_ip->tiempos)) <= 2){
                    cola_desencolar(cola_ip->tiempos);
                    if(!hash_pertenece(ips_dos, ip_actual)) hash_guardar(ips_dos, ip_actual, NULL);
                }
            }
        }
    }
    hash_iter_t* hash_iter = hash_iter_crear(ips_dos);
    while(!hash_iter_al_final(hash_iter)){
        printf("%s\n", hash_iter_ver_actual(hash_iter));
        hash_iter_avanzar(hash_iter);
    }
    hash_iter_destruir(hash_iter);
    hash_destruir(ips);
    hash_destruir(ips_dos);
    fclose(archivo);
}

int main(int argc, char const *argv[]){
    agregar_archivo("access001.log"); 
    return 0;
}
