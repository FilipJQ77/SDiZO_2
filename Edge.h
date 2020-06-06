#ifndef SDIZO2_EDGE_H
#define SDIZO2_EDGE_H


/**
 * klasa pomocnicza reprezentująca krawędź grafu (wierzchołek początkowy, końcowy oraz wagę krawędzi)
 */
class Edge {

    int vertex1;
    int vertex2;
    int edgeWeight;

public:

    Edge(int, int, int);

    int getVertex1();

    int getVertex2();

    int getEdgeWeight();

    void setVertex1(int);

    void setVertex2(int);

    void setEdgeWeight(int);

};


#endif //SDIZO2_EDGE_H
