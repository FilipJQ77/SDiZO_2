#include <windows.h>
#include "PrimVertexHeap.h"

PrimVertexHeap::PrimVertexHeap(int vertices) {
    primVertices = new PrimVertex *[vertices];
    position = new int[vertices];
    // stworzenie pomocniczych obiektów - wierzchołków (numer wierzchołka, key)
    for (int i = 0; i < vertices; ++i) {
        primVertices[i] = new PrimVertex(i, INT_MAX / 2); // INT_MAX/2 jako nieskończoność
        position[i] = i;
    }
    heapSize = vertices;
    originalSize = vertices;
}

PrimVertexHeap::~PrimVertexHeap() {
    for (int i = 0; i < originalSize; ++i) {
        delete primVertices[i];
    }
    delete[] primVertices;
    delete[] position;
}

bool PrimVertexHeap::isElementInHeap(int vertex) {
    return position[vertex] < heapSize;
}

bool PrimVertexHeap::hasElements() {
    return heapSize > 0;
}

void PrimVertexHeap::createMinHeap() {
    for (int i = (heapSize - 2) / 2; i >= 0; --i)
        minHeapifyDown(i);
}

void PrimVertexHeap::minHeapifyDown(int parentIndex) {
    int smallestIndex = parentIndex;
    int leftIndex = 2 * parentIndex + 1;
    int rightIndex = 2 * parentIndex + 2;
    if ((leftIndex < heapSize) && primVertices[leftIndex]->getKey() < primVertices[smallestIndex]->getKey())
        smallestIndex = leftIndex;
    if ((rightIndex < heapSize) && primVertices[rightIndex]->getKey() < primVertices[smallestIndex]->getKey())
        smallestIndex = rightIndex;
    if (smallestIndex != parentIndex) {
        // zamiana elementów stosu oraz odnośników do ich pozycji
        position[primVertices[smallestIndex]->getVertexNumber()] = parentIndex;
        position[primVertices[parentIndex]->getVertexNumber()] = smallestIndex;
        auto swap = primVertices[parentIndex];
        primVertices[parentIndex] = primVertices[smallestIndex];
        primVertices[smallestIndex] = swap;
        minHeapifyDown(smallestIndex);
    }
}

PrimVertex* PrimVertexHeap::extractMin() {
    if (heapSize > 0) {
        --heapSize;
        PrimVertex *mini = primVertices[0];
        position[primVertices[0]->getVertexNumber()] = heapSize;
        position[primVertices[heapSize]->getVertexNumber()] = 0;
        auto swap = primVertices[0];
        primVertices[0] = primVertices[heapSize];
        primVertices[heapSize] = swap;
        return mini;
    }
    return nullptr;
}