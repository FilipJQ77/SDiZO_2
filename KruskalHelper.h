#ifndef SDIZO2_KRUSKALHELPER_H
#define SDIZO2_KRUSKALHELPER_H


/**
 * klasa pomocnicza do algorytmu Kruskala - metoda FindSet
 */
class KruskalHelper {

public:

    /**
     * metoda znajdująca podzbiór danego wierzchołka jednocześnie kompresując ścieżki
     * @param parent
     * @param x
     * @return
     */
    static int kruskalFindSet(int *, int);
};


#endif //SDIZO2_KRUSKALHELPER_H
