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
    char* sitio;
}tupla_sitios_t;


time_t iso8601_to_time(char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

int comparar_numeros(tupla_sitios_t* x,tupla_sitios_t* y){
    return (x->cant_llamados >= y->cant_llamados) ?  1 : -1;
}

int comparar_ips(char* ip1, char* ip2){
    char** ip1_split = split(ip1, '.');
    char** ip2_split = split(ip2, '.');
    int respuesta = 0;
    for(size_t i = 0; i < MAXIMO_INDICE_IPS_SPLITEADO; i++){
        int ip1_atoi = atoi(ip1_split[i]);
        int ip2_atoi = atoi(ip2_split[i]);
        if(ip1_atoi < ip2_atoi){
            respuesta = -1;
            break;
        }
        if(ip1_atoi > ip2_atoi){
            respuesta = 1;
            break;            
        }
    }
    free_strv(ip1_split);
    free_strv(ip2_split);    
    if(respuesta == 0) return 0;
    return respuesta;
}

//Funcion auxiliar que checkea si el ip pasado como parametro esta en el rango inicial y final.
// bool es_valida(char* ip_actual, char* rango_inicial, char* rango_final){
//     char** rango_inicial_spliteado = split(rango_inicial, '.');
//     char** rango_final_spliteado = split(rango_final, '.');
//     char** ip_actual_spliteado = split(ip_actual, '.');
//     char* ip_actual_aux = malloc(sizeof(char) * 12);
//     char* rango_inicial_aux = malloc(sizeof(char) * 12);
//     char* rango_final_aux = malloc(sizeof(char) * 12);
//     for(size_t i = 0; i < MAXIMO_INDICE_IPS_SPLITEADO; i++){
//         strcat(ip_actual_aux, ip_actual_spliteado[i]);
//         strcat(rango_inicial_aux, rango_inicial_spliteado[i]);
//         strcat(rango_final_aux, rango_final_spliteado[i]);
//     }
//     printf("Actual: %s\n", ip_actual_aux);
//     printf("Inicial: %s\n", rango_inicial_aux);
//     printf("Final: %s\n", rango_final_aux);
//     free_strv(rango_inicial_spliteado);
//     free_strv(rango_final_spliteado);
//     free_strv(ip_actual_spliteado);
//     return true;
// }

bool agregar_archivo(char* nombre_archivo, hash_t* ips, hash_t* sitios, abb_t* visitantes){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return false;
    hash_t* ips_dos = hash_crear(NULL);
    if(!ips_dos) return false;

    char* buffer = NULL;
    char** buffer_spliteado;
    size_t tamanio = 0;
    ssize_t linea;
    char* ips_dos_ordenadas[1000];
    size_t pos = 0;
    while(!feof(archivo)){
        linea = getline(&buffer, &tamanio, archivo);
        if(linea == EOF) break;
        buffer[linea - 1] = '\0';
        buffer_spliteado = split(buffer, '\t');

        char* ip_actual = buffer_spliteado[0];
        char* sitio_actual = buffer_spliteado[3];
        time_t tiempo_actual = iso8601_to_time(buffer_spliteado[1]);

        //free_strv(buffer_spliteado);
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
                    if(!hash_pertenece(ips_dos, ip_actual)){
                        hash_guardar(ips_dos, ip_actual, NULL);
                        //En caso de ser DoS, lo inserto en un arreglo para ordenarlo al final e imprimirlo.
                        ips_dos_ordenadas[pos] = ip_actual;
                        pos++;
                    }
                }
                cola_desencolar(cola_ip->tiempos);
            }
            hash_guardar(ips, ip_actual, cola_ip);
        }

        //Mantenemos el hash de sitios actualizado.En caso de no existir el sitio, lo agregamos como clave y valor la cantidad. Si existe, cantidad++.
        if(!hash_pertenece(sitios, sitio_actual)){
            size_t* cantidad = malloc(sizeof(size_t));
            *cantidad = 1;
            hash_guardar(sitios, sitio_actual, cantidad);
        }else{
            size_t* cantidad = hash_obtener(sitios, sitio_actual);
            (*cantidad)++;
            hash_guardar(sitios, sitio_actual, cantidad);
        }
        //Mantenemos el abb actualizado. Como usamos un abb, va a estar ordenado.
        if(!abb_pertenece(visitantes, ip_actual)){
            char* ip = malloc(sizeof(char));
            ip = ip_actual;
            abb_guardar(visitantes, ip, NULL);
        }
    }

    //Imprimo las ips que son posibles DoS.
    for(size_t i = 0; ips_dos_ordenadas[i]; i++) fprintf(stdout, "DoS: %s\n", ips_dos_ordenadas[i]);
    hash_destruir(ips_dos);



    fclose(archivo);
    return true;
}

