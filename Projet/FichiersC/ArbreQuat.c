#include "../FichiersH/ArbreQuat.h"

//                      Exercice 5 – Troisieme methode : stockage par arbre quaternaire (TME8)

// Exo 5.1
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) {
    // Vérifie si la structure Chaines est valide et si elle contient des chaines
    if (!C || !C->chaines) return;
    // Initialise les coordonnées minimales à l'infini positif et les coordonnées maximales à l'infini négatif
    *xmin = INFINITY; *ymin = INFINITY; 
    *xmax = -INFINITY; *ymax = -INFINITY; 

    // Parcourt chaque chaine dans la structure Chaines
    CellChaine *cc = C->chaines; 
    while (cc) {
        // Parcourt chaque point dans la chaine actuelle
        CellPoint *point = cc->points;
        while (point != NULL) {
            // Récupère les coordonnées du point actuel
            double x = point->x; 
            double y = point->y; 
            // Met à jour les coordonnées minimales et maximales si nécessaire
            if (x < *xmin) *xmin = x; 
            if (y < *ymin) *ymin = y; 
            if (x > *xmax) *xmax = x; 
            if (y > *ymax) *ymax = y; 
            // Passe au point suivant dans la chaine
            point = point->suiv; 
        } 
        // Passe à la chaine suivante dans la structure Chaines
        cc = cc->suiv; 
    }
}


// Exo 5.2
ArbreQuat *creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat *aq = (ArbreQuat*) malloc(sizeof(ArbreQuat)); 
    aq->xc = xc; 
    aq->yc = yc; 
    aq->coteX = coteX; 
    aq->coteY = coteY;

    aq->noeud = NULL; 
    aq->no = NULL; 
    aq->ne = NULL;
    aq->so = NULL; 
    aq->se = NULL; 
    return aq; 
}

// Exo 5.3
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent) {
    if (*a == NULL) { 
        double x = n->x, y = n->y; 
        double xc = parent->xc, yc = parent->yc; 
        double coteX = parent->coteX, coteY = parent->coteY; 
        ArbreQuat *aq; 
        /* NO */  
        if (x < xc && y > yc) {
            aq = creerArbreQuat(xc-coteX/4, yc+coteY/4, coteX/2, coteY/2); 
            parent->no = aq; 
        }
        /* NE */ 
        else if (x >= xc && y >= yc) {
            aq = creerArbreQuat(xc+coteX/4, yc+coteY/4, coteX/2, coteY/2); 
            parent->ne = aq;  
        }
        /* SE */ 
        else if (x >= xc && y < yc) {
            aq = creerArbreQuat(xc+coteX/4, yc-coteY/4, coteX/2, coteY/2); 
            parent->se = aq; 
        }
        /* SO */ 
        else { 
            aq = creerArbreQuat(xc-coteX/4, yc-coteY/4, coteX/2, coteY/2); 
            parent->so = aq; 
        }
        aq->noeud = n; 
        *a = aq; 
    }
    else if ((*a)->noeud != NULL) {
        Noeud *noeud = (*a)->noeud; 
        (*a)->noeud = NULL;
        insererNoeudArbre(n, a, parent); 
        insererNoeudArbre(noeud, a, parent); 
    }
    else {
        double x = n->x, y = n->y; 
        double xc = (*a)->xc, yc = (*a)->yc; 
        if (x < xc && y > yc) 
            insererNoeudArbre(n, &((*a)->no), *a); /* NO */ 
        else if (x >= xc && y >= yc) 
            insererNoeudArbre(n, &((*a)->ne), *a); /* NE */  
        else if (x >= xc && y < yc) 
            insererNoeudArbre(n, &((*a)->se), *a); /* SE */  
        else 
            insererNoeudArbre(n, &((*a)->so), *a); /* SO */ 
    }
}

