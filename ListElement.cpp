#include "ListElement.h"

ListElement::ListElement(int neighbour, int edgeWeight, ListElement *p = nullptr,
                     ListElement *n = nullptr) {
    this->neighbour = neighbour;
    this->edgeWeight = edgeWeight;
    prev = p;
    next = n;
}