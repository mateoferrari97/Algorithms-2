#include "strutil.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/*
    ******************************************
    ******************************************
            Funciones Auxiliares
    ******************************************
    ******************************************
*/
size_t cantidad_palabras(char** strv){
    size_t contador = 0;
    size_t i = 0;
    while(strv[i] != NULL){
        contador++;
        i++;
    };    
    return contador;   
}

size_t contar_delimitadores(const char* str, char sep){
    size_t apariciones = 0;
    for(size_t i = 0; i < strlen(str); i++) if(str[i] == sep) apariciones++;
    return apariciones;
}

size_t obtener_posicion_delimitador(const char* str, char sep){
    size_t i = 0;
    while(str[i] != '\0' && str[i] != sep)i++;
    return i;
}

void recorrer_arreglo_dinamico(char** strv){
    size_t i = 0;
    while(strv[i] != NULL){
        printf("%s\n", strv[i]);
        i++;
    }
}

/*
    ******************************************
    ******************************************
            Funciones Principales
    ******************************************
    ******************************************
*/

char *substr(const char *str, size_t n){
    if(!str) return NULL;
    char* nueva_cadena = malloc((sizeof(char) * n) + 1);
    if(!nueva_cadena) return NULL;
    strncpy(nueva_cadena, str, n);
    nueva_cadena[n] = '\0';
    return nueva_cadena;
}
char **split(const char *str, char sep){
    if(!str) return NULL;
    size_t i, delimitadores = contar_delimitadores(str, sep) + 1;
    char** arreglo_cadenas = malloc(sizeof(char*) * (delimitadores + 1));
    if(!arreglo_cadenas) return NULL;
    const char* str_aux = str;
    for(i = 0; i < delimitadores; i++){
        size_t caracteres = obtener_posicion_delimitador(str_aux, sep);
        char* nueva_cadena = malloc(sizeof(char) * (caracteres + 1));
        if(!nueva_cadena) return NULL;
        strncpy(nueva_cadena, str_aux, caracteres);
        nueva_cadena[caracteres] = '\0';
        arreglo_cadenas[i] = nueva_cadena;
        str_aux += caracteres + 1;
    }
    arreglo_cadenas[i] = NULL;
    return arreglo_cadenas;
}

char *join(char **strv, char sep){
    size_t largo = 0;
    if(strv) largo = cantidad_palabras(strv);
    if(largo == 0){
        char* cadena_nueva = malloc(sizeof(char));
        cadena_nueva[0] = '\0';
        return cadena_nueva;
    }
    size_t largo_cadena_nueva = 0;
    size_t offset = 0;
    for(size_t i = 0; i < largo; i++) largo_cadena_nueva += strlen(strv[i]);
    char* cadena_nueva = malloc(sizeof(char*) * (largo_cadena_nueva + largo));
    for(size_t i = 0; i < largo; i++){
        strcpy(cadena_nueva + offset, strv[i]);
        offset += strlen(strv[i]);
        if(sep != '\0' && i != largo-1){
            cadena_nueva[offset] = sep;  
            offset++;
        } 
    }
    cadena_nueva[offset] = '\0';
    return cadena_nueva;
}

void free_strv(char *strv[]){
    for(size_t i = 0; strv[i]; i++){
        free(strv[i]);
    }
    free(strv);
}