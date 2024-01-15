#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
int pathidx;
void mbash();
void accueil();
void countnbArgs();
void execute();
char* getpremierMot(char* cmd);

int main(int argc, char** argv) {
  accueil();
  while (1) {
    printf("Commande: ");
    fgets(cmd, MAXLI, stdin);
    mbash(cmd);
  }
  return 0;
}

void mbash(char* cmd) {
  printf("Execute: %s", cmd);
  execute(cmd);
}
accueil(){
  printf("Bienvenue sur Mini bash \\(^.^)/ \n");
  printf("comment ça fonctionne:\n 1-saisir votre commande \n 2-press entrée\n");
}
void countnbArgs(){
  
}
void execute(char* cmd){
  char* premMot = getpremierMot(cmd);
  //get le premier mot
 if (strcmp(premMot, "cd") == 0) {
        printf("CDDDDD.\n");
    } else if (strcmp(premMot, "B") == 0) {
        printf("Vous avez choisi B.\n");
    } else if (strcmp(premMot, "C") == 0) {
        printf("Vous avez choisi C.\n");
    } else {
      system(cmd);
    }
  
}
char* getpremierMot(char* cmd){
  int i = 0;
  int end = 0;

  // Skip les premiers espaces si il y en a
  while(cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t'){
    i++;
  }

  // Calculer la taille du premier mot
  int wordLength = 0;
  while(end == 0){
    if(cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t' || cmd[i] == '\0'){
      end = 1;
    } else {
      i++;
      wordLength++;
    }
  }

  //allocation de la mémoire du premier mot 
  char *array = (char *)malloc((wordLength + 1) * sizeof(char));

  // Copier le premier mot
  for(int j = 0; j < wordLength; j++){
    array[j] = cmd[i - wordLength + j];
  }

  // fin du tableau
  array[wordLength] = '\0';
  return array;
}