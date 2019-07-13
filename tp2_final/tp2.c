#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "strutil.h"
#include "lista.h"
#include "heap.h"
#include "hash.h"
#include "cola.h"
#define TIME_FORMAT "%FT%T%z"
#define CANTIDAD_POSIBLE_DOS 5
#define MAXIMO_INDICE_IPS_SPLITEADO 3

typedef struct{
    size_t cantidad;
    cola_t* tiempos;
}cola_ips_t;

time_t iso8601_to_time(char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

bool es_valida(char* ip, char* rango_inicial, char* rango_final){
    char** rango_inicial_spliteado = split(rango_inicial, '.');
    char** rango_final_spliteado = split(rango_final, '.');
    char** ip_spliteada = split(ip, '.');
    for(size_t i = 0; i < MAXIMO_INDICE_IPS_SPLITEADO; i++){
        int ip_valor = atoi(ip_spliteada[i]);
        int rango_inicial_valor = atoi(rango_inicial_spliteado[i]);
        int rango_final_valor = atoi(rango_final_spliteado[i]);
        if(ip_valor >= rango_inicial_valor && ip_valor <= rango_final_valor) return true;
    }
    return false;
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

void ver_visitantes(char* rango_inicial, char* rang_final){
    FILE* archivo = fopen("access001.log", "r");
    if(!archivo) return;
    char* buffer = NULL, *ip_actual;
    char** buffer_spliteado;
    size_t tamanio = 0;
    ssize_t linea;
    hash_t* ips_validas = hash_crear(NULL);
    while(!feof(archivo)){
        linea = getline(&buffer, &tamanio, archivo);
        if(linea == EOF) break;
        buffer[linea - 1] = '\0';
        buffer_spliteado = split(buffer, '\t');
        ip_actual = buffer_spliteado[0];
        if(es_valida(ip_actual, rango_inicial, rang_final)){  //Valida que la ip actual procesada esté en el rango correspondiente.
            if(!hash_pertenece(ips_validas, ip_actual)) hash_guardar(ips_validas, ip_actual, NULL);
        }
        free_strv(buffer_spliteado);
    }
    hash_iter_t* iter = hash_iter_crear(ips_validas);
    printf("Visitantes:\n");
    while(!hash_iter_al_final(iter)){
        printf("\t%s\n", hash_iter_ver_actual(iter));
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    free(buffer);
    fclose(archivo);
}

void menu(char** input){
    char* comando, *parametro1, *parametro2;
    if(input[1]) comando = input[1]; //Comando a ejectuar
    if(input[2]) parametro1 = input[2]; //Posible parametro pasado. Ejemplo: ip_inicial (ver_visitantes()).
    if(input[3]) parametro2 = input[3]; //Posible parametro pasado. Ejemplo: ip_final (ver_visitantes()).

    if(strcmp(comando, "agregar_archivo") == 0) agregar_archivo(parametro1);
    else if(strcmp(comando, "ver_visitantes") == 0) ver_visitantes(parametro1, parametro2);
    else if(strcmp(comando, "ver_mas_visitados") == 0); //
    else{
        fprintf(stderr, "Error en comando %s", comando);
        return;
    }
    printf("OK");
}

int main(int argc, char *argv[]){
    menu(argv);
    return 0;
}
