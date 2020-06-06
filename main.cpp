#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <ctime>
#include "Edge.h"
#include "Matrix.h"
#include "ListGraph.h"
#include "MatrixGraph.h"

using std::cin;
using std::cout;
using std::string;
using std::fstream;

/**
 * fragment kodu z https://stackoverflow.com/questions/1739259/how-to-use-queryperformancecounter
 * odpowiadający za przygotowanie QueryPerformanceCounter oraz metod umożliwiających mierzenie czasu
 */
double pcFreq = 0.0;
__int64 counterStart = 0;

/**
 * włącza (od nowa) stoper
 */
void timerStart() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";
    pcFreq = double(li.QuadPart) / 1000000.0;
    QueryPerformanceCounter(&li);
    counterStart = li.QuadPart;
}

/**
 * zwraca czas w mikrosekundach od momentu ostatniego uruchomienia stopera
 * @return
 */
double timerTime() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - counterStart) / pcFreq;
}

/**
 * losuje liczbę z danego przedziału
 * @param mini
 * @param maxi
 * @return
 */
int randomNumber(int mini, int maxi) {
    return mini + rand() % (maxi - mini + 1);
}

/**
 * generuje losowy spójny nieskierowany graf
 * @param vertices
 * @param density
 * @param listGraph
 * @param matrixGraph
 */
void generateUndirectedEdges(int vertices, float density, ListGraph *listGraph, MatrixGraph *matrixGraph) {
    int possibleEdges = vertices * (vertices - 1) / 2;
    int edges = (int) ((float) possibleEdges * density / 100);
    // minimalna liczba krawędzi aby graf był spójny
    if (edges < vertices - 1) {
        edges = vertices - 1;
    }
    int *edgeWeights = new int[edges];
    for (int i = 0; i < edges; ++i) {
        edgeWeights[i] = randomNumber(1, 999);
    }
    Matrix *matrix = new Matrix(edges, vertices);
    int *verticesArray = new int[vertices - 1];
    for (int i = 0; i < vertices - 1; ++i) {
        verticesArray[i] = i + 1;
    }
    int vertexStart = 0;
    int vertexEnd = 0;
    int edgeIndex = 0;
    // losujemy krawędzie, gdzie nowa krawędź zaczyna się tam gdzie skończyła poprzednia
    // aby mieć pewność, że da się dojść z każdego wierzchołka do każdego wierzchołka
    for (int i = vertices - 1; i > 0; --i) {
        vertexEnd = randomNumber(0, i - 1);
        matrix->set(edgeIndex, vertexStart, 1);
        matrix->set(edgeIndex, verticesArray[vertexEnd], 1);
        listGraph->addUndirectedEdge(vertexStart, verticesArray[vertexEnd], edgeWeights[edgeIndex]);
        matrixGraph->addUndirectedEdge(vertexStart, verticesArray[vertexEnd], edgeWeights[edgeIndex]);
        ++edgeIndex;
        vertexStart = verticesArray[vertexEnd];
        verticesArray[vertexEnd] = verticesArray[i - 1];
    }
    delete[] verticesArray;
    int *edgeStart = new int[possibleEdges];
    int *edgeEnd = new int[possibleEdges];
    int index = 0;
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            edgeStart[index] = i;
            ++index;
        }
    }
    index = 0;
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            edgeEnd[index] = j;
            ++index;
        }
    }
    //losujemy krawędź, jeśli już taka krawędź istnieje to usuwamy ze zbioru możliwych krawędzi, jeśli nie to dodajemy do grafu
    for (; edgeIndex < edges; ++edgeIndex) {
        int chosenEdge = randomNumber(0, possibleEdges - 1);
        while (matrix->areUndirectedConnected(edgeStart[chosenEdge], edgeEnd[chosenEdge])) {
            edgeStart[chosenEdge] = edgeStart[possibleEdges - 1];
            edgeEnd[chosenEdge] = edgeEnd[possibleEdges - 1];
            --possibleEdges;
            chosenEdge = randomNumber(0, possibleEdges - 1);
        }
        matrix->set(edgeIndex, edgeStart[chosenEdge], 1);
        matrix->set(edgeIndex, edgeEnd[chosenEdge], 11);
        listGraph->addUndirectedEdge(edgeStart[chosenEdge], edgeEnd[chosenEdge], edgeWeights[edgeIndex]);
        matrixGraph->addUndirectedEdge(edgeStart[chosenEdge], edgeEnd[chosenEdge], edgeWeights[edgeIndex]);
        edgeStart[chosenEdge] = edgeStart[possibleEdges - 1];
        edgeEnd[chosenEdge] = edgeEnd[possibleEdges - 1];
        --possibleEdges;
    }
    delete matrix;
    delete[] edgeWeights;
    delete[] edgeStart;
    delete[] edgeEnd;
}

