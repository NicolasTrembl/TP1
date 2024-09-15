# ifndef STRUCT_H
# define STRUCT_H

typedef struct Sommet {
    char* name;
    int num;
} sommet;

sommet* initialize(char* name, int num);
void free_sommet(sommet* sommet);



# endif