//En esta funcion nos encargamos de recorrer el abb de visitados con un iterador interno  de tal manera que los visitantes que van a ser imprimidos son aquellos que esten en el rango pasado por parametro.
bool ver_visitantes(abb_t* visitantes, char* rango_inicial, char* rango_final){
    abb_iter_t* abb_iter = abb_iter_in_crear(visitantes);
    if(!abb_iter) return false;
    printf("Visitantes:\n");
    while(!abb_iter_in_al_final(abb_iter)){
        char* ip_actual = (char*)abb_iter_in_ver_actual(abb_iter);
        //if(es_valida(ip_actual, rango_inicial, rango_final)) printf("\t%s\n", abb_iter_in_ver_actual(abb_iter));
        abb_iter_in_avanzar(abb_iter);
    }
    abb_iter_in_destruir(abb_iter);
    return true;
}

bool ver_mas_visitados(size_t n_mas_solicitados, hash_t* sitios){
    // Creo el heap y el iter que vamos a usar
    heap_t* heap_aux = heap_crear((cmp_func_t )comparar_numeros);
    if(!heap_aux) return false;
    hash_iter_t* hash_iter = hash_iter_crear(sitios);
    if(!hash_iter){
        heap_destruir(heap_aux,NULL);
        return false;
    }
    //Completo el heap con los primeros n_mas_solicitados.
    for(size_t i = 0; i < n_mas_solicitados && !hash_iter_al_final(hash_iter); i++){
        char* sitio_actual = (char*)hash_iter_ver_actual(hash_iter);
        size_t cant_llamados_actual = *((size_t*)hash_obtener(sitios, sitio_actual));
        tupla_sitios_t* tupla_sitio =  malloc(sizeof(tupla_sitios_t));
        tupla_sitio->cant_llamados = cant_llamados_actual;
        tupla_sitio->sitio = sitio_actual;
        heap_encolar(heap_aux, tupla_sitio);
        hash_iter_avanzar(hash_iter);
    }
    //Una vez que tengo el heap con k elementos, voy avanzando el hash_iter y comparando el actual con el min del heap. En caso de ser mayor el actual, desencolo el min del heap y encolo el actual del hash_iter.
    while(!hash_iter_al_final(hash_iter)){
        char* sitio_actual = (char*)hash_iter_ver_actual(hash_iter);
        size_t cant_llamados_actual = *((size_t*)hash_obtener(sitios, sitio_actual));
        tupla_sitios_t* tupla_sitio = malloc(sizeof(tupla_sitios_t));
        tupla_sitio->sitio = sitio_actual;
        tupla_sitio->cant_llamados = cant_llamados_actual;
        if(comparar_numeros(heap_ver_max(heap_aux), tupla_sitio) == -1){ //En caso de que el min_heap sea menor a el actual, desencolo el min_heap y encolo el actual.
            free(heap_desencolar(heap_aux));
            heap_encolar(heap_aux, tupla_sitio);
        }else{
            free(tupla_sitio);
        };
        hash_iter_avanzar(hash_iter);
    }
    hash_iter_destruir(hash_iter);
    //Al tener el heap completo, voy desencolando e imprimiendo sus valores.
    printf("Sitios mas visitados:\n");
    while(!heap_esta_vacio(heap_aux)){
        tupla_sitios_t* tupla = heap_desencolar(heap_aux);
        printf("\t%s - %zd\n", tupla->sitio, tupla->cant_llamados);
        free(tupla);
    }
    heap_destruir(heap_aux, NULL);
    // hash_iter_destruir(hash_iter);
    // Ordernar el heap, luego recorrerlo e imprimirlo TODO
    return true;
}


int main(int argc, char* argv[]){
    // Empiezo el programa creando las estructuras que voy a utilizar para manejar la informacion
    hash_t* ips = hash_crear(NULL);/*necesitamos una func de destruccion para el struc de cola_ips   TODO */;
    if(!ips) return 0;
    hash_t* sitios = hash_crear(NULL);
    if(!sitios){
        hash_destruir(ips);
        return 0;
    }
    abb_t* abb_visitados = abb_crear((abb_comparar_clave_t)comparar_ips,NULL);
    char* buffer = NULL;
    size_t tamanio = 0;
    ssize_t linea;
    bool error = false;
    while(!feof(stdin)){
        linea = getline(&buffer, &tamanio, stdin);
        if(linea == EOF) break;
        if(buffer[linea - 1] == '\n') buffer[linea - 1] = '\0';
        char** comandos = split(buffer, ' ');
        if(strcmp(comandos[0], "agregar_archivo") == 0){
            if(!agregar_archivo(comandos[1], ips, sitios, abb_visitados)) error = true;            
        }
        if(strcmp(comandos[0], "ver_mas_visitados") == 0){
            if(!ver_mas_visitados(atoi(comandos[1]),sitios)) error = true;
        }
        else if(strcmp(comandos[0], "ver_visitantes") == 0){
            if(!ver_visitantes(abb_visitados, comandos[1],comandos[2])) error = true;
        }

        // Si hubo algun error en los comandoss terminamos el programa
        if(error){
            fprintf(stderr, "Error en comandos %s", comandos[0]);
            hash_destruir(ips);
            hash_destruir(sitios);
            return 0;
        }
        fprintf(stdout, "OK\n");
        free_strv(comandos);
    }
    free(buffer);
}
