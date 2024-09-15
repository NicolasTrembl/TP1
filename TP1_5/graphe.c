#include "graphe.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialize_graphe(int ordre, graphe* graphe) {
    graphe->ordre = ordre;

    graphe->matrice = malloc(ordre * sizeof(int*));
    for (int i = 0; i < ordre; i++) {
        graphe->matrice[i] = calloc(ordre, sizeof(int));
    }
    graphe->sommets = malloc(ordre * sizeof(sommet*));
}

// from https://stackoverflow.com/a/1068937
int numPlaces (int n) {
    if (n == 0) return 1;
    return floor (log10 (abs (n))) + 1;
}

void read_txt(char* filename, graphe* graphe) {
    FILE *fptr;

    int ordre = 0;
    fptr = fopen(filename, "r");

    if(fptr == NULL) {
        printf("Not able to open the file.");
    }

    fscanf(fptr, "%d ", &ordre);

    initialize_graphe(ordre, graphe);


    char text[99];

    for (int i = 0; i < ordre;) {

        fgets(text, 99, fptr);
        int value;
        sscanf(text, "%d", &value);
        char* name = &text[numPlaces(value) + 1];
        graphe->sommets[i] = initialize(name, value);
        fgets(text, 99, fptr);
        for (int j = 0; j < ordre; j++) {
            graphe->matrice[i][j] = (text[j+1] == '1');
        }
        i++;
    }
}

void free_graphe(graphe* graphe) {
    for (int i = 0; i < graphe->ordre; i++) {
        free_sommet(graphe->sommets[i]);
        graphe->sommets[i] = NULL;
        free(graphe->matrice[i]);
        graphe->matrice[i] = NULL;
    }
    free(graphe->sommets);
    graphe->sommets = NULL;
    free(graphe->matrice);
    graphe->matrice = NULL;
    free(graphe);

}


void show_influence(graphe* graphe) {
    printf("Influence:\n");
    for (int i = 0; i < graphe->ordre; i++) {
        printf("%s, influence :\n> ", graphe->sommets[i]->name);
        char inf = 0;
        for (int j = 0; j < graphe->ordre; j++) {
            if (graphe->matrice[i][j]) {
                inf = 1;
                printf("%s ", graphe->sommets[j]->name);
            }
        }
        if (!inf) printf("<Personne>");
        printf("\n");
    }
}
