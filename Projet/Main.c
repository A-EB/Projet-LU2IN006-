#include "FichiersH/ArbreQuat.h"

//                      Exercice 6 – Comparaison des trois structures (TME9)

// Exo 6.1
void tester_instances1(char* nom_fichier, int taille_table_hachage) {
    FILE *fichier_resultats;
    fichier_resultats = fopen("Resultats/resultats_temps.txt", "w");

    if (fichier_resultats == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        exit(1);
    }

    FILE* f1 = fopen(nom_fichier,"r");
    Chaines *chaines = lectureChaines(f1);
    fclose(f1);
    srand(time(NULL));

    clock_t debut, fin;
    debut = clock();
    Reseau* RA = reconstitueReseauArbre(chaines);
    fin = clock();
    double temps_arbre = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    debut = clock();
    Reseau* RL = reconstitueReseauListe(chaines);
    fin = clock();
    double temps_liste = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    debut = clock();
    Reseau* RH = reconstitueReseauHachage(chaines,taille_table_hachage);
    fin = clock();
    double temps_hachage =((double) (fin - debut)) / CLOCKS_PER_SEC;

    fprintf(fichier_resultats, "Temps d'execution reconstitueReseauArbre : %.6f secondes\n", temps_arbre);
    fprintf(fichier_resultats, "Temps d'execution reconstitueReseauListe : %.6f secondes\n", temps_liste);
    fprintf(fichier_resultats, "Temps d'execution reconstitueReseauHachage (taille %d) : %.6f secondes\n", taille_table_hachage, temps_hachage);
    fprintf(fichier_resultats, "\n");

    fclose(fichier_resultats);
    libererChaines(chaines);
    libererReseau(RA);
    libererReseau(RL);
    libererReseau(RH);

}

// Exo 6.2
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    Chaines *C = malloc(sizeof(Chaines));
    C->nbChaines = nbChaines;
    C->gamma = 3; 
    C->chaines = NULL;

    srand(time(NULL));

    for (int i = 0; i < nbChaines; i++) {
        CellChaine *nouvelle_chaine = malloc(sizeof(CellChaine));
        nouvelle_chaine->numero = i;
        nouvelle_chaine->points = NULL;

        for (int j = 0; j < nbPointsChaine; j++) {
            double x = rand()%xmax; 
            double y = rand()%ymax;

            CellPoint *nouveau_point = (CellPoint*)malloc(sizeof(CellPoint));
            nouveau_point->x = x;
            nouveau_point->y = y;
            nouveau_point->suiv = NULL;

            if (nouvelle_chaine->points == NULL) {
                nouvelle_chaine->points = nouveau_point;
            } else {
                CellPoint *temp = nouvelle_chaine->points;
                while (temp->suiv != NULL) {
                    temp = temp->suiv;
                }
                temp->suiv = nouveau_point;
            }
        }

        nouvelle_chaine->suiv = C->chaines;
        C->chaines = nouvelle_chaine;
    }

    return C;
}

// Exo 6.3
void tester_instances2(FILE* fichier_resultats_HA,FILE* fichier_resultats_LC,char* nom_fichier) {

    FILE* f1 = fopen(nom_fichier,"r");
    Chaines *chaines = lectureChaines(f1);
    fclose(f1);

    clock_t debut, fin;
    debut = clock();
    Reseau* RA = reconstitueReseauArbre(chaines);
    fin = clock();
    double temps_arbre = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    debut = clock();
    Reseau* RL = reconstitueReseauListe(chaines);
    fin = clock();
    double temps_liste = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    debut = clock();
    Reseau* RH1 = reconstitueReseauHachage(chaines,10);
    fin = clock();
    double temps_hachage1 =((double) (fin - debut)) / CLOCKS_PER_SEC;

    debut = clock();
    Reseau* RH2 = reconstitueReseauHachage(chaines,100);
    fin = clock();
    double temps_hachage2 =((double) (fin - debut)) / CLOCKS_PER_SEC;

    debut = clock();
    Reseau* RH3 = reconstitueReseauHachage(chaines,1000);
    fin = clock();
    double temps_hachage3 =((double) (fin - debut)) / CLOCKS_PER_SEC;

    fprintf(fichier_resultats_LC, "%.6f \n", temps_liste);
    fprintf(fichier_resultats_HA, "%.6f ", temps_hachage1);
    fprintf(fichier_resultats_HA, "%.6f ", temps_hachage2);
    fprintf(fichier_resultats_HA, "%.6f ", temps_hachage3);
    fprintf(fichier_resultats_HA, "%.6f \n", temps_arbre);
    libererReseau(RA);
    libererReseau(RL);
    libererReseau(RH1);
    libererReseau(RH2);
    libererReseau(RH3);
}

int main() {
       
    int rep;
    do{
        menu();
        scanf ("%d" ,&rep);
        switch (rep) {
            case 1:
                clear_screen();
                int t_table_hachage;
                char nom_fichier[100];
                printf("Entrez le nom du fichier a lire (cha) : ");
                scanf("%s", nom_fichier);
                printf("Entrez la taille de la table de hachage : ");
                scanf("%d", &t_table_hachage);
                tester_instances1(nom_fichier, t_table_hachage);
                break;
            case 2:
                clear_screen();

            FILE *resultats_HA = fopen("Resultats/resultats_temps_HA.txt", "w");
            if (resultats_HA == NULL) {
                printf("Erreur lors de l'ouverture du fichier.");
                exit(1);
            }
            FILE *resultats_LC = fopen("Resultats/resultats_temps_LC.txt", "w");
            if (resultats_LC == NULL) {
                printf("Erreur lors de l'ouverture du fichier.");
                exit(1);
            }
            
            for(int i=500;i<=5000;i+=500){
                FILE* f2=fopen("Resultats/chaine_alea.txt","w");
                Chaines* ch = generationAleatoire(i,100,5000,5000);
                ecrireChaines(ch,f2);
                fprintf(resultats_LC, "%d ",i);
                fprintf(resultats_HA, "%d ",i);
                tester_instances2(resultats_HA,resultats_LC,"Resultats/chaine_alea.txt");
                fclose(f2);
            }
            fclose(resultats_LC);
            fclose(resultats_HA);
            break;
        }
    }while (rep != 0);
}

//              Fonctions supp
void menu(){
    printf("\n~~~~~~~~~~~~~~~~Veuillez choisir une des options proposées~~~~~~~~~~~~~~~~\n");
    printf("\n0 : Quitter le programme\n");
    printf("1 : Reconstitution du Resaux burma\n");
    printf("2 : Temps de calcul generation aleatoire\n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void clear_screen() {
    printf("\033[H\033[2J");
    fflush(stdout);
}
