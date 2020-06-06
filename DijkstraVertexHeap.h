#ifndef SDIZO2_DIJKSTRAVERTEXHEAP_H
#define SDIZO2_DIJKSTRAVERTEXHEAP_H

#include "DijkstraVertex.h"

/**
 * klasa reprezentująca stos, wykorzystywana jako kolejka priorytetowa dla algorytmu Dijkstry, dostosowana klasa stosu z 1 projektu
 */
class DijkstraVertexHeap {

    int heapSize;
    int originalSize;

public:

    DijkstraVertex **dijkstraVertices;
    int *position; // position[i] pokazuje gdzie w stosie jest wierzchołek 'i'

    DijkstraVertexHeap(int);

    ~DijkstraVertexHeap();

    bool isElementInHeap(int);

    bool hasElements();

    void createMinHeap();

    void minHeapifyDown(int);

    DijkstraVertex *extractMin();
};


#endif //SDIZO2_DIJKSTRAVERTEXHEAP_H
