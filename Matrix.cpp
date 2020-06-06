#include "Matrix.h"

Matrix::Matrix(int columns, int rows) {
    sizeX = columns;
    sizeY = rows;
    array = new int *[sizeX];
    for (int i = 0; i < sizeX; ++i) {
        array[i] = new int[sizeY];
        for (int j = 0; j < sizeY; ++j) {
            array[i][j] = 0;
        }
    }
}

/**
 * destruktor
 */
Matrix::~Matrix() {
    for (int i = 0; i < sizeX; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

void Matrix::addColumn() {
    int **newArray = new int *[sizeX + 1];
    for (int i = 0; i < sizeX; ++i) {
        newArray[i] = new int[sizeY];
        for (int j = 0; j < sizeY; ++j) {
            newArray[i][j] = array[i][j];
        }
    }
    newArray[sizeX] = new int[sizeY];
    for (int j = 0; j < sizeY; ++j) {
        newArray[sizeX][j] = 0;
    }
    for (int i = 0; i < sizeX; ++i) {
        delete[] array[i];
    }
    delete[] array;
    ++sizeX;
    array = newArray;
}

void Matrix::setNewEdge(int vertex, int value) {
    array[sizeX - 1][vertex] = value;
}

int Matrix::get(int x, int y) {
    return array[x][y];
}

void Matrix::set(int x, int y, int value) {
    array[x][y] = value;
}

bool Matrix::areUndirectedConnected(int x, int y) {
    for (int i = 0; i < sizeX; ++i) {
        if (array[i][x] != 0 && array[i][y] != 0) {
            return true;
        }
    }
    return false;
}

bool Matrix::areDirectedConnected(int x, int y) {
    for (int i = 0; i < sizeX; ++i) {
        if (array[i][x] == 1 && array[i][y] == -1) {
            return true;
        }
    }
    return false;
}

void Matrix::print() {
    for (int y = 0; y < sizeY; ++y) {
        cout << "Wierz. " << y << "\t";
        for (int x = 0; x < sizeX; ++x) {
            cout << "|" << array[x][y] << "\t";
        }
        cout << "\n";
    }
    cout << "\n";
}