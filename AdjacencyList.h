#ifndef SDIZO2_ADJACENCYLIST_H
#define SDIZO2_ADJACENCYLIST_H

#include "ListElement.h"


/**
 * klasa reprezentująca listę sąsiedztwa/następników wierzchołka, dostosowana klasa z 1 projektu
 */
class AdjacencyList {

    ListElement *head;
    ListElement *tail;
    int listSize;

public:

    /**
     * stworzenie nowej pustej listy
     */
    AdjacencyList();

    /**
     * destruktor
     */
    ~AdjacencyList();

    void addEdge(int , int);

    bool removeEdge(int);

    void removeElement(ListElement *);

    ListElement *getHead();

};


#endif //SDIZO2_ADJACENCYLIST_H
