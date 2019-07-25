#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"
#include "abb.h"
#include "cola.h"
#include "hash.h"
#include "heap.h"
#include "strutil.h"
#define TIME_FORMAT "%FT%T%z"
#define MAXIMO_INDICE_IPS_SPLITEADO 4

typedef struct{
    size_t* cantidad;
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

void destruir_cola_ips(cola_ips_t* dato){
    free(dato->cantidad);
    cola_destruir(dato->tiempos, NULL);
    free(dato);
}

int comparar_numeros(tupla_sitios_t* x,tupla_sitios_t* y){
    return (x->cant_llamados >= y->cant_llamados) ?  -1 : 1;
}

//Funcion auxiliar que checkea si el ip pasado como parametro esta en el rango inicial y final.
bool es_valida(char* ip_actual, char* rango_inicial, char* rango_final){
    char** rango_inicial_spliteado = split(rango_inicial, '.');
    char** rango_final_spliteado = split(rango_final, '.');
    char** ip_actual_spliteado = split(ip_actual, '.');
    int suma_actual = 0;
    int suma_tope = 0;
    for(size_t i = 0; i < MAXIMO_INDICE_IPS_SPLITEADO; i++){
        int base = atoi(rango_inicial_spliteado[i]); 
        int actual = atoi(ip_actual_spliteado[i]) + suma_actual;
        int tope = atoi(rango_final_spliteado[i]) + suma_tope;
        if(actual < base || actual > tope) return false;
        if(i == 3 && base == actual && actual == tope) return true;
        suma_actual = (actual - base)*255;
        suma_tope = (tope - base)*255;
    }
    free_strv(rango_inicial_spliteado);
    free_strv(rango_final_spliteado);
    free_strv(ip_actual_spliteado);
    return true;
}

int comparar_ips(char* ip1, char* ip2){
    char** ip1_split = split(ip1, '.');
    char** ip2_split = split(ip2, '.');
    int respuesta = 0;
    for(size_t i = 0; ip1_split[i] && ip2_split[i]; i++){
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

/*void bubbleSort(char** arr, size_t n){
    size_t i = 0, j = 0;
    for(i = 0; i < n - 1; i++){
        for(j = 0; j < n - i - 1; j++){
            if(comparar_ips(arr[j], arr[j+1]) > 0){
                char* aux = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = aux;
            };
        }
    }
}*/

bool agregar_archivo(char* nombre_archivo, hash_t* sitios, abb_t* visitantes){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return false;
    hash_t* ips = hash_crear((hash_destruir_dato_t)destruir_cola_ips);
    hash_t* ips_dos = hash_crear(NULL);
    if(!ips_dos) return false;
    char* buffer = NULL;
    size_t tamanio = 0;
    ssize_t linea;

    char** ips_dos_ordenadas = malloc(sizeof(char*) * 2500);

    size_t pos = 0;
    while(!feof(archivo)){
        linea = getline(&buffer, &tamanio, archivo);
        if(linea == EOF) break;
        buffer[linea-1] = '\0';
        char** buffer_spliteado = split(buffer, '\t');
        char* ip_actual = buffer_spliteado[0];
        char* sitio_actual = buffer_spliteado[3];
        time_t tiempo_actual = iso8601_to_time(buffer_spliteado[1]);

        if(!hash_pertenece(ips, ip_actual)){ // En caso de no pertenecer al hash de ips, lo guardo como clave y como valor una cola con la cantidad de elementos que va teniendo.
            cola_ips_t* cola_ip = malloc(sizeof(cola_ips_t));
            cola_ip->tiempos = cola_crear();
            cola_encolar(cola_ip->tiempos, &tiempo_actual);
            size_t* cantidad = malloc(sizeof(size_t));
            *cantidad = 1;
            cola_ip->cantidad = cantidad; 
            hash_guardar(ips, ip_actual, cola_ip);
        }else{ //Si la ip pertenece, lo encolamos y aumentamos la cantidad, nos fijamos la diferencia de tiempo entre el ultimo y el primero. En caso de ser menor a 2 segundos, lo insertamos en un hash de DOS.
            cola_ips_t* cola_ip = (cola_ips_t*)hash_obtener(ips, ip_actual); 
            cola_encolar(cola_ip->tiempos, &tiempo_actual);
            size_t* cantidad = cola_ip->cantidad;
            (*cantidad)++;
            if(*cantidad >= 5){
                if(difftime(tiempo_actual, (time_t)cola_ver_primero(cola_ip->tiempos)) <= 2){
                    if(!hash_pertenece(ips_dos, ip_actual)){
                        hash_guardar(ips_dos, ip_actual, NULL);

                        char* ip = malloc(sizeof(char) * strlen(buffer_spliteado[0])+1);
                        strcpy(ip,buffer_spliteado[0]);
                        //En caso de ser DoS, lo inserto en un arreglo para ordenarlo al final e imprimirlo.
                        ips_dos_ordenadas[pos] = ip;
                        pos++;
                    }
                }
                cola_desencolar(cola_ip->tiempos);
                (*cantidad)--;
            }
            //hash_guardar(ips, ip_actual, cola_ip);
        }

        //Mantenemos el hash de sitios actualizado.En caso de no existir el sitio, lo agregamos como clave y valor la cantidad. Si existe, cantidad++.
        if(!hash_pertenece(sitios, sitio_actual)){
            size_t* cantidad = malloc(sizeof(size_t));
            *cantidad = 1;
            hash_guardar(sitios, sitio_actual, cantidad);
        }else{
            size_t* cantidad = hash_obtener(sitios, sitio_actual);
            (*cantidad)++;
        }

        //Mantenemos el abb actualizado. Como usamos un abb, va a estar ordenado.
        if(!abb_pertenece(visitantes, ip_actual)){            
            abb_guardar(visitantes, ip_actual, NULL);
        }
        free_strv(buffer_spliteado);
    }

    ips_dos_ordenadas[pos] = NULL;

    //bubbleSort(ips_dos_ordenadas, maximo_logico);
    //Imprimo las ips que son posibles DoS.     
    for(size_t i = 0; ips_dos_ordenadas[i]; i++){
        fprintf(stdout, "DoS: %s\n", ips_dos_ordenadas[i]);
        free(ips_dos_ordenadas[i]);
    }
    free(ips_dos_ordenadas);

    hash_destruir(ips);
    hash_destruir(ips_dos);
    free(buffer);
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
    // //Completo el heap con los primeros n_mas_solicitados.
    for(size_t i = 0; i < n_mas_solicitados; i++){
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
        if(comparar_numeros(heap_ver_max(heap_aux), tupla_sitio) == 1){ //En caso de que el min_heap sea menor a el actual, desencolo el min_heap y encolo el actual.
            free(heap_desencolar(heap_aux));
            heap_encolar(heap_aux, tupla_sitio);
        }else{
            free(tupla_sitio);
        };
        hash_iter_avanzar(hash_iter);
    }
    hash_iter_destruir(hash_iter);
    printf("Sitios mas visitados:\n");

    pila_t* pila_aux = pila_crear();
    while(!heap_esta_vacio(heap_aux)){
        tupla_sitios_t* tupla = heap_desencolar(heap_aux);
        pila_apilar(pila_aux, tupla);
    }
    while(!pila_esta_vacia(pila_aux)){
        tupla_sitios_t* tupla = pila_desapilar(pila_aux);
        printf("\t%s - %zd\n", tupla->sitio, tupla->cant_llamados);
        free(tupla);
    }    
    //Al tener el heap completo, voy desencolando e imprimiendo sus valores.
    pila_destruir(pila_aux);
    heap_destruir(heap_aux, NULL);
    return true;
}


int main(int argc, char* argv[]){
    // Empiezo el programa creando las estructuras que voy a utilizar para manejar la informacion
    hash_t* sitios = hash_crear(free);
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
            if(!agregar_archivo(comandos[1], sitios, abb_visitados)) error = true;            
        }
        else if(strcmp(comandos[0], "ver_mas_visitados") == 0){
            if(!ver_mas_visitados(atoi(comandos[1]),sitios)) error = true;
        }
        else if(strcmp(comandos[0], "ver_visitantes") == 0){
            if(!ver_visitantes(abb_visitados, comandos[1],comandos[2])) error = true;
        }
        else{
            fprintf(stderr, "Error en comando %s", comandos[0]);
            free_strv(comandos);
            break;
        }
        // Si hubo algun error en los comandoss terminamos el programa
        fprintf(stdout, "OK\n");
        free_strv(comandos);
    }
    abb_destruir(abb_visitados);
    hash_destruir(sitios);
    free(buffer);
}
