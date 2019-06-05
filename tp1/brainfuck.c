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
void brainfuck(char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return;
    size_t i = 0;
    unsigned char* ptr_variable;
    unsigned char* caracteres = crear_arr_caracteres(archivo);
    unsigned char* variables = crear_arr_variables();
    pila_t* ciclos = pila_crear();
    if(!ciclos) return;
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
                    if(!pos) return;
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
}

int main(int argc, char *argv[]){
    if(argc == 2){
        char* archivo = argv[1];
        brainfuck(archivo);
    }
    return 0;
}



