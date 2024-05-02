#include "../FichiersH/Reseau.h"
//                          Reconstitution du reseau
//                      Exercice 2 – Premiere methode : stockage par liste chainee (TME5-TME6)

// Exo 2.1
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    CellNoeud *temp = R->noeuds;
    while (temp != NULL) {
        if (temp->nd->x == x && temp->nd->y == y) {
            return temp->nd; // Noeud trouvé
        }
        temp = temp->suiv;
    }

    // Créer un nouveau noeud
    Noeud *N = malloc(sizeof(Noeud));
    N->num = R->nbNoeuds + 1;
    N->x = x;
    N->y = y;
    N->voisins = NULL;

    // Mettre à jour la liste des noeuds du réseau
    CellNoeud *CN = malloc(sizeof(CellNoeud));
    CN->nd = N;
    CN->suiv = R->noeuds;
    R->noeuds = CN;

    // Mettre à jour le nombre de noeuds dans le réseau
    R->nbNoeuds++;
    return N;
}

// Exo 2.2
Reseau* reconstitueReseauListe(Chaines *C) {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    
    R->gamma = C->gamma;
    R->nbNoeuds = 0;
    R->noeuds = NULL;
    R->commodites = NULL;

    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        CellPoint *point = chaine->points;
        Noeud *precedent_noeud = NULL;
        Noeud *depart_noeud = NULL;
        while (point != NULL) {
            Noeud *nouveau_noeud = rechercheCreeNoeudListe(R,point->x,point->y);
            if (precedent_noeud != NULL) {
                // Vérifier si la liaison entre les nœuds existe déjà
                if (!liaisonExiste(precedent_noeud, nouveau_noeud)) {
                    // Ajouter la liaison entre les nœuds dans un sens
                    ajouterVoisin(precedent_noeud, nouveau_noeud);
                    // Ajouter la liaison entre les nœuds dans l'autre sens
                    ajouterVoisin(nouveau_noeud, precedent_noeud);
                }
            }
            if (depart_noeud == NULL)
                depart_noeud = nouveau_noeud;
            precedent_noeud = nouveau_noeud;
            point = point->suiv;
        }
        // Ajouter les commodités de la chaîne
        ajouterCommodite(depart_noeud, precedent_noeud, R);
        chaine = chaine->suiv;
    }
    return R;
}

//                      Exercice 3 – Manipulation d’un reseau (TME6)

// Exo 3.1
int nbCommodites(Reseau *R){
    int res = 0;
    CellCommodite *commoditeCourant = R->commodites;
    // Parcourt chaque commodité dans la liste des commodités du réseau
    while(commoditeCourant){
        res++;
        // Passe à la commodité suivante dans la liste
        commoditeCourant = commoditeCourant->suiv;
    }
    return res;
}

int nbLiaisons(Reseau *R){
    int res = 0;
    CellNoeud *cnc = R->noeuds;

    // Parcourt chaque noeud dans la liste des noeuds du réseau
    while(cnc){
        Noeud *nd = cnc->nd;
        CellNoeud *voisin = nd->voisins;
        // Parcourt chaque voisin du noeud actuel
        while (voisin != NULL) {
            // Vérifie que la liaison n'a pas déjà été comptée (pour éviter de compter deux fois la même liaison)
            if (nd->num < voisin->nd->num) {
                res++;
            }
            // Passe au voisin suivant dans la liste des voisins
            voisin = voisin->suiv;
        }
        // Passe au noeud suivant dans la liste des noeuds du réseau
        cnc = cnc->suiv;
    }
    return res;
}

