#include "FichiersH/Graph.h"

int main(){
    FILE* f1 = fopen("00014_burma.cha","r");
    Chaines *chaines = lectureChaines(f1);
    fclose(f1);
    Reseau* R = reconstitueReseauListe(chaines);
    Graphe* g = creerGraphe(R);

    afficher_Graphe(g);
    printf("\n");
    printf("%d \n",plusPetitNombreAretes(g,3,6));

    libererGraphe(g);
    libererChaines(chaines);
    libererReseau(R);
}

