#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
void fixcol(const  char* nombre_archivo, size_t n){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo){
        fprintf(stderr, "Error: archivo fuente inaccesible");
        return;
    }
    int c;
    size_t posicion = 0;
    
    while((c = getc(archivo)) && c != EOF){
        if(c == '\n'){
            printf("%c", c);
            posicion = 0;
            continue;
        } 
        if(posicion == n){   
            printf("\n");
            posicion = 0;
            printf("%c", c);
            posicion++;
            continue;
        }
        printf("%c", c);
        posicion++;
    }
    fclose(archivo);
}

int main(int argc, const char** argv){
    if(argc != 3){
        fprintf(stderr, "Error: Cantidad erronea de parametros");
    }else{
        fixcol(argv[1], atoi(argv[2]));
    }
    return 0;
}