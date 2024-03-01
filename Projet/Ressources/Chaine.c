#include "Chaine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void insererPoint(CellPoint *lp , double x , double y){
    CellPoint *np = malloc(sizeof(CellPoint));
    np->x = x;
    np->y = y;
    np->suiv = lp;
    lp = np;
} 

void insererChaine(CellChaine *lc ,CellPoint *lp){
    CellChaine *nc = malloc(sizeof(CellChaine));
    nc->points = lp;
    nc->suiv = lc;
    lc = nc;
}




Chaines* lectureChaines(FILE *f){
    if (f==NULL){
        printf("Erreur a l'ouverture du fichier txt\n");
        exit(1);
    }
    Chaines* c=malloc(sizeof(Chaines));
    int gamma,nbChaine;
    fscanf(f,"NbChain : %d\n",&nbChaine);
    fscanf(f,"Gamma: %d\n",&gamma);
    c->gamma = gamma;
    c->nbChaines = nbChaine;

    CellChaine *lc = malloc(sizeof(CellChaine));
    for(int i = 0 ; i<c->nbChaines ; i++){
        int numero,nbP;
        fscanf(f,"%d %d",&numero,&nbP);
        CellPoint* cp = malloc(sizeof(CellPoint));
        for(int j = 0 ; j < nbP ; j++){
            double x,y;
            fscanf(f,"%lf %lf",&x,&y);
            insererPoint(cp,x,y);
        }
        int x;
        lc->numero = numero;
        insererChaine(lc,cp);
    }
    c->chaines = lc;
    return c;
}

void afficheChaines(Chaines *C) {
    if (C == NULL) {
        printf("La liste de chaines est vide.\n");
        return;
    }

    printf("gamma : %d\n", C->gamma);
    printf("Nbchaines : %d\n", C->nbChaines);

    CellChaine *chaineCourante = C->chaines;

    while (chaineCourante != NULL) {
        printf("\nChaine numero %d:\n", chaineCourante->numero);

        CellPoint *pointCourant = chaineCourante->points;

        while (pointCourant != NULL) {
            printf("Point (%lf, %lf)\n", pointCourant->x, pointCourant->y);
            pointCourant = pointCourant->suiv;
        }

        chaineCourante = chaineCourante->suiv;
    }
}

int main(){
    FILE* f=fopen("00014_burma.cha","r");
    Chaines* c=lectureChaines(f);
    afficheChaines(c);
}