#include "../FichiersH/Hachage.h"

//                      Exercice 4 – Deuxieme methode : stockage par table de hachage (TME7)

// Exo 4.2
int clef(double x, double y) {
    return y + (x + y) * (x + y + 1) / 2;
}

// Exo 4.3
int fonctionHachage(int k, int M) {
    double A = (sqrt(5) - 1) / 2;
    return (int)(M * ((A * k) - (int)(A * k)));
}

// Exo 4.4
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) {
    // Calcule la clé pour les coordonnées (x, y)
    int k = clef(x,y);
    // Calcule l'indice correspondant dans la table de hachage
    int indice = fonctionHachage(k, H->tailleMax);

    // Parcourt la liste chainée à l'indice correspondant dans la table de hachage
    CellNoeud *temp = H->T[indice];
    while (temp != NULL) {
        // Vérifie si le noeud existe déjà aux coordonnées (x, y)
        if (temp->nd->x == x && temp->nd->y == y) {
            // Si oui, retourne ce noeud
            return temp->nd;
        }
        // Passe au noeud suivant dans la liste chainée
        temp = temp->suiv;
    }

    // Si le noeud n'existe pas, crée un nouveau noeud
    Noeud *noeud = (Noeud*)malloc(sizeof(Noeud));
    noeud->x = x;
    noeud->y = y;
    noeud->num = R->nbNoeuds + 1;
    noeud->voisins = NULL;

    // Ajoute le nouveau noeud à la table de hachage
    CellNoeud *CNH = (CellNoeud*)malloc(sizeof(CellNoeud));
    CNH->nd = noeud;
    CNH->suiv = H->T[indice];
    H->T[indice] = CNH;

    // Ajoute le nouveau noeud au réseau
    CellNoeud *CNR = (CellNoeud*)malloc(sizeof(CellNoeud));
    CNR->nd = noeud;
    CNR->suiv = R->noeuds;
    R->noeuds = CNR;
    R->nbNoeuds++;

    return noeud;
}

// Exo 4.5
Reseau* reconstitueReseauHachage(Chaines *C, int M) {
    // Allocation de mémoire pour la structure Reseau
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    // Initialisation des attributs du réseau
    R->gamma = C->gamma;
    R->nbNoeuds = 0;
    R->noeuds = NULL;
    R->commodites = NULL;

    // Allocation de mémoire pour la table de hachage
    TableHachage *H = (TableHachage*)malloc(sizeof(TableHachage));
    // Initialisation de la taille maximale de la table de hachage
    H->tailleMax = M;
    // Allocation de mémoire pour les listes chainées de la table de hachage
    H->T = (CellNoeud**)malloc(M * sizeof(CellNoeud*));
    // Initialisation de chaque liste chainée à NULL
    for (int i = 0; i < M; i++) {
        H->T[i] = NULL;
    }

    // Parcourt chaque chaine dans la structure Chaines
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        // Parcourt chaque point dans la chaine actuelle
        CellPoint *point = chaine->points;
        Noeud *precedent_noeud = NULL;
        Noeud *depart_noeud = NULL;
        while (point != NULL) {
            // Recherche ou création d'un nouveau noeud pour le point actuel
            Noeud *nouveau_noeud = rechercheCreeNoeudHachage(R,H,point->x,point->y);
            // Ajout de liaisons entre le nouveau noeud et le précédent noeud
            if (precedent_noeud != NULL) {
                if (!liaisonExiste(precedent_noeud, nouveau_noeud)) {
                    ajouterVoisin(precedent_noeud, nouveau_noeud);
                    ajouterVoisin(nouveau_noeud, precedent_noeud);
                }
            }
            // Stocke le premier noeud de la chaine comme départ pour la commodité
            if (depart_noeud == NULL)
                depart_noeud = nouveau_noeud;
            precedent_noeud = nouveau_noeud;
            point = point->suiv;
        }
        // Ajoute une commodité entre le premier et le dernier noeud de la chaine
        ajouterCommodite(depart_noeud, precedent_noeud, R);
        chaine = chaine->suiv;
    }

    // Libère la mémoire allouée pour les listes chainées de la table de hachage
    for (int i = 0; i < M; i++) {
        CellNoeud *cell = H->T[i];
        while (cell != NULL) {
            CellNoeud *temp = cell;
            cell = cell->suiv;
            free(temp);
        }
    }
    // Libère la mémoire allouée pour le tableau de listes chainées de la table de hachage et pour la table de hachage elle-même
    free(H->T);
    free(H);
    
    return R;
}

