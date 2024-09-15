#ifndef GRAPHE_H
#define GRAPHE_H
#include "sommet.h"

typedef struct Graphe {
    int** matrice;
    int ordre;
    sommet** sommets;
} graphe;

void initialize_graphe(int ordre, graphe* graphe);
void read_txt(char* filename, graphe* graphe);
void free_graphe(graphe* graphe);
void show_influence(graphe* graphe);

#endif //GRAPHE_H
