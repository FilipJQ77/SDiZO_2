#ifndef SDIZO2_LISTGRAPH_H
#define SDIZO2_LISTGRAPH_H

#include <iostream>
#include "Edge.h"
#include "KruskalHelper.h"
#include "PrimVertexHeap.h"
#include "DijkstraVertexHeap.h"
#include "AdjacencyList.h"

using std::cout;

/**
 * klasa reprezentująca graf reprezentowany listami sąsiedztwa
 */
class ListGraph {

    int vertices;
    int edges;
    AdjacencyList **adjacencyLists;

public:

    explicit ListGraph(int vertices = 0);

    ~ListGraph();

    void addVertex();

    void addUndirectedEdge(int, int, int);

    void addDirectedEdge(int, int, int);

    /**
     * algorytm Prima wg. Cormena, z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param key tablica
     * @param parent tablica
     * @param startingVertex wierzchołek startowy
     */
    void prim(int *&, int *&, int);

    /**
     * algorytm Kruskala z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param mstEdges
     */
    void kruskal(Edge **);

    /**
     * algorytm Dijkstry z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param distance
     * @param parent
     * @param startingVertex
     */
    void dijkstra(int *&, int *&, int);

    /**
     * algorytm Bellmana-Forda, z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param distance
     * @param parent
     * @param startingVertex
     */
    bool bellmanFord(int *&, int *&, int);

    void print();

    int getVertices();

    int getEdges();
};


#endif //SDIZO2_LISTGRAPH_H