// Exo 5.4
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat* a, ArbreQuat* parent, double x, double y) {
    // Si l'arbre est vide
    if (a == NULL) {
        // Allocation de mémoire pour un nouveau noeud
        Noeud *nd = (Noeud*) malloc(sizeof(Noeud));
        // Initialisation des attributs du nouveau noeud
        nd->x = x;
        nd->y = y;
        nd->num = R->nbNoeuds + 1;
        nd->voisins = NULL;

        // Ajoute le nouveau noeud au réseau
        CellNoeud *cnd = (CellNoeud*) malloc(sizeof(CellNoeud));
        cnd->nd = nd;
        cnd->suiv = R->noeuds;
        R->noeuds = cnd;
        R->nbNoeuds++; 

        // Insère le nouveau noeud dans l'arbre
        insererNoeudArbre(nd, &a, parent);
        return nd; 
    }
    // Si l'arbre contient un noeud
    else if (a->noeud != NULL) {
        // Récupère le noeud existant dans le noeud actuel de l'arbre
        Noeud *n = a->noeud;
        // Si les coordonnées du noeud existant correspondent à x et y, retourne ce noeud
        if (compareNoeud_xy(n, x, y))
            return n;  
        // Sinon, crée un nouveau noeud
        else {
            // Allocation de mémoire pour un nouveau noeud
            Noeud *nd = (Noeud*) malloc(sizeof(Noeud));
            // Initialisation des attributs du nouveau noeud
            nd->x = x;
            nd->y = y;
            nd->num = R->nbNoeuds + 1;
            nd->voisins = NULL;

            // Ajoute le nouveau noeud au réseau
            CellNoeud *cnd = (CellNoeud*) malloc(sizeof(CellNoeud));
            cnd->nd = nd;
            cnd->suiv = R->noeuds;
            R->noeuds = cnd;
            R->nbNoeuds++; 

            // Insère le nouveau noeud dans l'arbre
            insererNoeudArbre(nd, &a, parent); 
            // Retourne le nouveau noeud
            return nd; 
        }
    }
    // Si l'arbre contient un sous-arbre
    else {
        // Récupère les coordonnées du centre du sous-arbre
        double xc = a->xc, yc = a->yc; 
        // Compare les coordonnées du point avec celles du centre du sous-arbre pour déterminer le quadrant
        if (x < xc && y > yc) 
            return rechercheCreeNoeudArbre(R, a->no, a, x, y); /* NO */ 
        else if (x >= xc && y >= yc) 
            return rechercheCreeNoeudArbre(R, a->ne, a, x, y); /* NE */  
        else if (x >= xc && y < yc) 
            return rechercheCreeNoeudArbre(R, a->se, a, x, y); /* SE */  
        else 
            return rechercheCreeNoeudArbre(R, a->so, a, x, y); /* SO */ 
    }
}


// Exo 5.5
Reseau* reconstitueReseauArbre(Chaines* C) {
    // Allocation de mémoire pour la structure Reseau
    Reseau *R = (Reseau*) malloc(sizeof(Reseau));
    // Initialisation des attributs du réseau
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    // Détermination des coordonnées minimales et maximales de toutes les chaines
    double xmin, ymin, xmax, ymax; 
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    // Calcul des coordonnées du centre et des côtés du cadre 
    double xc = (xmax + xmin) / 2, yc = (ymax + ymin) / 2;  
    double coteX = (xmax - xmin), coteY = (ymax - ymin); 
    // Création de l'arbre quadtree avec le cadre englobant
    ArbreQuat *arbre = creerArbreQuat(xc, yc, coteX, coteY);

    // Parcourt chaque chaine dans la structure Chaines
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        // Parcourt chaque point dans la chaine actuelle
        CellPoint *point = chaine->points;
        Noeud *precedent_noeud = NULL;
        Noeud *depart_noeud = NULL;
        while (point != NULL) {
            // Recherche ou création d'un nouveau noeud dans l'arbre quadtree
            Noeud *nouveau_noeud = rechercheCreeNoeudArbre(R, arbre, NULL, point->x, point->y);
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
    //libererArbreQuat(arbre);
    return R;
}



//                      Fonctions supp

void libererArbreQuat(ArbreQuat *arbre) {
    if (arbre == NULL)
        return;
    libererArbreQuat(arbre->so);
    libererArbreQuat(arbre->se);
    libererArbreQuat(arbre->no);
    libererArbreQuat(arbre->ne);
    if (arbre->noeud != NULL)
        free(arbre->noeud);

    free(arbre);
}