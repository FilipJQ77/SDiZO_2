#include "KruskalHelper.h"

int KruskalHelper::kruskalFindSet(int *parent, int x) {
    if (parent[x] != x)
        parent[x] = kruskalFindSet(parent, parent[x]);
    return parent[x];
}