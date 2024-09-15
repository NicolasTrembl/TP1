#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "sommet.h"

int main(void) {

    graphe* g = malloc(sizeof(graphe));
    
    read_txt("../data.txt", g);

    show_influence(g);

    free_graphe(g);

    return 0;
}