/**
 * generuje losowy spójny skierowany graf
 * @param vertices
 * @param density
 * @param listGraph
 * @param matrixGraph
 */
void generateDirectedEdges(int vertices, float density, ListGraph *listGraph, MatrixGraph *matrixGraph) {
    int possibleEdges = vertices * (vertices - 1);
    int edges = (int) ((float) possibleEdges * density / 100);
    if (edges < vertices) {
        edges = vertices;
    }
    int *edgeWeights = new int[edges];
    for (int i = 0; i < edges; ++i) {
        edgeWeights[i] = randomNumber(1, 999);
    }
    Matrix *matrix = new Matrix(edges, vertices);
    int *verticesArray = new int[vertices - 1];
    for (int i = 0; i < vertices - 1; ++i) {
        verticesArray[i] = i + 1;
    }
    int vertexStart = 0;
    int vertexEnd = 0;
    int edgeIndex = 0;
    // losujemy krawędzie, gdzie nowa krawędź zaczyna się tam gdzie skończyła poprzednia
    // aby mieć cykl, żeby dało się dojść z każdego wierzchołka do każdego wierzchołka
    for (int i = vertices - 1; i > 0; --i) {
        vertexEnd = randomNumber(0, i - 1);
        matrix->set(edgeIndex, vertexStart, 1);
        matrix->set(edgeIndex, verticesArray[vertexEnd], -1);
        listGraph->addDirectedEdge(vertexStart, verticesArray[vertexEnd], edgeWeights[edgeIndex]);
        matrixGraph->addDirectedEdge(vertexStart, verticesArray[vertexEnd], edgeWeights[edgeIndex]);
        ++edgeIndex;
        vertexStart = verticesArray[vertexEnd];
        verticesArray[vertexEnd] = verticesArray[i - 1];
    }
    delete[] verticesArray;
    matrix->set(edgeIndex, vertexStart, 1);
    matrix->set(edgeIndex, 0, -1);
    listGraph->addDirectedEdge(vertexStart, 0, edgeWeights[edgeIndex]);
    matrixGraph->addDirectedEdge(vertexStart, 0, edgeWeights[edgeIndex]);
    ++edgeIndex;
    int *edgeStart = new int[possibleEdges];
    int *edgeEnd = new int[possibleEdges];
    int index = 0;
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j) {
                edgeStart[index] = i;
                edgeEnd[index] = j;
                ++index;
            }
        }
    }
    //losujemy krawędź, jeśli już taka krawędź istnieje to usuwamy ze zbioru możliwych krawędzi, jeśli nie to dodajemy do grafu
    for (; edgeIndex < edges; ++edgeIndex) {
        int chosenEdge = randomNumber(0, possibleEdges - 1);
        while (matrix->areDirectedConnected(edgeStart[chosenEdge], edgeEnd[chosenEdge])) {
            edgeStart[chosenEdge] = edgeStart[possibleEdges - 1];
            edgeEnd[chosenEdge] = edgeEnd[possibleEdges - 1];
            --possibleEdges;
            chosenEdge = randomNumber(0, possibleEdges - 1);
        }
        matrix->set(edgeIndex, edgeStart[chosenEdge], 1);
        matrix->set(edgeIndex, edgeEnd[chosenEdge], -1);
        listGraph->addDirectedEdge(edgeStart[chosenEdge], edgeEnd[chosenEdge], edgeWeights[edgeIndex]);
        matrixGraph->addDirectedEdge(edgeStart[chosenEdge], edgeEnd[chosenEdge], edgeWeights[edgeIndex]);
        edgeStart[chosenEdge] = edgeStart[possibleEdges - 1];
        edgeEnd[chosenEdge] = edgeEnd[possibleEdges - 1];
        --possibleEdges;
    }
    delete matrix;
    delete[] edgeWeights;
    delete[] edgeStart;
    delete[] edgeEnd;
}

