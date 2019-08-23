from collections import deque as Deque

class Grafo(object):
    def __init__(self):
        self.vertices = {}
        self.cantidad_vertices = 0
        self.cantidad_artistas = 0

    def adyacentes(self, clave):
        return list(self.vertices.get(clave, None))

    def agregar_vertice(self, vertice):
        if vertice not in self.vertices: 
            self.cantidad_vertices += 1
            self.vertices[vertice] = {}

    def eliminar_vertice(self, vertice):
        if vertice in self.vertices:
            for adyacente in self.vertices[vertice]:
                self.vertices[vertice].remove(adyacente)
                self.cantidad_artistas -= 1
            self.vertices.pop(vertice)
            self.cantidad_vertices -= 1

    def devolver_cantidad_vertices(self):
        return self.cantidad_vertices

    def existe_vertice(self, vertice):
        return vertice in self.vertices
        
    def devolver_valor_vertice(self, vertice):
        if vertice in self.vertices: return self.vertices[vertice]

    def agregar_arista(self, origen, destino):
        if origen in self.vertices:
            valores = self.vertices.get(origen, {})
            valores[destino] = None #no tiene pesos
            self.vertices[origen] = valores
            self.cantidad_artistas += 1

    def eliminar_arista(self, origen, destino):
        if origen in self.vertices:
            self.vertices[origen].remove(destino)
            self.cantidad_artistas -= 1

    def devolver_cantidad_artistas(self):
        return self.cantidad_artistas

    def existe_arista(self, origen, destino):
        if origen in self.vertices: return destino in self.vertices[origen]

