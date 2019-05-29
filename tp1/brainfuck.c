#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define CANTIDAD_INICIAL_VECTOR 2000

unsigned char* crear_arr_caracteres(FILE* archivo){
    unsigned char* arr = malloc(sizeof(unsigned char) * CANTIDAD_INICIAL_VECTOR);
    int c;
    size_t i = 0;
    if(!arr) return NULL;
    while((c = fgetc(archivo)) != EOF){
        arr[i] = (unsigned char)c;
        i++;
    }  
    arr[i] = '\0';
    return arr;  
}
unsigned char* crear_arr_variables(){
    unsigned char* arr = malloc(sizeof(unsigned char) * CANTIDAD_INICIAL_VECTOR);
    size_t i;
    if(!arr) return NULL;
    for(i = 0; i < CANTIDAD_INICIAL_VECTOR; i++) arr[i] = 0;
    return arr;
}

int main(int argc, char const *argv[]){
    if(argc != 2) return 0;
    FILE* archivo = fopen(argv[1], "r");
    if(!archivo) return 0;
    size_t i = 0;
    unsigned char* ptr_variable;
    unsigned char* caracteres = crear_arr_caracteres(archivo);
    unsigned char* variables = crear_arr_variables();
    pila_t* ciclos = pila_crear();
    if(!ciclos) return 0;
    ptr_variable = variables;
    while(caracteres[i] != '\0'){
        switch (caracteres[i]){
            case '+':
                i++;
                ++*ptr_variable;
                break;
            case '-':
                i++;
                --*ptr_variable;
                break;
            case '.':
                i++;
                printf("%c", *ptr_variable);
                break;                        
            case ',':
                i++;
                scanf("%c", ptr_variable);
                break;        
            case '>':
                i++;
                ++ptr_variable;
                break;            
            case '<':
                if(ptr_variable != variables) --ptr_variable;                
                i++;
                break;            
            case '[':
                if(*ptr_variable == 0){
                    size_t contador = 1;
                    while(contador != 0){
                        i++;
                        if(caracteres[i] == '[') contador++;
                        if(caracteres[i] == ']') contador--;
                    }
                }else{
                    size_t* pos = malloc(sizeof(size_t));
                    if(!pos) return 0;
                    *pos = i;
                    pila_apilar(ciclos, pos);
                }         
                i++;
                break;
            case ']':
                if(*ptr_variable == 0){
                    free(pila_desapilar(ciclos));
                }else{
                    i = *(size_t*)pila_ver_tope(ciclos);  
                } 
                i++;
                break;
            default:
                i++;
                break;         
        }
    }
    free(caracteres);
    free(variables);
    pila_destruir(ciclos);
    fclose(archivo);
    return 0;
}



