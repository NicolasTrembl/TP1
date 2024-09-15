#include "sommet.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

sommet* initialize(char* name, int num) {
    sommet* s = malloc(sizeof(sommet));
    s->name = malloc(sizeof (char) * (strlen(name)));
    name[strlen(name)-1] = '\0';
    strcpy(s->name, name);
    s->num = num;
    return s;
}

void free_sommet(sommet* sommet) {
    free(sommet->name);
    free(sommet);
}
