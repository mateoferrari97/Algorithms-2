#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap (int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

int maximo(int vector[], int n) {

    if(n == 0){
        return -1;
    }

    int maximo = vector[0];
    int pos_maximo = 0;

    for(int i = 0; i < n; i++){
        if(vector[i] > maximo){
            maximo = vector[i];
            pos_maximo = i;
        }
    }

    return pos_maximo;
}

int comparar(int vector1[], int n1, int vector2[], int n2) {
    if(n1 == 0 && n2 == 0){
        return 0;
    }

    for(int i = 0; i < n1 && i < n2; i++){
        if(vector1[i] > vector2[i]){
            return 1;
        } 
        
        if(vector1[i] < vector2[i]){
            return -1;
        }
    }

    if(n1 > n2){
        return 1;
    }

    if(n1 < n2){
        return -1;
    }    

    return 0;
}

void seleccion(int vector[], int n) {
    int max;

    for(int i = n-1; i > 0; i--){
        max = i;
        if(vector[maximo(vector, i)] > vector[max]){
            swap(&vector[maximo(vector, i)], &vector[max]);
        }                            
    } 
}
