#include "DijkstraVertex.h"

DijkstraVertex::DijkstraVertex(int vertexNumberr, int distancee) {
    vertexNumber = vertexNumberr;
    distance = distancee;
}

int DijkstraVertex::getVertexNumber() {
    return vertexNumber;
}

int DijkstraVertex::getDistance() {
    return distance;
}

void DijkstraVertex::setDistance(int distancee) {
    distance = distancee;
}