void mst() {
    string inputS;
    int inputI;
    int verticesNumber = 2;
    int edgesNumber = 1;
    ListGraph *listGraph = new ListGraph(2);
    MatrixGraph *matrixGraph = new MatrixGraph(2);
    listGraph->addUndirectedEdge(0, 1, 1);
    matrixGraph->addUndirectedEdge(0, 1, 1);
    bool run = true;
    while (run) {
        cout
                << "1. Wczytaj graf z pliku\n2. Wygeneruj losowy graf\n3. Wyswietl graf w postaci list oraz macierzy\n4. Algorytm Prima na aktualnym grafie macierzowo i listowo z wyswietleniem wynikow\n5. Algorytm Kruskala jw.\n0. Wyjdz\n";
        getline(cin, inputS);
        try {
            inputI = stoi(inputS);
        } catch (std::exception &e) {
            cout << "Wyjatek: " << e.what() << "\n";
            continue;
        }
        switch (inputI) {
            case 1: {
                cout << "Podaj nazwe pliku lub jego sciezke: ";
                getline(cin, inputS);
                fstream file;
                file.open(inputS, std::ios::in);
                if (file.good()) {
                    delete listGraph;
                    delete matrixGraph;
                    file >> edgesNumber >> verticesNumber;
                    listGraph = new ListGraph(verticesNumber);
                    matrixGraph = new MatrixGraph(verticesNumber);
                    int v1, v2, weight;
                    for (int j = 0; j < edgesNumber; ++j) {
                        file >> v1 >> v2 >> weight;
                        listGraph->addUndirectedEdge(v1, v2, weight);
                        matrixGraph->addUndirectedEdge(v1, v2, weight);
                    }
                } else cout << "Wystapil problem podczas otwierania pliku\n";
                break;
            }
            case 2: {
                cout << "Podaj liczbe wierzcholkow\n";
                int vertices;
                getline(cin, inputS);
                try {
                    vertices = stoi(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                cout << "Podaj gestosc grafu w procentach\n";
                float density;
                getline(cin, inputS);
                try {
                    density = stof(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                delete listGraph;
                listGraph = new ListGraph(vertices);
                delete matrixGraph;
                matrixGraph = new MatrixGraph(vertices);
                generateUndirectedEdges(vertices, density, listGraph, matrixGraph);
                verticesNumber = vertices;
                edgesNumber = listGraph->getEdges();
                break;
            }
            case 3: {
                cout << "Graf listowo:\n";
                listGraph->print();
                cout << "Graf macierzowo\n";
                matrixGraph->print();
                break;
            }
            case 4: {
                cout << "Podaj wierzcholek poczatkowy\n";
                getline(cin, inputS);
                try {
                    inputI = stoi(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                int *key = new int[verticesNumber];
                int *parent = new int[verticesNumber];
                listGraph->prim(key, parent, inputI);
                cout << "Algorytm Prima listowo; wierzcholek: key/parent\n";
                for (int i = 0; i < verticesNumber; ++i) {
                    cout << i << ": " << key[i] << "/" << parent[i] << "\n";
                }
                cout << "\nKrawedzie MST:\n";
                for (int i = 0; i < verticesNumber; ++i) {
                    if (parent[i] != -1) {
                        cout << i << " - " << parent[i] << " : Waga = " << key[i] << "\n";
                    }
                }
                matrixGraph->prim(key, parent, inputI);
                cout << "\nAlgorytm Prima macierzowo; wierzcholek: key/parent\n";
                for (int i = 0; i < verticesNumber; ++i) {
                    cout << i << ": " << key[i] << "/" << parent[i] << "\n";

                }
                cout << "\nKrawedzie MST:\n";
                for (int i = 0; i < verticesNumber; ++i) {
                    if (parent[i] != -1) {
                        cout << i << " - " << parent[i] << " : Waga = " << key[i] << "\n";
                    }
                }
                cout << "\n";
                delete[] key;
                delete[] parent;
                break;
            }
            case 5: {
                Edge **mstEdges = new Edge *[verticesNumber - 1];
                for (int i = 0; i < verticesNumber-1; i++)
                {
                    mstEdges[i] = new Edge(0,0,0);
                }
                listGraph->kruskal(mstEdges);
                cout << "Algorytm Kruskala listowo; krawedzie MST:\n";
                for (int i = 0; i < verticesNumber - 1; ++i) {
                    cout << mstEdges[i]->getVertex1() << " - " << mstEdges[i]->getVertex2() << " : Waga = "
                         << mstEdges[i]->getEdgeWeight()
                         << "\n";
                }
                matrixGraph->kruskal(mstEdges);
                cout << "Algorytm Kruskala macierzowo; krawedzie MST:\n";
                for (int i = 0; i < verticesNumber - 1; ++i) {
                    cout << mstEdges[i]->getVertex1() << " - " << mstEdges[i]->getVertex2() << " : Waga = "
                         << mstEdges[i]->getEdgeWeight()
                         << "\n";
                }
                for (int i = 0; i < verticesNumber - 1; ++i) {
                    delete mstEdges[i];
                }
                delete[] mstEdges;
                break;
            }
            case 0: {
                run = false;
                break;
            }
            default:
                break;
        }
    }
}

void spf() {
    string inputS;
    int inputI;
    int verticesNumber = 2;
    int edgesNumber = 1;
    ListGraph *listGraph = new ListGraph(2);
    MatrixGraph *matrixGraph = new MatrixGraph(2);
    listGraph->addDirectedEdge(0, 1, 1);
    matrixGraph->addDirectedEdge(0, 1, 1);
    bool run = true;
    while (run) {
        cout
                << "1. Wczytaj graf z pliku\n2. Wygeneruj losowy graf\n3. Wyswietl graf w postaci list oraz macierzy\n4. Algorytm Dijkstry na aktualnym grafie macierzowo i listowo z wyswietleniem wynikow\n5. Algorytm Bellmana-Forda jw.\n0. Wyjdz\n";
        getline(cin, inputS);
        try {
            inputI = stoi(inputS);
        } catch (std::exception &e) {
            cout << "Wyjatek: " << e.what() << "\n";
            continue;
        }
        switch (inputI) {
            case 1: {
                cout << "Podaj nazwe pliku lub jego sciezke: ";
                getline(cin, inputS);
                fstream file;
                file.open(inputS, std::ios::in);
                if (file.good()) {
                    delete listGraph;
                    delete matrixGraph;
                    file >> edgesNumber >> verticesNumber;
                    listGraph = new ListGraph(verticesNumber);
                    matrixGraph = new MatrixGraph(verticesNumber);
                    int v1, v2, weight;
                    for (int j = 0; j < edgesNumber; ++j) {
                        file >> v1 >> v2 >> weight;
                        listGraph->addDirectedEdge(v1, v2, weight);
                        matrixGraph->addDirectedEdge(v1, v2, weight);
                    }
                } else cout << "Wystapil problem podczas otwierania pliku\n";
                break;
            }
            case 2: {
                cout << "Podaj liczbe wierzcholkow\n";
                int vertices;
                getline(cin, inputS);
                try {
                    vertices = stoi(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                cout << "Podaj gestosc grafu w procentach\n";
                float density;
                getline(cin, inputS);
                try {
                    density = stof(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                delete listGraph;
                listGraph = new ListGraph(vertices);
                delete matrixGraph;
                matrixGraph = new MatrixGraph(vertices);
                generateDirectedEdges(vertices, density, listGraph, matrixGraph);
                verticesNumber = vertices;
                edgesNumber = listGraph->getEdges();
                break;
            }
            case 3: {
                cout << "Graf listowo:\n";
                listGraph->print();
                cout << "Graf macierzowo\n";
                matrixGraph->print();
                break;
            }
            case 4: {
                cout << "Podaj wierzcholek poczatkowy\n";
                getline(cin, inputS);
                try {
                    inputI = stoi(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                int *distance = new int[verticesNumber];
                int *parent = new int[verticesNumber];
                listGraph->dijkstra(distance, parent, inputI);
                cout << "Algorytm Dijkstry listowo; Wierzcholek: distance/parent\n";
                for (int i = 0; i < verticesNumber; ++i) {
                    cout << i << ": " << distance[i] << "/" << parent[i] << "\n";
                }
                matrixGraph->dijkstra(distance, parent, inputI);
                cout << "Algorytm Dijkstry macierzowo; Wierzcholek: distance/parent\n";
                for (int i = 0; i < verticesNumber; ++i) {
                    cout << i << ": " << distance[i] << "/" << parent[i] << "\n";
                }
                delete[] distance;
                delete[] parent;
                break;
            }
            case 5: {
                cout << "Podaj wierzcholek poczatkowy\n";
                getline(cin, inputS);
                try {
                    inputI = stoi(inputS);
                } catch (std::exception &e) {
                    cout << "Wyjatek: " << e.what() << "\n";
                    continue;
                }
                int *distance = new int[verticesNumber];
                int *parent = new int[verticesNumber];
                if (!listGraph->bellmanFord(distance, parent, inputI)) {
                    cout << "Wykryto cykl o lacznej ujemnej wadze\n";
                } else {
                    cout << "Algorytm Bellmana-Forda listowo; Wierzcholek: distance/parent\n";
                    for (int i = 0; i < verticesNumber; ++i) {
                        cout << i << ": " << distance[i] << "/" << parent[i] << "\n";
                    }
                }
                if (!matrixGraph->bellmanFord(distance, parent, inputI)) {
                    cout << "Wykryto cykl o lacznej ujemnej wadze\n";
                } else {
                    cout << "Algorytm Bellmana-Forda macierzowo; Wierzcholek: distance/parent\n";
                    for (int i = 0; i < verticesNumber; ++i) {
                        cout << i << ": " << distance[i] << "/" << parent[i] << "\n";
                    }
                }
                delete[] distance;
                delete[] parent;
                break;
            }
            case 0: {
                run = false;
                break;
            }
        }
    }
}

void completeTest() {
    ListGraph *listGraph;
    MatrixGraph *matrixGraph;
    int verticesNumbers[] = {20, 40, 60, 80, 100};
    float densityValues[] = {25, 50, 75, 99};
    cout << "Liczba wierzcholkow;Gestosc;Prim lista;Prim macierz;Kruskal lista;Kruskal macierz;Dijkstra lista;Dijkstra macierz;Bellman-Ford lista;Bellman-Ford macierz\n";
    for (int vertices : verticesNumbers) {
        for (float density : densityValues) {
            double timeListPrim = 0;
            double timeMatrixPrim = 0;
            double timeListKruskal = 0;
            double timeMatrixKruskal = 0;
            double timeListDijkstra = 0;
            double timeMatrixDijkstra = 0;
            double timeListBellmanFord = 0;
            double timeMatrixBellmanFord = 0;
            int numberOfTests = 100;
            for (int i = 0; i < numberOfTests; ++i) {
                listGraph = new ListGraph(vertices);
                matrixGraph = new MatrixGraph(vertices);
                generateUndirectedEdges(vertices, density, listGraph, matrixGraph);
                int startingVertex = randomNumber(0, vertices - 1);
                int *key = new int[vertices];
                int *parent = new int[vertices];
                int *distance = new int[vertices];
                Edge **mstEdges = new Edge *[vertices - 1];
                for (int i = 0; i < vertices - 1; i++)
                {
                    mstEdges[i] = new Edge(0, 0, 0);
                }
                timerStart();
                listGraph->prim(key, parent, startingVertex);
                timeListPrim += timerTime();
                timerStart();
                matrixGraph->prim(key, parent, startingVertex);
                timeMatrixPrim += timerTime();
                timerStart();
                listGraph->kruskal(mstEdges);
                timeListKruskal += timerTime();
                timerStart();
                matrixGraph->kruskal(mstEdges);
                timeMatrixKruskal += timerTime();
                delete listGraph;
                delete matrixGraph;
                listGraph = new ListGraph(vertices);
                matrixGraph = new MatrixGraph(vertices);
                generateDirectedEdges(vertices, density, listGraph, matrixGraph);
                timerStart();
                listGraph->dijkstra(distance, parent, startingVertex);
                timeListDijkstra += timerTime();
                timerStart();
                matrixGraph->dijkstra(distance, parent, startingVertex);
                timeMatrixDijkstra += timerTime();
                timerStart();
                listGraph->bellmanFord(distance, parent, startingVertex);
                timeListBellmanFord += timerTime();
                timerStart();
                matrixGraph->bellmanFord(distance, parent, startingVertex);
                timeMatrixBellmanFord += timerTime();
                delete[] distance;
                for (int j = 0; j < vertices - 1; ++j) {
                    delete mstEdges[j];
                }
                delete[] mstEdges;
                delete[] key;
                delete[] parent;
                delete listGraph;
                delete matrixGraph;
            }
            timeListPrim /= 100;
            timeMatrixPrim /= 100;
            timeListKruskal /= 100;
            timeMatrixKruskal /= 100;
            timeListDijkstra /= 100;
            timeMatrixDijkstra /= 100;
            timeListBellmanFord /= 100;
            timeMatrixBellmanFord /= 100;
            cout << vertices << ";" << density << ";" << timeListPrim << ";" << timeMatrixPrim << ";" << timeListKruskal
                 << ";" << timeMatrixKruskal << ";" << timeListDijkstra << ";" << timeMatrixDijkstra << ";"
                 << timeListBellmanFord << ";" << timeMatrixBellmanFord << "\n";
        }
    }
}

void menu() {
    bool run = true;
    string inputS;
    int inputI;
    while (run) {
        cout << "1. Problem MST\n2. Problem SPF\n3. Kompletny test do sprawozdania\n0. Wyjscie\n";
        getline(cin, inputS);
        try {
            inputI = stoi(inputS);
        } catch (std::exception &e) {
            cout << "Wyjatek: " << e.what() << "\n";
            continue;
        }
        switch (inputI) {
            case 1:
                cout << "STWORZ NOWY GRAF!\n";
                mst();
                break;
            case 2:
                cout << "STWORZ NOWY GRAF!\n";
                spf();
                break;
            case 3:
                cout << "Czy na pewno? Nie mozna przerwac tego testu. Wpisz y aby przejsc dalej\n";
                getline(cin, inputS);
                if (inputS == "Y" || inputS == "y")
                    completeTest();
                break;
            case 0:
                run = false;
                break;
            default:
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    menu();
    return 0;
}