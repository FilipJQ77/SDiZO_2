#ifndef SDIZO2_MATRIXGRAPH_H
#define SDIZO2_MATRIXGRAPH_H


#include "Edge.h"
#include "Matrix.h"

/**
 * klasa reprezentująca graf reprezentowany macierzą incydencji
 */
class MatrixGraph {

    int vertices;
    int edges;
    Matrix *incidenceMatrix;
    int *edgeWeights;

public:

    explicit MatrixGraph(int vertices = 0);

    ~MatrixGraph();

    void addUndirectedEdge(int vertex1, int vertex2, int edgeWeight);

    void addDirectedEdge(int vertex1, int vertex2, int edgeWeight);

    void newEdgeWeights(int addedEdgeWeight);

    /**
     * algorytm Prima wg. Cormena, z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param key tablica
     * @param parent tablica
     * @param startingVertex wierzchołek startowy
     */
    void prim(int *&key, int *&parent, int startingVertex = 0);

    /**
     * algorytm Kruskala z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     */
    void kruskal(Edge **mstEdges);

    /**
     * algorytm Dijkstry z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param distance
     * @param parent
     * @param startingVertex
     */
    void dijkstra(int *&distance, int *&parent, int startingVertex = 0);

    /**
     * algorytm Bellmana-Forda, z materiałów do ćw. SDiZO dr inż. Jarosława Mierzwy
     * @param distance
     * @param parent
     * @param startingVertex
     */
    bool bellmanFord(int *&distance, int *&parent, int startingVertex = 0);

    void print();

    int getVertices();

    int getEdges();

};


#endif //SDIZO2_MATRIXGRAPH_H
