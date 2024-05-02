#include "FichiersH/ArbreQuat.h"
#include <assert.h>
//                          Reconstitution du reseau
//                      Exercice 2 – Premiere methode : stockage par liste chainee (TME5-TME6)

// Exo 2.3

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier.cha>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int methode;
    do {
    menu2();
    // Lire le fichier .cha et initialiser la structure Chaines
    FILE *fi = fopen(argv[1], "r");
    assert(fi); 
	Chaines *chaines = lectureChaines(fi);
	fclose(fi);
    scanf ("%d" ,&methode);
    // Appeler la fonction reconstitueReseauListe ou toute autre fonction requise selon la méthode spécifiée
    switch (methode) {
        case 1: // Liste
            Reseau *rl = reconstitueReseauListe(chaines);
            FILE *fol = fopen("Resultats/newReseau.res", "w"); 
            assert(fol);
            ecrireReseau(rl, fol);
            afficheReseauSVG(rl, "Resultats/Reseau");
            fclose(fol);
            /* tests */ 
            printf("Nb liaisons   : %d\n", nbLiaisons(rl)); 
            printf("Nb commodites : %d\n", nbCommodites(rl)); 
            libererReseau(rl); 
            break;
        case 2: // Table de hachage
            int M; 
            printf("Taille de la table de hachage: "); 
            scanf(" %d", &M); 
            Reseau *rh = reconstitueReseauHachage(chaines, M); 
            FILE *foh = fopen("Resultats/newHachage.res", "w"); 
            assert(foh);
            ecrireReseau(rh, foh);
            afficheReseauSVG(rh, "Resultats/Hachage");
            fclose(foh);
            /* tests */ 
            printf("Nb liaisons   : %d\n", nbLiaisons(rh)); 
            printf("Nb commodites : %d\n", nbCommodites(rh)); 
            libererReseau(rh);
            break;
        case 3: // Arbre
            Reseau *ra = reconstitueReseauArbre(chaines);
            FILE *foa = fopen("Resultats/newArbreQuat.res", "w"); 
            assert(foa);
            ecrireReseau(ra, foa);
            afficheReseauSVG(ra, "Resultats/ArbreQuat");
            fclose(foa);
            /* tests */ 
            printf("Nb liaisons   : %d\n", nbLiaisons(ra)); 
            printf("Nb commodites : %d\n", nbCommodites(ra)); 
            libererReseau(ra);
            break;
        default:
            fprintf(stderr, "Méthode non reconnue.\n");
            exit(EXIT_FAILURE);
    }
    }while (methode != 0);
    clear_screen();
    printf("Merci, et au revoir.\n");
    return 0;
}

//          Fonctions supp
void clear_screen() {
    printf("\033[H\033[2J");
    fflush(stdout);
}

void menu2(){
    printf("\n~~~~~~~~~~~~~~~~Veuillez choisir une des options proposées~~~~~~~~~~~~~~~~\n");
    printf("\n0 : Quitter le programme\n");
    printf("\n1 : Methode LC\n");
    printf("\n2 : Methode H\n");
    printf("\n3 : Methode Arbre\n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}