#include "../FichiersH/Chaine.h"
//                                Lecture, stockage et affichage des donnees
//                      Exercice 1 – Manipulation d’une instance de “Liste de Chaines” (TME5)

// Exo 1.1
Chaines* lectureChaines(FILE *f) {
    Chaines *C = (Chaines *)malloc(sizeof(Chaines));
    // Lit le nombre de chaines et gamma à partir du fichier
    fscanf(f, "NbChain: %d\n", &(C->nbChaines));
    fscanf(f, "Gamma: %d\n", &(C->gamma));
    C->chaines = NULL;

    // Boucle pour lire chaque chaine à partir du fichier
    for (int i=0; i<C->nbChaines;i++) {
        CellChaine *temp1 = C->chaines;
        CellChaine *chaine = malloc(sizeof(CellChaine));
        // Connecte la nouvelle cellule à la liste des chaines existantes
        chaine->suiv = temp1;

        // Lit le numéro de la chaine et le nombre de points de cette chaine
        int nbpoints;
        fscanf(f, "%d %d", &(chaine->numero), &nbpoints);
        chaine->points = NULL;
        chaine->numero = i;

        // Boucle pour lire chaque point de la chaine
        for (int j=0; j<nbpoints;j++) {
            CellPoint *temp = chaine->points;
            CellPoint *point = malloc(sizeof(CellPoint));
            // Connecte la nouvelle cellule à la liste des points existante
            point->suiv = temp;

            // Lit les coordonnées (x, y)  à partir du fichier
            fscanf(f, "%lf %lf", &(point->x), &(point->y));
            // Met à jour la liste des points de la chaine avec le nouveau point
            chaine->points = point;
        }
        // Met à jour la liste des chaines avec la nouvelle chaine
        C->chaines = chaine;
    }
    return C;
}

// Exo 1.2
void ecrireChaines(Chaines *C, FILE *f) {
    // Vérifie si les paramètres sont valides
    if (C == NULL || f == NULL) {
        fprintf(stderr, "Erreur : Paramètres invalides.\n");
        return;
    }
    
    // Ecrit le nombre de chaines et le paramètre gamma dans le fichier
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);

    // Parcourt chaque chaine dans la liste des chaines
    CellChaine *ChaineCourante = C->chaines;
    while (ChaineCourante) {
        // Ecrit le numéro de la chaine et le paramètre gamma correspondant dans le fichier
        fprintf(f, "%d %d", ChaineCourante->numero, C->gamma);

        // Parcourt chaque point dans la chaine actuelle
        CellPoint *PointCourant = ChaineCourante->points;
        while (PointCourant) {
            // Ecrit les coordonnées (x, y) du point dans le fichier
            fprintf(f, " %.2lf %.2lf", PointCourant->x, PointCourant->y);
            PointCourant = PointCourant->suiv;
        }
        // Passe à la ligne suivante dans le fichier pour la prochaine chaine
        fprintf(f, "\n");
        ChaineCourante = ChaineCourante->suiv;
    }
}

// Exo 1.3
void afficheChainesSVG(Chaines *C, char* nomInstance){
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

// Exo 1.4
double longueurChaine(CellChaine *c){
    double res = 0.0;
    CellPoint *temp = c->points;
    // Parcourt chaque point de la chaine
    while (temp != NULL && temp->suiv != NULL) {
        // Ajoute la distance entre le point actuel et le point suivant à la longueur totale
        res += sqrt(pow((temp->suiv)->x - temp->x, 2) + pow((temp->suiv)->y - temp->y, 2));
        // Passe au point suivant dans la chaine
        temp = temp->suiv;
    }
    return res;
}

double longueurTotale(Chaines *C) {
    double res = 0.0;
    CellChaine *temp = C->chaines;
    // Parcourt chaque chaine dans la structure Chaines
    while (temp != NULL) {
        // Ajoute la longueur de la chaine actuelle à la longueur totale
        res += longueurChaine(temp);
        // Passe à la chaine suivante dans la structure Chaines
        temp = temp->suiv;
    }
    return res;
}


// Exo 1.5
int comptePointsTotal(Chaines *C) {
    int res = 0;
    CellChaine *chaine = C->chaines;
    // Parcourt chaque chaine dans la structure Chaines
    while (chaine != NULL) {
        CellPoint *point = chaine->points;
        // Parcourt chaque point dans la chaine actuelle
        while (point != NULL) {
            res++;
            // Passe au point suivant dans la chaine
            point = point->suiv; 
        }
        // Passe à la chaine suivante dans la structure Chaines
        chaine = chaine->suiv;
    }
    return res;
}


//                      Fonctions supp

void afficheChaines(Chaines *C) {
    if (C == NULL) {
        printf("La liste de chaines est vide.\n");
        return;
    }
    printf("Nbchaines : %d\n", C->nbChaines);
    printf("Gamma : %d\n", C->gamma);

    CellChaine *chaineCourante = C->chaines;

    while (chaineCourante != NULL) {
        printf("\nChaine numero %d:\n", chaineCourante->numero);

        CellPoint *pointCourantant = chaineCourante->points;
        int i=1;
        while (pointCourantant != NULL) {
            printf("Point %d (%.2lf, %.2lf)\n",i, pointCourantant->x, pointCourantant->y);
            pointCourantant = pointCourantant->suiv;
            i++;
        }

        chaineCourante = chaineCourante->suiv;
    }
}

void libererChaine(CellChaine *chaine) {
    CellPoint *PointCourant = chaine->points;
    while (PointCourant != NULL) {
        CellPoint *nextPoint = PointCourant->suiv;
        free(PointCourant);
        PointCourant = nextPoint;
    }
    free(chaine);
}

void libererChaines(Chaines *C) {
    CellChaine *ChaineCourante = C->chaines;
    while (ChaineCourante != NULL) {
        CellChaine *nextChaine = ChaineCourante->suiv;
        libererChaine(ChaineCourante);
        ChaineCourante = nextChaine;
    }
    free(C);
}