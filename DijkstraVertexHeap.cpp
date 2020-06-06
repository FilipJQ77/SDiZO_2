#include "windows.h"
#include "DijkstraVertexHeap.h"

DijkstraVertexHeap::DijkstraVertexHeap(int vertices) {
    dijkstraVertices = new DijkstraVertex *[vertices];
    position = new int[vertices];
    // stworzenie pomocniczych obiektów - wierzchołków (numer wierzchołka, distance)
    for (int i = 0; i < vertices; ++i) {
        dijkstraVertices[i] = new DijkstraVertex(i, INT_MAX / 2);
        position[i] = i;
    }
    heapSize = vertices;
    originalSize = vertices;
}

DijkstraVertexHeap::~DijkstraVertexHeap() {
    for (int i = 0; i < originalSize; ++i) {
        delete dijkstraVertices[i];
    }
    delete[] dijkstraVertices;
    delete[] position;
}

bool DijkstraVertexHeap::isElementInHeap(int vertex) {
    return position[vertex] < heapSize;
}

bool DijkstraVertexHeap::hasElements() {
    return heapSize > 0;
}

void DijkstraVertexHeap::createMinHeap() {
    for (int i = (heapSize - 2) / 2; i >= 0; --i)
        minHeapifyDown(i);
}

void DijkstraVertexHeap::minHeapifyDown(int parentIndex) {
    int smallestIndex = parentIndex;
    int leftIndex = 2 * parentIndex + 1;
    int rightIndex = 2 * parentIndex + 2;
    if ((leftIndex < heapSize) &&
        dijkstraVertices[leftIndex]->getDistance() < dijkstraVertices[smallestIndex]->getDistance())
        smallestIndex = leftIndex;
    if ((rightIndex < heapSize) &&
        dijkstraVertices[rightIndex]->getDistance() < dijkstraVertices[smallestIndex]->getDistance())
        smallestIndex = rightIndex;
    if (smallestIndex != parentIndex) {
        // zamiana elementów stosu oraz odnośników do ich pozycji
        position[dijkstraVertices[smallestIndex]->getVertexNumber()] = parentIndex;
        position[dijkstraVertices[parentIndex]->getVertexNumber()] = smallestIndex;
        auto swap = dijkstraVertices[parentIndex];
        dijkstraVertices[parentIndex] = dijkstraVertices[smallestIndex];
        dijkstraVertices[smallestIndex] = swap;
        minHeapifyDown(smallestIndex);
    }
}

DijkstraVertex * DijkstraVertexHeap::extractMin() {
    if (heapSize > 0) {
        --heapSize;
        DijkstraVertex *mini = dijkstraVertices[0];
        position[dijkstraVertices[0]->getVertexNumber()] = heapSize;
        position[dijkstraVertices[heapSize]->getVertexNumber()] = 0;
        auto swap = dijkstraVertices[0];
        dijkstraVertices[0] = dijkstraVertices[heapSize];
        dijkstraVertices[heapSize] = swap;
        return mini;
    }
    return nullptr;
}