#include "Edge.h"

Edge::Edge(int v1, int v2, int weight) {
    vertex1 = v1;
    vertex2 = v2;
    edgeWeight = weight;
}

int Edge::getVertex1() {
    return vertex1;
}

int Edge::getVertex2() {
    return vertex2;
}

int Edge::getEdgeWeight() {
    return edgeWeight;
}

void Edge::setVertex1(int v1) {
    vertex1 = v1;
}

void Edge::setVertex2(int v2) {
    vertex2 = v2;
}

void Edge::setEdgeWeight(int weight) {
    edgeWeight=weight;
}