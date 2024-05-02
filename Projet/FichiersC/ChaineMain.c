#include "../FichiersH/Chaine.h"

//                                Lecture, stockage et affichage des donnees

// Exo 1.2 (./ChaineMain FichierRead FichierWrite)
int main(int argc , char** argv){
    FILE* f1=fopen(argv[1],"r");
    FILE* f2=fopen(argv[2],"w");
    Chaines* c=lectureChaines(f1);
    afficheChaines(c);
    ecrireChaines(c,f2);

    afficheChainesSVG(c,"Reseaux");

    printf("La longueur totale de la chaine: %.2lf \n",longueurTotale(c));
    printf("Le nombre total de points : %d \n",comptePointsTotal(c));

    libererChaines(c);
    fclose(f2);
    fclose(f1);
}