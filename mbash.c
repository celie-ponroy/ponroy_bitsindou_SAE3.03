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
int main(int argc, char** argv) {
    accueil();
  while (1) {
    printf("Commande: ");
    fgets(cmd, MAXLI, stdin);
    mbash(cmd);
  }
  return 0;
}

void mbash() {
  printf("Execute: %s", cmd);
  system(cmd);
}
void accueil(){
  printf("Bienvenue sur Mini bash (^.^)\n");
}