// Exo 3.2
void ecrireReseau(Reseau *R, FILE *f){
    fprintf(f,"nbNoeuds: %d\n",R->nbNoeuds);
    fprintf(f,"NbLiaisons: %d\n",nbLiaisons(R));
    fprintf(f,"NbCommodites: %d\n",nbCommodites(R));
    fprintf(f,"Gamma: %d\n",R->gamma);
    
    // Saut de ligne pour séparer les sections
    fprintf(f,"\n");
    
    // Parcourt chaque noeud dans la liste des noeuds du réseau
    CellNoeud *ln = R->noeuds;
    while(ln){
        // Récupère le noeud actuel
        Noeud *n = ln->nd;
        // Écrit les informations sur le noeud dans le fichier
        fprintf(f,"v %d %lf %lf\n",n->num,n->x,n->y);
        // Passe au noeud suivant dans la liste
        ln = ln->suiv;
    }
    
    // Saut de ligne pour séparer les sections
    fprintf(f,"\n");

    // Parcourt chaque noeud dans la liste des noeuds du réseau
    ln = R->noeuds;
    while(ln){
        // Récupère le noeud actuel
        Noeud *n1 = ln->nd;
        // Parcourt chaque voisin du noeud actuel
        CellNoeud *nd1 = n1->voisins;
        while(nd1){
            // Récupère le voisin actuel
            Noeud *n2 = nd1->nd;
            // Vérifie que la liaison n'a pas déjà été écrite (pour éviter les doublons)
            if(n2->num < n1->num)
                // Écrit les informations sur la liaison dans le fichier
                fprintf(f,"l %d %d\n",n2->num,n1->num);
            // Passe au voisin suivant dans la liste
            nd1 = nd1->suiv;
        }
        // Passe au noeud suivant dans la liste
        ln=ln->suiv;
    }

    // Saut de ligne pour séparer les sections
    fprintf(f,"\n");

    // Parcourt chaque commodité dans la liste des commodités du réseau
    CellCommodite *lc = R->commodites;
    while(lc){
        // Écrit les informations sur la commodité dans le fichier
        fprintf(f,"k %d %d\n",lc->extrA->num,lc->extrB->num);
        // Passe à la commodité suivante dans la liste
        lc = lc->suiv;
    }
}

// Exo 3.3
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

//                      Fonctions supp

void libererNoeud(Noeud* noeud) {
    if (noeud) {
        CellNoeud *tmp; 
        CellNoeud *curr = noeud->voisins; 
        while (curr) {
            tmp = curr;
            curr = curr->suiv;
            free(tmp);  
        }
        free(noeud);
    }
}

void libererNoeuds(CellNoeud* noeuds) {
    CellNoeud *tmp; 
    CellNoeud *curr = noeuds; 
    while (curr) {
        tmp = curr;
        curr = curr->suiv;
        libererNoeud(tmp->nd); 
        free(tmp);  
    }
}

void libererCommodites(CellCommodite *coms) {
    CellCommodite *tmp; 
    CellCommodite *curr = coms; 
    while (curr) {
        tmp = curr; 
        curr = curr->suiv;  
        free(tmp);  
    }
}

void libererReseau(Reseau *res) {
    if (res) {
        libererNoeuds(res->noeuds); 
        libererCommodites(res->commodites); 
        free(res);
    }
}

void ajouterVoisin(Noeud *n1, Noeud *n2) {
    // Alloue de la mémoire pour une nouvelle cellule de noeud
    CellNoeud *nouveau = (CellNoeud*)malloc(sizeof(CellNoeud));
    // Assigner le noeud voisin à la nouvelle cellule
    nouveau->nd = n2;
    // Insère la nouvelle cellule au début de la liste des voisins du noeud n1
    nouveau->suiv = n1->voisins;
    n1->voisins = nouveau;
}

int liaisonExiste(Noeud *n1, Noeud *n2) {
    CellNoeud *voisin = n1->voisins;
    while (voisin != NULL) {
        if (voisin->nd == n2)
            return 1; // La liaison existe déjà
        voisin = voisin->suiv;
    }
    return 0; // La liaison n'existe pas
}

void ajouterCommodite(Noeud *a, Noeud *b, Reseau *R) {
    // Alloue de la mémoire pour une nouvelle cellule de commodité
    CellCommodite *nouvelle_commodite = (CellCommodite*)malloc(sizeof(CellCommodite));
    // Assigner les deux extrémités de la commodité à la nouvelle cellule
    nouvelle_commodite->extrA = a;
    nouvelle_commodite->extrB = b;
    // Insère la nouvelle cellule au début de la liste des commodités du réseau
    nouvelle_commodite->suiv = R->commodites;
    R->commodites = nouvelle_commodite;
}

int compareNoeud_xy(Noeud *nd, double x, double y) {
	return nd->x == x && nd->y == y;
}