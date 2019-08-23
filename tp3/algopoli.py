#!/usr/bin/python3
import csv
from grafo import *
import sys
import random
import collections
from biblioteca import *
import operator
CANT_ITERACIONES_COMUNIDADES = 10

def main():
    archivo = cargar_grafo(sys.argv[1])
    f = open(sys.argv[2]) if len(sys.argv) > 2 else sys.stdin    
    for line in f:
        comando = line.split()
        if comando[0] == "min_seguimientos":
            print(min_seguimientos(archivo, int(comando[1]),int(comando[2])))
        if comando[0] == "mas_imp":
            print(mas_imp(archivo, int(comando[1])))
        if comando[0] == "persecucion":
            delincuentes = [int(i) for i in comando[1].split(",")]
            print(persecucion(archivo, delincuentes, int(comando[2])))
        if comando[0] == "divulgar":
            print(divulgar(archivo, int(comando[1]), int(comando[2])))
        if comando[0] == "comunidades":
            comunidades(archivo, int(comando[1]))          
        if comando[0] == "divulgar_ciclo":
            print(divulgar_ciclo(archivo, int(comando[1]),int(comando[2])))
        if comando[0] == "cfc":
            cfc(archivo)


"""*********************************************************************************"""
"""*********************************************************************************"""
"""*********************************COMANDOS****************************************"""
"""*********************************************************************************"""
"""*********************************************************************************"""

#FUNCIONA OK
def min_seguimientos(grafo, origen, destino):
    resultado = []
    cadena = ""
    orden, padres = bfs_min_seguimiento(grafo,origen, destino)
    if destino not in padres:
        return "Seguimiento imposible"
    resultado.insert(0, destino)
    padre = padres[destino]
    while padre != None:
        resultado.insert(0, padre)
        padre = padres[padre]
    largo_resultado = len(resultado)
    for indice in range(largo_resultado):
        cadena += str(resultado[indice])
        if indice + 1 != largo_resultado: cadena += " -> "
    return cadena

def mas_imp(grafo, cantidad):
    apariciones = {}
    resultado = ""
    iteracion = 0
    randoms = random.choices(list(grafo.vertices), k = 100)
    for actual in randoms:
        recorridos = random_walks(grafo, actual, 250)
        for v, aparicion in recorridos.items(): 
            if not v in apariciones:
                apariciones[v] = aparicion
            else:
                apariciones[v] += aparicion
    apariciones = sorted(apariciones.items(), key = operator.itemgetter(1), reverse = True)
    for index, delincuente in enumerate(apariciones):
        if index >= cantidad: break
        resultado += str(apariciones[index][0]) + ", "
    resultado = resultado[:-2]
    return resultado
    
#LABURANDO ACA
def divulgar_ciclo(grafo, origen, largo):
    d = ciclo_n(grafo, origen, largo)
    if len(d) == 0: return "No se encontro recorrido"
    res=""
    for x in d:
        res+=str(x)
        res+=" -> "
    return res[:-4]

def persecucion(grafo, delincuentes, k):
    lista_mas_imp = mas_imp(grafo,k).split(", ")
    res = "."
    res_aux = ""
    for delincuente in delincuentes:
        for mas_importante in lista_mas_imp:
            res_aux = min_seguimientos(grafo, delincuente, int(mas_importante))
            if res_aux == "Seguimiento imposible":
                return "Seguimiento imposible"
            largo_res = len(res)
            if largo_res == 1: 
                res = res_aux
                continue
            if len(res_aux) <= largo_res:
                res = res_aux
    return res

def comunidades(grafo, n):
    label = {}
    #inicio el dicc label con clave vertice y valor su indice en el dicc de grafo.vertices
    for index, clave in enumerate(grafo.vertices): label[clave] = index
    claves = list(label.keys())
    vertices_random = random.choices(claves, k=CANT_ITERACIONES_COMUNIDADES)
    largo_claves = len(claves)
    for vertice in vertices_random:
        for x in range(largo_claves): #voy actualizando el valor del vertice en el dicc con la max_freq de los labesl de sus adyacentes
            actual = claves[x]
            lista_aux = []
            for adyacente in grafo.adyacentes(actual): lista_aux.append(label[adyacente])
            if max_freq(lista_aux) == None: continue
            label[actual] = max_freq(lista_aux)
    #recorro el dicc label y voy metiendo las comunidades(que son listas) en una lista de comunidades
    dicc_de_comunidades = {}
    for vertice in label:
        etiqueta = label[vertice]
        if not etiqueta in dicc_de_comunidades:
            comunidad = []
            comunidad.append(vertice)
            dicc_de_comunidades[etiqueta] = comunidad
        else:
            dicc_de_comunidades[etiqueta].append(vertice)
    #por ultimo tengo que devolver las comunidades que tengan al menos n vertices
    contador = 1
    for comunidad in dicc_de_comunidades.values():
        if len(comunidad) >= n:
            print("Comunidad " + str(contador) + ": " + str(comunidad))
            contador += 1

#FUNCIONA OK
def cfc(grafo):
    visitados = set()
    en_cfs = set()
    orden = {}
    lista1 = []
    pila2 = Deque()
    cfcs = []
    resultado = ""
    for vertice in grafo.vertices:
        if vertice not in visitados:
            orden[vertice] = 0
            dfs_cfc(grafo, vertice, visitados, orden, lista1, pila2, cfcs, en_cfs)
    for index, cfc in enumerate(cfcs):
        resultado = "CFC " + str(index + 1) + ": "
        for num in cfc: 
            resultado += str(num) + ", "
        print(resultado[:-2])
        
#FUNCIONA OK
def divulgar(grafo, delincuente, n):
    distancias,padres = bfs_divulgar(grafo, delincuente, n)
    resultado = ""
    for vertice in distancias:
        if vertice == delincuente: continue
        resultado += str(vertice) + ", "
    resultado = resultado[:-2]
    return resultado
    
main()