/*  Apodoulianakis Marios
    csd4801@csd.uoc.gr
    October 2024
    CS345 Fall Semester 2024 */

#include <fcntl.h> /*O_WRONLY O_CREAT O_TRUNC*/
#include <stdio.h>
#include <stdlib.h>   /*EXIT_FAILURE*/
#include <string.h>   /*strlen str..etc*/
#include <sys/stat.h> /*S_IRUSR S_IWUSR S_IRGRP S_IWGRP*/
#include <sys/types.h>/*pid_t*/
#include <sys/wait.h> /*waitpid, wait*/
#include <unistd.h>   /*STDOUT_FILENO*/

#define BUFFER_SIZE 512
#define ARG_LIMIT 16
#define SPLIT_CHARS "<>\n\t\r "

/*global variables*/
char bgCmdName[BUFFER_SIZE / 2];
pid_t procId;

/*function declarations*/
int findSymbol(char *a, int len, const char *symbol) {
  int index = -1;
  for (int i = 0; i < len; i++) {
    if (a[i] == symbol[0]) {
      index = i;
      break; /*if found then there is no reason to continue the loop*/
    }
    /*prepei na ftiakso gia >> */
  }
  return index;
}
void redirectStream(char **args, int streamIndex,
                    const char *symbol) { /*writes by reference to args*/
  int fd;
  if (streamIndex == -1) {
    fprintf(stderr, "streamIndex is NULL\n");
    return;
  }
  if (args[streamIndex + 1] == NULL) {
    fprintf(stderr, "No file specified for redirection\n");
    return;
  }

  if (symbol[0] == '>') {
    fd = open(args[streamIndex + 1], O_WRONLY | O_CREAT | O_TRUNC,
              S_IRUSR | S_IWUSR | S_IRGRP |
                  S_IWGRP); /*opens the filename after redirection char*/
    if (fd < 0) {
      perror("Failed to open file descriptor");
      exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) < 0) {
      perror("Failed to redirect stdout");
      exit(EXIT_FAILURE);
    }

  } else if (*symbol == '<') {
    fd = open(args[streamIndex + 1], O_RDONLY);
    if (fd < 0) {
      perror("Failed to open file descriptor");
      exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) < 0) {
      perror("Failed to redirect stdin");
    }
  } else {
    fprintf(stderr, "Invalid redirection operator\n");
    return; /*I want to check in the main function if is */
  }
  close(fd);
}
void executeCommand(char **args, int streamIndex, const char *symbol) {
  procId = fork();
  int temp;
  if (procId == 0) { /*If this condition is true, we are sure that we are at
                        child process*/
    if (streamIndex != -1 && symbol != NULL) {
      redirectStream(args, streamIndex, symbol);
    }
    if (strcmp(args[0], "cd") != 0) {
      temp = execvp(args[0], args); /*child process takes another action to
                                       execute from args array*/

      if (temp < 0) {
        perror("Failed to execute the argument");
        exit(EXIT_FAILURE);
      }
    }
  } else if (procId < 0) {
    perror("Fork failed!");
  } else { /*parent process*/

    waitpid(procId, NULL, 0);
  }
}

void handlePipes(char **args, int numCommands, int *cmdStarts) {
  int pipeFds[2 * (numCommands - 1)];
  for (int i = 0; i < (numCommands - 1); i++) {
    if (pipe(pipeFds + i * 2) < 0) {
      perror("Pipe failed");
      exit(EXIT_FAILURE);
    }
  }

  int pipeIndex = 0;
  for (int i = 0; i < numCommands; i++) {
    procId = fork();
    if (procId == 0) {
      if (i != 0) { /* Not the first command, use input pipe*/
        dup2(pipeFds[pipeIndex - 2], STDIN_FILENO);
      }
      if (i != numCommands - 1) { /* Not the last command, use output pipe*/
        dup2(pipeFds[pipeIndex + 1], STDOUT_FILENO);
      }
      for (int j = 0; j < 2 * (numCommands - 1); j++) {
        close(pipeFds[j]);
      }
      execvp(args[cmdStarts[i]], &args[cmdStarts[i]]);
      perror("Execution failed");
      exit(EXIT_FAILURE);
    }
    pipeIndex += 2;
  }
  for (int i = 0; i < 2 * (numCommands - 1); i++) {
    close(pipeFds[i]);
  }
  for (int i = 0; i < numCommands; i++) {
    wait(NULL);
  }
}

