#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLI 2048

char cmd[MAXLI];

void mbash();
void accueil();
int countnbArgs(char* cmd);
void execute();
char* getpremierMot(char* cmd);
void cd(char* args[]);
char *prompt;
void addToHistory(char *command[]);
void changePrompt(char *newPrompt);
void printHistory();
#define MAX_HISTORY_SIZE 100

typedef struct {//structure pour history
    int number;
    char command[2048]; //la commande
} CommandHistory;
CommandHistory history[MAX_HISTORY_SIZE];
int historyCount = 0;

/**
  * Fonction principale.
*/
int main(int argc, char** argv) {
  accueil();
  size_t promptSize = MAXLI;
  prompt = (char *)malloc(promptSize);
  getcwd(prompt, MAXLI); //prompt par défault
  while (1) {
    printf("%s:$ ",prompt);
    fgets(cmd, MAXLI, stdin);
    mbash(cmd);
  }
  return 0;
}
/**
  * Fonction qui lance l'execution de la commande.
*/
void mbash(char* cmd) {
    char* token;
    char* tokens[MAXLI];
    int index = 0;

    // Tokenize the command
    token = strtok(cmd, " \n\t");
    while (token != NULL) {
        tokens[index++] = strdup(token);
        token = strtok(NULL, " \n\t");
    }
    tokens[index] = NULL;

    // execution de la commande
    execute(tokens);  // Pass tokens, not cmd
    
    // Free l'allocation de le mémoire
    for (int i = 0; i < index; ++i) {
        free(tokens[i]);
    }
}


/**
  * Fonction qui affiche l'accueil.
*/
void accueil() {
  printf("Bienvenue sur Mini bash \\(^.^)/ \n");
  printf("comment ça fonctionne:\n 1-saisir votre commande \n 2-appuyer entrée\n");
}
/**
  * Fonction qui compte le nombre d'arguments.
*/
int countnbArgs(char* cmd) {
  int i = 0;
  int espaces = 0;
  int nbArgs = 1;
  while (cmd[i] != '\0') {
    i++;
    if (cmd[i] == ' ' || cmd[i] == '\n') {
      espaces = 1;
    } else {
      if (espaces == 1) {
        espaces = 0;
        nbArgs++;
      }
    }
  }
  return nbArgs;
}
/**
  * Fonction qui execute la commande.
*/
void execute(char *cmd[]) {
    char *premMot = cmd[0];
    char *dexiMot = cmd[1];
    
    addToHistory(cmd);

    if (strcmp(premMot, "cd") == 0) {
        cd(cmd);
    } else if (strcmp(premMot, "exit") == 0) {
        printf("%d", getpid());
        kill(getppid(), 9); // on ferme le terminal
    } else if (strncmp(cmd[0], "export", 6) == 0 && strncmp(cmd[1], "PS1=", 4) == 0) {
        changePrompt(cmd[1] + 4); // Skip "PS1=" dans cmd[1]
        printf("Nouveau prompt : %s\n", prompt);
    } else if (strcmp(premMot, "!") == 0) {
      //on récupère le deuxième mot (chiffre)
      int dexiMot = atoi(cmd[1]);

      //on trouve la commande corespondante 
      char *nvcmd = history[dexiMot - 1].command;

      // création des tokens
      char *token;
      char *tokens[MAXLI];
      int index = 0;

      token = strtok(nvcmd, " \n\t");
      while (token != NULL) {
          tokens[index++] = strdup(token);
          token = strtok(NULL, " \n\t");
      }
      tokens[index] = NULL;

      // execution de la commande
      execute(tokens);

      // Free la mémoire
      for (int i = 0; i < index; ++i) {
          free(tokens[i]);
      }

    } else if (strcmp(premMot, "history") == 0) {
        printHistory();
    } else {
        pid_t pid = fork();

        if (pid == 0) {
            // le processus fils
            execvp(cmd[0], cmd);
            //si il y a une erreur
            perror("mBash");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Fork a échoué
            perror("mBash");
        } else {
            // processus parent
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                fprintf(stderr, "Command failed with exit code %d\n", WEXITSTATUS(status));
            }
        }
    }
}
/**
  *fonction cd qui change le répertoire courant.
*/
void cd(char* args[]) {
  if (args[1] == NULL) {
    fprintf(stderr, "Expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("mBash");
    }
  }
}
/**
*Fonction qui change le prompt.
*/
void changePrompt(char* newPrompt) {
  if (newPrompt != NULL) {
    size_t length = strlen(newPrompt);

    // supprimer les "" si il y en a
    if (length >= 2 && newPrompt[0] == '"' && newPrompt[length - 1] == '"') {
      newPrompt[length - 1] = '\0'; // suppression de \0
      prompt = strdup(newPrompt + 1); // allocation de la nouvelle mémoire et copie du nouveau contenu
    } else {
      prompt = strdup(newPrompt); // si il y a aucun ""
    }
  }
}
/**
*Fonction qui ajoute la commande à l'historique.
*/
void addToHistory(char *command[]) {
    // concatenation des mots de command
    char fullCommand[MAXLI] = "";
    for (int i = 0; command[i] != NULL; ++i) {
        strcat(fullCommand, command[i]);
        strcat(fullCommand, " ");
    }


    if (historyCount < MAX_HISTORY_SIZE) {
        history[historyCount].number = historyCount + 1;
        size_t commandLength = strlen(fullCommand);

        if (fullCommand[commandLength - 1] == '\n') {
            commandLength--;
        }

        // Copie de la commande 
        int copyLength = commandLength < sizeof(history[historyCount].command) - 1 ? commandLength : sizeof(history[historyCount].command) - 1;
        strncpy(history[historyCount].command, fullCommand, copyLength);
        history[historyCount].command[copyLength] = '\0';

        historyCount++;
    }
}
/**
*Fonction qui affiche l'historique.
*/
void printHistory() {
    for (int i = 0; i < historyCount; i++) {
        printf("%d: %s\n", history[i].number, history[i].command);
    }
}