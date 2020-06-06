#ifndef SDIZO2_PRIMVERTEXHEAP_H
#define SDIZO2_PRIMVERTEXHEAP_H

#include "PrimVertex.h"

/**
 * klasa reprezentująca stos, wykorzystywana jako kolejka priorytetowa dla algorytmu Prima, dostosowana klasa stosu z 1 projektu
 */
class PrimVertexHeap {

    int heapSize;
    int originalSize;

public:

    PrimVertex **primVertices;
    int *position; // position[i] pokazuje gdzie w stosie jest wierzchołek 'i'

    PrimVertexHeap(int);

    ~PrimVertexHeap();

    bool isElementInHeap(int);

    bool hasElements();

    void createMinHeap();

    void minHeapifyDown(int);

    PrimVertex *extractMin();
};


#endif //SDIZO2_PRIMVERTEXHEAP_H
