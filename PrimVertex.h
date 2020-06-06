#ifndef SDIZO2_PRIMVERTEX_H
#define SDIZO2_PRIMVERTEX_H

/**
 * klasa pomocnicza reprezentująca wierzchołek w algorytmie Prima (numer wierzchołka oraz jego 'key')
 */
class PrimVertex {

    int vertexNumber;
    int key;

public:

    PrimVertex(int, int);

    int getVertexNumber();

    int getKey();

    void setKey(int);
};

#endif //SDIZO2_PRIMVERTEX_H
