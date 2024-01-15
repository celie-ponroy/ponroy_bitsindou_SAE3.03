#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

//Constantes
#define MAXLI 2048

//variables globales
char cmd[MAXLI];
char path[MAXLI];

//prototypes des fonctions
void mbash();
void execute_command(char *command);
void change_directory(char *directory);

//fonction principale
int main(int argc, char** argv) {
  while (1) {
    printf("Commande: ");
    fgets(cmd, MAXLI, stdin);

    // Supprimer le caractère de nouvelle ligne de la commande
    cmd[strcspn(cmd, "\n")] = '\0';

    mbash();
  }
  return 0;
}

//fonction pour exécuter une commande interne
void mbash() {
  // Vérifier si la commande est "cd"
  if (strncmp(cmd, "cd", 2) == 0) {
    // Extraire le répertoire à partir de la commande
    char *directory = cmd + 3;
    change_directory(directory);
  } else if (strncmp(cmd, "pwd", 3) == 0) {
    // Si la commande est "pwd", afficher le répertoire courant
    printf("%s\n", getcwd(path, sizeof(path)));
  } else {
    // Sinon, exécuter la commande en respectant le contenu de la variable PATH
    execute_command(cmd);
  }
}

//fonction pour exécuter une commande externe
void execute_command(char *command) {
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0) {
    // Processus enfant
    execlp(command, command, (char *)NULL);
    // Si execlp échoue, afficher un message d'erreur
    perror("mbash");
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Erreur lors de la création du processus enfant
    perror("mbash");
  } else {
    // Processus parent, attendre que le processus enfant se termine
    waitpid(pid, &status, 0);
  }
}

//fonction pour changer le répertoire courant
void change_directory(char *directory) {
  // Changer le répertoire courant
  if (chdir(directory) != 0) {
    perror("mbash");
  }
}
