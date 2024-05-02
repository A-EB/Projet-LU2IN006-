#ifndef __HACHAGE_H__
#define __HACHAGE_H__	
#include "Reseau.h"

//                      Exercice 4 – Deuxieme methode : stockage par table de hachage (TME7)

// Exo 4.1

typedef struct{
  int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

int clef(double x, double y);
int fonctionHachage(int k, int M);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);

#endif