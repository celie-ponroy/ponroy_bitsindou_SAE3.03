// Authors: Célie Ponroy et Maëlle Bitsinodu

//insertion des bibliothèques
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

//Constantes
#define MAXLI 2048 //taille maximale de la ligne de commande
//#define MAXARG : taille maximale du nombres d'arguments

char cmd[MAXLI];
char path[MAXLI];
int pathidx;
void mbash();

//fonction qui permet de lire le fichier .mbashrc
int main(int argc, char** argv) { // argc: nb d'arguments, argv: tableau de chaine de caractères
  while (1) { 
    printf("Commande: ");
    fgets(cmd, MAXLI, stdin); 
    mbash(cmd);
  }
  return 0;
}

void mbash() { // fonction qui permet d'executer une commande
  printf("Execute: %s", cmd);
  system(cmd);
}
