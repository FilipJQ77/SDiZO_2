#include "MatrixGraph.h"
#include "PrimVertexHeap.h"
#include "KruskalHelper.h"
#include "DijkstraVertexHeap.h"

MatrixGraph::MatrixGraph(int vertices) {
    this->vertices = vertices;
    edges = 0;
    edgeWeights = new int[edges];
    incidenceMatrix = new Matrix(0, vertices);
}

MatrixGraph::~MatrixGraph() {
    delete incidenceMatrix;
    delete[] edgeWeights;
}

void MatrixGraph::addUndirectedEdge(int vertex1, int vertex2, int edgeWeight) {
    incidenceMatrix->addColumn();
    incidenceMatrix->setNewEdge(vertex1, 1);
    incidenceMatrix->setNewEdge(vertex2, 1);
    newEdgeWeights(edgeWeight);
    ++edges;
}

void MatrixGraph::addDirectedEdge(int vertex1, int vertex2, int edgeWeight) {
    incidenceMatrix->addColumn();
    incidenceMatrix->setNewEdge(vertex1, 1);
    incidenceMatrix->setNewEdge(vertex2, -1);
    newEdgeWeights(edgeWeight);
    ++edges;
}

void MatrixGraph::newEdgeWeights(int addedEdgeWeight) {
    int *newEdgeWeights = new int[edges + 1];
    for (int i = 0; i < edges; ++i) {
        newEdgeWeights[i] = edgeWeights[i];
    }
    newEdgeWeights[edges] = addedEdgeWeight;
    delete[] edgeWeights;
    edgeWeights = newEdgeWeights;
}


