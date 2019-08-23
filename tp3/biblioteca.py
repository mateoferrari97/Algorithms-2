import csv
from grafo import *
import sys
import random
import collections
from collections import deque as Deque
import operator
 
def cargar_grafo(nombre_archivo):
    grafo = Grafo()
    with open(nombre_archivo) as delincuentes:
        delincuentes_tsv = csv.reader(delincuentes, delimiter="\t")
        for linea in delincuentes_tsv:
            grafo.agregar_vertice(int(linea[0]))
            grafo.agregar_vertice(int(linea[1]))
            grafo.agregar_arista(int(linea[0]), int(linea[1]))                
    return grafo

def max_freq(lista):
    resultado = collections.Counter(lista)
    etiqueta_que_mas_se_repite = -1
    cant_veces = -1
    for clave,valor in resultado.items():
        if valor > cant_veces:
            cant_veces = valor
            etiqueta_que_mas_se_repite = clave
    if etiqueta_que_mas_se_repite == -1: return None
    return etiqueta_que_mas_se_repite

def dfs_cfc(grafo, vertice, visitados, orden, lista1, pila2, cfcs, en_cfs):
    visitados.add(vertice)
    lista1.append(vertice)
    pila2.append(vertice)
    for adyacente in grafo.adyacentes(vertice):
        if adyacente not in visitados:
            orden[adyacente] = orden[vertice] + 1
            dfs_cfc(grafo, adyacente, visitados, orden, lista1, pila2, cfcs, en_cfs)
        elif adyacente not in en_cfs:
            while orden[lista1[-1]] > orden[adyacente]:
                lista1.pop()

    if lista1[-1] == vertice:
        lista1.pop()
        aux = None
        nueva_cfc = []
        while aux != vertice:
            aux = pila2.pop()
            en_cfs.add(aux)
            nueva_cfc.append(aux)
        cfcs.append(nueva_cfc)

def random_walks(grafo, origen, largo):
    recorrido = {}
    v = origen
    for i in range(largo):
        if not v in recorrido:
            recorrido[v] = 1
        else: 
            recorrido[v] += 1
        adyacentes = grafo.adyacentes(v)
        if len(adyacentes) > 0:
            v = random.choice(adyacentes)
        else:
            break
    return recorrido
    
def bfs_min_seguimiento(grafo, vertice_inicial, destino):
    visitados = set()
    distancia = {}
    padres = {}
    q = Deque()
    q.append(vertice_inicial)
    padres[vertice_inicial] = None
    visitados.add(vertice_inicial)
    distancia[vertice_inicial] = 0

    while q:
        v = q.popleft()
        if v == destino: break
        for w in grafo.adyacentes(v):
            if w not in visitados:
                visitados.add(w)
                distancia[w] = distancia[v] + 1
                padres[w] = v
                q.append(w)
    return distancia, padres

def bfs_divulgar(grafo, vertice_inicial, n):
    visitados = set()
    distancia = {}
    padres = {}
    q = Deque()
    q.append(vertice_inicial)
    padres[vertice_inicial] = None
    visitados.add(vertice_inicial)
    distancia[vertice_inicial] = 0
    while q:
        v = q.popleft()
        if distancia[v] == n: break
        for w in grafo.adyacentes(v):
            if w not in visitados:
                visitados.add(w)
                distancia[w] = distancia[v] + 1
                padres[w] = v
                q.append(w)
    return distancia, padres

def ciclo_n(grafo, origen, largo):
    visitados = set()
    respuesta = Deque()
    respuesta.append(origen)
    visitados.add(origen)
    if _ciclo_n(grafo, origen, largo, visitados, respuesta, origen, 1):return respuesta
    respuesta.pop()
    return respuesta

def _ciclo_n(grafo, v, largo, visitados, respuesta, principio, cantidad): 
    #if len(respuesta)-1 > largo: return False
    if cantidad-1 > largo: return False
    adyacentes = grafo.adyacentes(v)
    if cantidad == largo:
        if principio in adyacentes:
            respuesta.append(principio)
            return respuesta
        return False
    for w in adyacentes:
        if w in visitados:continue
        respuesta.append(w)
        visitados.add(w)
        cantidad += 1
        #print(respuesta)
        if _ciclo_n(grafo, w, largo, visitados, respuesta, principio, cantidad): return respuesta
        respuesta.pop()
        cantidad -= 1
        #visitados.remove(w)
        #else:
        #    if v == principio and len(respuesta)-1 == largo: return respuesta
    return False

        