#include "../FichiersH/Graph.h"
//								Optimisation du reseau
//						Exercice 7 – Parcours en Lrgeur (TME10)

// Exo 7.1
Graphe *creerGraphe(Reseau *r) {
	Graphe *g = malloc(sizeof(Graphe));

	g->nbsom = r->nbNoeuds;
	g->nbcommod = nbCommodites(r);
	g->gamma = r->gamma; 
	
	/* Tableau de sommets */ 
	g->T_som = malloc((g->nbsom+1) * sizeof(Sommet*));
	for (int i = 1; i <= g->nbsom; i++) {
		g->T_som[i] = (Sommet*) malloc(sizeof(Sommet));
		g->T_som[i]->num = i; 
		g->T_som[i]->L_voisin = NULL;
	}

	CellNoeud *cn = r->noeuds;  
	while (cn != NULL) { 
        /* Ajout des sommets */ 
		int i = cn->nd->num;
		g->T_som[i]->x = cn->nd->x;
		g->T_som[i]->y = cn->nd->y;

		/* Ajout des voisins d'aretes */ 
		CellNoeud *voisins = cn->nd->voisins; 
		while (voisins != NULL) {
			int j = voisins->nd->num; 
			if (i < j)
				ajoutArete(g, i, j); 
			voisins = voisins->suiv; 
		}
		cn = cn->suiv;
	}

	/*Tableau de commodites */ 
	g->T_commod =  malloc(g->nbcommod * sizeof(Commod));
	CellCommodite *com = r->commodites; 
	int i = 0; 
	while (com != NULL) {
		g->T_commod[i++] = (Commod) {com->extrA->num, com->extrB->num};  
		com = com->suiv;
	}

	return g;
}

// Exo 7.2
int plusPetitNombreAretes(Graphe *g, int u, int v) {
    int *distance = calloc(g->nbsom + 1, sizeof(int)); // Tableau pour stocker les distances
    int *visite = calloc(g->nbsom + 1, sizeof(int));   // Tableau pour marquer les sommets visités

    S_file file;
    Init_file(&file);
    enfile(&file, u); // Ajouter le sommet de départ à L file
    visite[u] = 1;    // Marquer le sommet de départ comme visité

    while (!estFileVide(&file)) {
        int sommetCourant = defile(&file);
        if (sommetCourant == v) // Si le sommet de destination est atteint, retourner la distance
            return distance[v];
        
        // Parcourir les voisins du sommet courant
        Sommet *sommet = g->T_som[sommetCourant];
        Cellule_arete *voisin = sommet->L_voisin;
        while (voisin != NULL) {
            int voisinCourant = (voisin->a->u == sommetCourant) ? voisin->a->v : voisin->a->u;
            if (!visite[voisinCourant]) {
                visite[voisinCourant] = 1;       // Marquer le voisin comme visité
                distance[voisinCourant] = distance[sommetCourant] + 1; // Mettre à jour la distance
                enfile(&file, voisinCourant);    // Ajouter le voisin à la file
            }
            voisin = voisin->suiv;
        }
    }

    // Si le sommet de destination n'est pas atteint, retourner -1 (impossible)
    return -1;
}


//                      Fonctions supp
Arete *creerArete(int i, int j) {
	Arete *a = (Arete*) malloc(sizeof(Arete));
	a->u = i;
	a->v = j;
	return a;
}

void insererTeteListe(Cellule_arete **L, Arete *a) {
	Cellule_arete *newA = (Cellule_arete*) malloc(sizeof(Cellule_arete));
	newA->a = a;
	newA->suiv = *L;
	*L = newA;
}

void ajoutArete(Graphe *g, int i, int j) {
	Arete *a = creerArete(i, j);
	insererTeteListe(&(g->T_som[i]->L_voisin), a);
	insererTeteListe(&(g->T_som[j]->L_voisin), a);
}

void afficher_Graphe(Graphe *g) {
	printf("Nombre de sommets : %d\n", g->nbsom);
	printf("Nombre de commodites : %d\n", g->nbcommod); 
	printf("Gamma : %d\n\n", g->gamma);
	
	for (int i = 1; i <= g->nbsom; i++) {
		printf ("%d : ", i);
		afficher_ListeA(g->T_som[i]->L_voisin);
		printf("\n");
	}

    printf("\nCommodites:\n"); 
    for (int i = 0; i < g->nbcommod; i++){
		printf("%d %d\n", g->T_commod[i].e1, g->T_commod[i].e2); 
	}
    printf("\n"); 
}

void afficher_ListeA(Cellule_arete *L) { 
	while (L != NULL) {
		printf("(%d, %d) | ", L->a->u, L->a->v);
		L = L->suiv ;
	}
}

void libererListeA(Cellule_arete *L) {
    while (L != NULL) {
        Cellule_arete *temp = L;
        L = L->suiv; 
        if (temp->a->u != -1)
            temp->a->u = -1;  
        else 
            free(temp->a);
        free(temp);
    }
}

void libererGraphe(Graphe *g) {
    if (!g) 
		return; 
    for (int i = 1; i <= g->nbsom ; i++) {
        libererListeA(g->T_som[i]->L_voisin);
        free(g->T_som[i]); 
    }
    free(g->T_som); 
    free(g->T_commod); 
    free(g);  
}