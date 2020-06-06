#ifndef SDIZO2_DIJKSTRAVERTEX_H
#define SDIZO2_DIJKSTRAVERTEX_H

/**
 * klasa pomocnicza reprezentująca wierzchołek w algorytmie Dijkstry (numer wierzchołka oraz jego 'distance')
 */
class DijkstraVertex {

    int vertexNumber;
    int distance;

public:

    DijkstraVertex(int, int);

    int getVertexNumber();

    int getDistance();

    void setDistance(int);
};

#endif //SDIZO2_DIJKSTRAVERTEX_H
