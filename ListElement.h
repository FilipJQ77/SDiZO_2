#ifndef SDIZO2_LISTELEMENT_H
#define SDIZO2_LISTELEMENT_H


/**
     * pomocnicza klasa reprezentująca pojedynczy element listy
     */
class ListElement {
public:
    int neighbour; //wartość elementu
    int edgeWeight;
    ListElement *prev; //wskaźnik na poprzedni element
    ListElement *next; //wskaźnik na następny element

    explicit ListElement(int, int, ListElement *, ListElement *);
};


#endif //SDIZO2_LISTELEMENT_H
