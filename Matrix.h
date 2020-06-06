#ifndef SDIZO2_MATRIX_H
#define SDIZO2_MATRIX_H
#include <iostream>

using std::cout;

/**
 * klasa reprezentująca macierz
 */
class Matrix {

    int **array;
    int sizeX;
    int sizeY;

public:

    Matrix(int, int);

    /**
     * destruktor
     */
    ~Matrix();

    void addColumn();

    void setNewEdge(int, int);

    int get(int, int);

    void set(int, int, int);

    bool areUndirectedConnected(int, int);

    bool areDirectedConnected(int, int);

    /**
     * wypisanie macierzy na ekran
     */
    void print();
};


#endif //SDIZO2_MATRIX_H
