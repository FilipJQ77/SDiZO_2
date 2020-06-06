#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    head = nullptr;
    tail = nullptr;
    listSize = 0;
}


AdjacencyList::~AdjacencyList() {
    while (listSize) {
        removeElement(head); //aby zdealokować pamięć po wszystkich elementach
    }
}

void AdjacencyList::addEdge(int neighbour, int edgeWeight) {
    auto *newElement = new ListElement(neighbour, edgeWeight, tail, nullptr);
    if (tail == nullptr)
        head = newElement; //jeśli ogon nie wskazuje na nic, oznacza to że lista jest pusta, więc dodajemy jedyny element, który będzie jednocześnie ogonem oraz głową
    else
        tail->next = newElement;
    tail = newElement;
}

bool AdjacencyList::removeEdge(int neighbour) {
    ListElement *temp = head;
    while (temp != nullptr) {
        if (temp->neighbour == neighbour) {
            removeElement(temp);
            return true;
        } else
            temp = temp->next;
    }
    return false;
}

void AdjacencyList::removeElement(ListElement *temp) {
    --listSize;
    ListElement *left = temp->prev;
    ListElement *right = temp->next;
    delete temp;
    if (left != nullptr)
        left->next = right;
    if (right != nullptr)
        right->prev = left;
}

ListElement *AdjacencyList::getHead() {
    return head;
}
