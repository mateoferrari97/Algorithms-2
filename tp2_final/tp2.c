#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "abb.h"
#include "cola.h"
#include "hash.h"
#include "heap.h"
#include "strutil.h"
#define TIME_FORMAT "%FT%T%z"
#define CANTIDAD_POSIBLE_DOS 5
#define MAXIMO_INDICE_IPS_SPLITEADO 4


/* 
Para buscar los "errores" o temas a tener en cuenta hacer un CTRL+F y buscar 'TODO'.
Basicamente faltaria hacer el ver_visitantes, el tema de ordenar el heap en ver_mas_visitados, ver que onda 
la funcion de destruccion del struct cola_ips y alguna que otra boludez mas.  
*/

typedef struct{
    size_t cantidad;
    cola_t* tiempos;
}cola_ips_t;


typedef struct{
    size_t cant_llamados;
    char* sitio
}tupla_sitios_t;


time_t iso8601_to_time(char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

int comparar_numeros(tupla_sitios_t* x,tupla_sitios_t* y){
    return (x->cant_llamados > y->cant_llamados) ?  1 : -1;
}



/* esto supongo que vuela pero por si acaso lo dejo         TODO

bool es_valida(char* ip, char* rango_inicial, char* rango_final){
    char** rango_inicial_spliteado = split(rango_inicial, '.');
    char** rango_final_spliteado = split(rango_final, '.');
    char** ip_spliteada = split(ip, '.');
    printf("\n");
    printf("IP: %s\n", ip);
    printf("Rango Inicial: %s\n", rango_inicial);
    printf("Rango Final: %s\n", rango_final);    
    for(size_t i = 0; i < MAXIMO_INDICE_IPS_SPLITEADO; i++){
        int ip_valor = atoi(ip_spliteada[i]);
        int rango_inicial_valor = atoi(rango_inicial_spliteado[i]);
        int rango_final_valor = atoi(rango_final_spliteado[i]);
        if(ip_valor < rango_inicial_valor || ip_valor > rango_final_valor){
            printf("Rechazado\n");
            return false;
        }    
    }
    printf("Aceptado\n");
    printf("\n");
    return true;
}
*/

void agregar_archivo(char* nombre_archivo,hash_t* ips, hash_t* sitios){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return NULL;
    hash_t* ips_dos = hash_crear(NULL);
    if(!ips_dos) return NULL;

    char* buffer = NULL;
    char** buffer_spliteado;
    size_t tamanio = 0;

    while(!feof(archivo)){
        ssize_t linea = getline(&buffer, &tamanio, archivo);
        if(linea == EOF) break;
        buffer[linea - 1] = '\0';
        buffer_spliteado = split(buffer, '\t');

        char* ip_actual = buffer_spliteado[0];
        char* sitio_actual = buffer_spliteado[3];
        time_t tiempo_actual = iso8601_to_time(buffer_spliteado[1]);

        //Guardamos la IP en ips(hash) y verificamos si hay o no un DOS
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

        //Guardamos el sitio en sitios(sitios) y aumentamos la cantidad de apareciones en 1 si ya se encontraba
        cantidad = hash_obtener(sitios, sitio_actual);
        if(!cantidad){
            hash_guardar(sitios, sitio_actual, 1);
        }
        else{
            cantidad++;
            hash_guardar(sitios, sitio_actual, cantidad)
        }
    }


    //Iteramos el hash DoS y vamos imprimiendo las ips
    hash_iter_t* hash_iter = hash_iter_crear(ips_dos);
    while(!hash_iter_al_final(hash_iter)){
        fprintf(stdout, "DoS:  %s\n", hash_iter_ver_actual(hash_iter));
        hash_iter_avanzar(hash_iter);
    }
    hash_iter_destruir(hash_iter)
    hash_destruir(ips_dos);
    fclose(archivo);
}


/* Te la dejo a vos para que la hagas jaja, yo no le entendi a Martin lo que puso.      TODO

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

*/


void ver_mas_visitados(int n_mas_solicitados, hash_t* sitios){
    // Creo el heap y el iter que vamos a usar
    heap_t* heap_aux = heap_crear((cmp_func_t)comparar_numeros);
    if(!heap_aux)return NULL;
    hash_iter_t* hash_iter = hash_iter_crear(sitios);
    if(!hash_iter){
        heap_destruir(heap_aux,NULL);
        return NULL;
    }
    // Arranco a iterar el hash
    while(!hash_iter_al_final(hash_iter)){
        char* sitio_actual = hash_iter_ver_actual(hash_iter));
        int cant_llamados_actual = hash_obtener(sitios, sitio);

        // Si el heap tiene n elementos entonces comparamos el tope con el actual
        if(heap_cantidad(heap_aux) == n_mas_solicitados){
            tupla_sitios_t* tope = heap_ver_max(heap_aux);
            if(cant_llamados_actual > tope->cant_llamados){
                free(heap_desencolar(heap_aux));
                tupla_sitios_t* actual = malloc(sizeof(tupla_sitios_t));
                if(!actual){
                    heap_destruir(heap_aux,free);
                    hash_iter_destruir(hash_iter);
                    return NULL;
                }
                actual->cant_llamados = cant_llamados_actual;
                actual->sitio = sitio_actual;
                heap_encolar(heap_aux, actual);
            }
        }
        // Si no llega a n elementos entonces encolamos el actual de una
        else{
            tupla_sitios_t* actual = malloc(sizeof(tupla_sitios_t));
            if(!actual){
                heap_destruir(heap_aux,free);
                hash_iter_destruir(hash_iter);
                return NULL;
            }
            actual->cant_llamados = cant_llamados_actual;
            actual->sitio = sitio_actual;
            heap_encolar(heap_aux, actual);
        }
        hash_iter_avanzar(hash_iter);
    }

    hash_iter_destruir(hash_iter)
    // Ordernar el heap, luego recorrerlo e imprimirlo          TODO    
    }
}


int main(){
    // Empiezo el programa creando las estructuras que voy a utilizar para manejar la informacion
    hash_t* ips = hash_crear(/*necesitamos una func de destruccion para el struc de cola_ips   TODO */);
    if(!ips) return 0
    hash_t* sitios = hash_crear(NULL);
    if(!sitios){
        hash_destruir(ips);
        return 0
    }
    // abb_t* abb_visitados = abb_crear(comparar_ips,NULL) se necesita al final o no?     TODO
    
    bool error = false;
    char linea[1000]; // Esto seguro es una villereada o directamente no anda.       TODO
    while(fgets(linea, 1000, stdin)){

        // Spliteamos la linea, comando[0] va a ser el comando en si, el 1 y el 2 son los parametros
        char** comando = split(linea, " "); 
        if(strcmp(comando[0], "agregar_archivo")==0){
            if(!agregar_archivo(comando[1], ips, sitios, abb?))error = true;            
        }
        else if(strcmp(comando[0], "ver_visitantes")==0){
            if(!ver_visitantes(comando[1],comando[2], ips))error = true;
        }
        else if(strcmp(comando[0], "ver_mas_visitados")==0){
            if(!ver_mas_visitados(atoi(comando[1]),sitios))error = true;
        }

        else{ // Si pasaron un comando invalido terminamos el programa
            fprintf(stderr, "Error en comando %s", comando[0]);
            hash_destruir(ips);
            hash_destruir(sitios, NULL);
            return 0;
        }
        // Si hubo algun error en los comandos terminamos el programa
        if(error){
            fprintf(stderr, "Error en comando %s", comando[0]);
            hash_destruir(ips);
            hash_destruir(sitios);
            return 0;
        }
        fprintf(stdout, "OK");
    }
}