int main() {
  int linelen, index = 0, slen, check_exit = 0;
  char *user = getlogin();
  char dir[BUFFER_SIZE];
  char temp[BUFFER_SIZE];
  char line[BUFFER_SIZE];
  char *token;
  char *arguments[ARG_LIMIT];
  char tmp[BUFFER_SIZE];
  char *tok;
  char *semArgs[ARG_LIMIT];

  system("clear");
  while (1) {
    if ((user == NULL) || (getcwd(dir, sizeof(dir)) == NULL)) {
      fprintf(
          stderr,
          "We can't access username or working directory\n\t System Exits!!\n");
      sleep(2);
      exit(1);
    }

    printf("csd4801-hy345sh@%s:%s: ", user, dir);

    if (fgets(line, BUFFER_SIZE, stdin) == NULL) {
      /*we use fgets because we can set BUFFER_SIZE unlike gets*/
      fprintf(
          stderr,
          "we can't read any arguments from the terminal\n\t System Exits!!");
      sleep(1);
      exit(1);
    }
    if (strcmp(line, "\n") == 0 || strlen(line) == 0) {
      continue; /* Skip iteration if input is empty or just newline*/
    }

    strcpy(tmp, line);
    slen = 0;
    tok = strtok(tmp, ";");
    while (tok != NULL) {
      semArgs[slen] = tok;
      tok = strtok(NULL, ";");
      slen++;
    }

    for (int k = 0; k < slen; k++) {
      strcpy(temp, semArgs[k]); /*Copy the input line to a temporary buffer
                                   because strtok modifies the original string*/
      linelen = 0;
      // printf("temp: %s\n",temp);

      token = strtok(temp, SPLIT_CHARS);
      // printf("token: %s\n",token);

      while (token != NULL) { /*not correct tokenize for redirect*/
        arguments[linelen] = token;
        token = strtok(NULL, SPLIT_CHARS);
        linelen++;
      }
      arguments[linelen] = NULL; /*we want the to be correct*/

      if (strcmp(arguments[0], "exit") == 0) {
        printf("System Exits !!\n");
        /*sleep(2);*/
        check_exit = 1;
        break;
      }
      if (strcmp(arguments[0], "cd") == 0) {
        if (arguments[1] == NULL) {
          chdir(getenv("~"));
        } else {
          chdir(arguments[1]);
        }
      }
      int f = -1;
      if ((f = findSymbol(semArgs[k], linelen, ">")) != -1) {
        executeCommand(arguments, f, ">");
        continue;
      }

      if ((f = findSymbol(semArgs[k], linelen, "<")) != -1) {
        executeCommand(arguments, f, "<");
        continue;
      }

      if (strchr(semArgs[k], '=')) {
        char *envArgs[2];
        envArgs[0] = strtok(arguments[0], "=");
        envArgs[1] = strtok(NULL, "="); /*I take the remaining*/
        if (envArgs[0] != NULL && envArgs[1] != NULL) {
          if (setenv(envArgs[0], envArgs[1], 1) != 0) {
            perror("setenv failed");
            exit(EXIT_FAILURE);
          }
        }
        continue;
      }
      if (strcmp(arguments[0], "echo") == 0) {
        char *argCheck;
        for (int i = 1; i < linelen; i++) {
          argCheck = arguments[i];
          if (argCheck[0] == '$') {
            char *temp1 = getenv(argCheck + 1);
            if (temp1 != NULL) {
              arguments[i] = temp1;
            }
          }
          printf("%s ", arguments[i]);
        }
        printf("\n");
        continue;
      }
      if (strchr(semArgs[k], '|')) {
        int pipeCount = 0;
        for (int i = 0; arguments[i] != NULL; i++) {
          if (strcmp(arguments[i], "|") == 0) {
            pipeCount++;
          }
        }
        int cmdStarts[pipeCount + 1];
        cmdStarts[0] = 0;
        int j = 1;
        for (int i = 0; arguments[i] != NULL; i++) {
          if (strcmp(arguments[i], "|") == 0) {
            arguments[i] = NULL;
            cmdStarts[j++] = i + 1;
          }
        }
        handlePipes(arguments, pipeCount + 1, cmdStarts);
        continue;
      }

      executeCommand(arguments, 0, NULL);
    }
    if (check_exit == 1) {
      break;
    }
  }

  return 0;
}
