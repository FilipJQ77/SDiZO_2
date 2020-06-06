#include "ListGraph.h"

ListGraph::ListGraph(int vertices) {
    this->vertices = vertices;
    edges = 0;
    adjacencyLists = new AdjacencyList *[vertices];
    for (int i = 0; i < vertices; ++i) {
        adjacencyLists[i] = new AdjacencyList();
    }
}

ListGraph::~ListGraph() {
    for (int i = 0; i < vertices; ++i)
        delete adjacencyLists[i];
    delete[] adjacencyLists;
}

void ListGraph::addVertex() {
    auto newAdjacencyLists = new AdjacencyList *[vertices + 1];
    for (int i = 0; i < vertices; ++i) {
        newAdjacencyLists[i] = adjacencyLists[i];
        delete adjacencyLists[i];
    }
    newAdjacencyLists[vertices] = new AdjacencyList();
    delete[] adjacencyLists;
    ++vertices;
}

void ListGraph::addUndirectedEdge(int vertex1, int vertex2, int edgeWeight) {
    adjacencyLists[vertex1]->addEdge(vertex2, edgeWeight);
    adjacencyLists[vertex2]->addEdge(vertex1, edgeWeight);
    ++edges;
}

void ListGraph::addDirectedEdge(int vertex1, int vertex2, int edgeWeight) {
    adjacencyLists[vertex1]->addEdge(vertex2, edgeWeight);
    ++edges;
}

void ListGraph::prim(int *&key, int *&parent, int startingVertex = 0) {
    //stos wierzchołków Prima (tzn. obiektów wierzchołek posiadających swój numer, oraz key)
    auto *heap = new PrimVertexHeap(vertices);
    heap->primVertices[startingVertex]->setKey(0);
    key[startingVertex] = 0;
    parent[startingVertex] = -1;
    while (heap->hasElements()) {
        //tworzymy stos (aby mieć wierzchołek o najmniejszej wadze), trzeba co pętlę ponieważ w pętli zmieniają się elementy stosu
        heap->createMinHeap();
        PrimVertex *vertexU = heap->extractMin();
        //neighbourTraverse - obiekt (ListElement) za pomocą którego dobieramy się do wszystkich sąsiadów wierzchołka z listy sąsiedztwa
        auto neighbourTraverse = adjacencyLists[vertexU->getVertexNumber()]->getHead();
        while (neighbourTraverse != nullptr) {
            int neighbour = neighbourTraverse->neighbour;
            if (heap->isElementInHeap(neighbour)) {
                int edgeWeight = neighbourTraverse->edgeWeight;
                int neighbourPosition = heap->position[neighbour];
                if (edgeWeight < heap->primVertices[neighbourPosition]->getKey()) {
                    heap->primVertices[neighbourPosition]->setKey(edgeWeight);
                    key[neighbour] = edgeWeight;
                    parent[neighbour] = vertexU->getVertexNumber();
                }
            }
            neighbourTraverse = neighbourTraverse->next;
        }
    }
    delete heap;
}

void ListGraph::kruskal(Edge **mstEdges) {
    int *parent = new int[vertices];
    int *rank = new int[vertices];
    Edge **graphEdges = new Edge *[2 * edges]; //2*edges bo będziemy mieć duplikaty krawędzi
    int graphEdgeIndex = 0;
    //tworzenie pomocniczych obiektów - krawędzi oraz ich sortowanie metodą insertionsort
    for (int j = 0; j < vertices; ++j) {
        parent[j] = j;
        rank[j] = 0;
        auto listTraverse = adjacencyLists[j]->getHead();
        while (listTraverse != nullptr) {
            graphEdges[graphEdgeIndex] = new Edge(j, listTraverse->neighbour, listTraverse->edgeWeight);
            Edge *swap = graphEdges[graphEdgeIndex];
            int k = graphEdgeIndex - 1;
            while (k >= 0 && graphEdges[k]->getEdgeWeight() > swap->getEdgeWeight()) {
                graphEdges[k + 1] = graphEdges[k];
                --k;
            }
            graphEdges[k + 1] = swap;
            ++graphEdgeIndex;
            listTraverse = listTraverse->next;
        }
    }
    int mstEdgeIndex = 0;
    for (graphEdgeIndex = 0; graphEdgeIndex < 2 * edges; ++graphEdgeIndex) {
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


void ListGraph::dijkstra(int *&distance, int *&parent, int startingVertex = 0) {
    //stos wierzchołków Dijkstry (tzn. obiektów wierzchołek posiadających swój numer, oraz distance)
    auto *heap = new DijkstraVertexHeap(vertices);
    heap->dijkstraVertices[startingVertex]->setDistance(0);
    distance[startingVertex] = 0;
    parent[startingVertex] = -1;
    while (heap->hasElements()) {
        //tworzymy stos (aby mieć wierzchołek o najmniejszym dystansie), trzeba co pętlę ponieważ w pętli zmieniają się elementy stosu
        heap->createMinHeap();
        DijkstraVertex *vertexU = heap->extractMin();
        //neighbourTraverse - obiekt (ListElement) za pomocą którego dobieramy się do wszystkich sąsiadów wierzchołka z listy sąsiedztwa
        auto neighbourTraverse = adjacencyLists[vertexU->getVertexNumber()]->getHead();
        while (neighbourTraverse != nullptr) {
            int neighbour = neighbourTraverse->neighbour;
            int edgeWeight = neighbourTraverse->edgeWeight;
            int neighbourPosition = heap->position[neighbour];
            int distanceU = vertexU->getDistance();
            int distanceV = heap->dijkstraVertices[neighbourPosition]->getDistance();
            if (distanceV > distanceU + edgeWeight) {
                heap->dijkstraVertices[neighbourPosition]->setDistance(distanceU + edgeWeight);
                distance[neighbour] = distanceU + edgeWeight;
                parent[neighbour] = vertexU->getVertexNumber();
            }
            neighbourTraverse = neighbourTraverse->next;
        }
    }
    delete heap;
}

bool ListGraph::bellmanFord(int *&distance, int *&parent, int startingVertex = 0) {
    Edge **graphEdges = new Edge *[edges];
    int i = 0;
    for (int j = 0; j < vertices; ++j) {
        distance[j] = INT_MAX / 2;
        parent[j] = -1;
        auto listTraverse = adjacencyLists[j]->getHead();
        while (listTraverse != nullptr) {
            graphEdges[i] = new Edge(j, listTraverse->neighbour, listTraverse->edgeWeight);
            ++i;
            listTraverse = listTraverse->next;
        }
    }
    parent[startingVertex] = startingVertex;
    distance[startingVertex] = 0;
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
    }
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

void ListGraph::print() {
    for (int i = 0; i < vertices; ++i) {
        cout << "Wierz. " << i << " Nastepnicy: ";
        auto listTraverse = adjacencyLists[i]->getHead();
        while (listTraverse != nullptr) {
            cout << listTraverse->neighbour << " waga: " << listTraverse->edgeWeight << ";\t";
            listTraverse = listTraverse->next;
        }
        cout << "\n";
    }
}

int ListGraph::getVertices() {
    return vertices;
}

int ListGraph::getEdges() {
    return edges;
}