void MatrixGraph::prim(int *&key, int *&parent, int startingVertex) {
    //stos wierzchołków Prima (tzn. obiektów wierzchołek posiadających swój numer, oraz key)
    auto *heap = new PrimVertexHeap(vertices);
    heap->primVertices[startingVertex]->setKey(0);
    key[startingVertex] = 0;
    parent[startingVertex] = -1;
    while (heap->hasElements()) {
        //tworzymy stos (aby mieć wierzchołek o najmniejszej wadze), trzeba co pętlę ponieważ w pętli zmieniają się elementy stosu
        heap->createMinHeap();
        PrimVertex *vertexU = heap->extractMin();
        int vertexNumber = vertexU->getVertexNumber();
        for (int i = 0; i < edges; ++i) {
            if (incidenceMatrix->get(i, vertexNumber) == 1) {
                int edgeWeight = edgeWeights[i];
                for (int j = 0; j < vertices; ++j) {
                    if (incidenceMatrix->get(i, j) == 1 && j != vertexNumber) {
                        // 'j' to sąsiad (neighbour)
                        if (heap->isElementInHeap(j)) {
                            int neighbourPosition = heap->position[j];
                            if (edgeWeight < heap->primVertices[neighbourPosition]->getKey()) {
                                heap->primVertices[neighbourPosition]->setKey(edgeWeight);
                                key[j] = edgeWeight;
                                parent[j] = vertexU->getVertexNumber();
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    delete heap;
}

void MatrixGraph::kruskal(Edge **mstEdges) {
    int *parent = new int[vertices];
    int *rank = new int[vertices];
    Edge **graphEdges = new Edge *[edges];
    int graphEdgeIndex = 0;
    int mstEdgeIndex = 0;
    for (int i = 0; i < vertices; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
    //tworzenie pomocniczych obiektów - krawędzi oraz ich sortowanie metodą insertionsort
    for (int i = 0; i < edges; ++i) {
        int vertex1 = 0;
        int vertex2 = 0;
        int j = 0;
        for (; j < edges; ++j) {
            if (incidenceMatrix->get(i, j) == 1) {
                vertex1 = j;
                ++j;
                break;
            }
        }
        for (; j < edges; ++j) {
            if (incidenceMatrix->get(i, j) == 1) {
                vertex2 = j;
                break;
            }
        }
        graphEdges[graphEdgeIndex] = new Edge(vertex1, vertex2, edgeWeights[i]);
        Edge *swap = graphEdges[graphEdgeIndex];
        int k = graphEdgeIndex - 1;
        while (k >= 0 && graphEdges[k]->getEdgeWeight() > swap->getEdgeWeight()) {
            graphEdges[k + 1] = graphEdges[k];
            --k;
        }
        graphEdges[k + 1] = swap;
        ++graphEdgeIndex;
    }
    for (graphEdgeIndex = 0; graphEdgeIndex < edges; ++graphEdgeIndex) {
        Edge *edge = graphEdges[graphEdgeIndex];
        int v1 = edge->getVertex1();
        int v2 = edge->getVertex2();
        int set1 = KruskalHelper::kruskalFindSet(parent, v1);
        int set2 = KruskalHelper::kruskalFindSet(parent, v2);
        if (set1 != set2) {
            mstEdges[mstEdgeIndex]->setVertex1(v1);
            mstEdges[mstEdgeIndex]->setVertex2(v2);
            mstEdges[mstEdgeIndex]->setEdgeWeight(edge->getEdgeWeight());
            ++mstEdgeIndex;
            if (rank[set1] < rank[set2])
                parent[set1] = set2;
            else
                parent[set2] = set1;
            if (rank[set1] == rank[set2])
                ++rank[set1];
        }
        delete graphEdges[graphEdgeIndex];
    }
    delete[] parent;
    delete[] rank;
    delete[] graphEdges;
}

void MatrixGraph::dijkstra(int *&distance, int *&parent, int startingVertex) {
    auto *heap = new DijkstraVertexHeap(vertices);
    heap->dijkstraVertices[startingVertex]->setDistance(0);
    distance[startingVertex] = 0;
    parent[startingVertex] = -1;
    while (heap->hasElements()) {
        heap->createMinHeap();
        DijkstraVertex *vertexU = heap->extractMin();
        int vertexNumber = vertexU->getVertexNumber();
        for (int i = 0; i < edges; ++i) {
            if (incidenceMatrix->get(i, vertexNumber) == 1) {
                int edgeWeight = edgeWeights[i];
                for (int j = 0; j < vertices; ++j) {
                    if (incidenceMatrix->get(i, j) == -1) {
                        // 'j' to sąsiad (neighbour)
                        int neighbourPosition = heap->position[j];
                        int distanceU = vertexU->getDistance();
                        int distanceV = heap->dijkstraVertices[neighbourPosition]->getDistance();
                        if (distanceV > distanceU + edgeWeight) {
                            heap->dijkstraVertices[neighbourPosition]->setDistance(distanceU + edgeWeight);
                            distance[j] = distanceU + edgeWeight;
                            parent[j] = vertexU->getVertexNumber();
                        }
                        break;
                    }
                }
            }
        }
    }
    delete heap;
}

bool MatrixGraph::bellmanFord(int *&distance, int *&parent, int startingVertex) {
    for (int i = 0; i < vertices; ++i) {
        distance[i] = INT_MAX / 2;
        parent[i] = -1;
    }
    parent[startingVertex] = startingVertex;
    distance[startingVertex] = 0;
    Edge **graphEdges = new Edge *[edges];
    int graphEdgeIndex = 0;
    for (int i = 0; i < edges; ++i) {
        int vertex1 = 0;
        int vertex2 = 0;
        for (int j = 0; j < edges; ++j) {
            if (incidenceMatrix->get(i, j) == 1) {
                vertex1 = j;
                break;
            }
        }
        for (int j = 0; j < edges; ++j) {
            if (incidenceMatrix->get(i, j) == -1) {
                vertex2 = j;
                break;
            }
        }
        graphEdges[graphEdgeIndex] = new Edge(vertex1, vertex2, edgeWeights[i]);
        ++graphEdgeIndex;
    }
    bool relaxed = true;
    for (int ii = 1; ii < vertices && relaxed; ++ii) {
        relaxed = false;
        for (int j = 0; j < edges; ++j) {
            Edge *edge = graphEdges[j];
            int u = edge->getVertex1();
            int v = edge->getVertex2();
            int weight = edge->getEdgeWeight();
            if (distance[v] > distance[u] + weight) {
                distance[v] = distance[u] + weight;
                parent[v] = u;
                relaxed = true;
            }
        }
    }//todo
    if (relaxed) {
        for (int j = 0; j < edges; ++j) {
            Edge *edge = graphEdges[j];
            int u = edge->getVertex1();
            int v = edge->getVertex2();
            int weight = edge->getEdgeWeight();
            if (distance[v] > distance[u] + weight) {
                delete[] distance;
                delete[] parent;
                return false;
            }
        }
    }
    for (int j = 0; j < edges; ++j) {
        delete graphEdges[j];
    }
    delete[] graphEdges;
    return true;
}

void MatrixGraph::print() {
    cout << "Waga kraw.";
    for (int i = 0; i < edges; ++i) {
        cout << "\t|" << edgeWeights[i];
    }
    cout << "\n\n";
    incidenceMatrix->print();
}

int MatrixGraph::getVertices() {
    return vertices;
}

int MatrixGraph::getEdges() {
    return edges;
}