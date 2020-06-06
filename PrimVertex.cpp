#include "PrimVertex.h"

PrimVertex::PrimVertex(int vertexNumberr, int keyy) {
    vertexNumber = vertexNumberr;
    key = keyy;
}

int PrimVertex::getVertexNumber() {
    return vertexNumber;
}

int PrimVertex::getKey() {
    return key;
}

void PrimVertex::setKey(int keyy) {
    key = keyy;
}