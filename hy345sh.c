#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 512
#define ARG_LIMIT 16
#define SPLIT_CHARS "\t\n"

/*global variables*/
char bgCmdName[BUFFER_SIZE/2];
int bgActive;
pid_t procId;

/*function declarations*/
void checkBgProcess(char *cmd){ /*takes as input the command we want to check if have a "&" at the end*/
  int length;
  length = strlen(cmd);
  if(cmd[length-1] == '&'){/*checking*/
    cmd[length-1] = '\0';/*we remove "&" */
    bgActive = 1; /*we set the helping var TRUE so we will know if the condition inside returned TRUE */
    strcpy(bgCmdName,cmd);// we copy the command to the global variable for background command processing*/
  }else{
    bgActive =0;
  }
}

void redirectStream(char **args, int *streamIndex,const char * symbol){
  if(*symbol =='>'){
    fd = open(args[*streamIndex + 1],O_WRONLY | O_CREAT | O_TRUNC ,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if(fd < 0){
      perror("Failed to open file descriptor");
      exit(EXIT_FAILURE);
    }
    if(dup2(fd, STDOUT_FILENO) < 0){
      perror("Failed to redirect stdout");
      exit(EXIT_FAILURE);
    }
    close(fd);
  }
  else if(*symbol == '<'){
    
  }
  else{
    fprintf(stderr,"Invalid redirection operator\n");
    return;/*I want to check in the main function if is */
  }  
  close(fd); /*I don't know if this is necessary*/
}
void executeCommand(char **args){

}


int main() {
  int linelen;
  char* user;
  char dir[BUFFER_SIZE];
  char temp[BUFFER_SIZE];
  char line[BUFFER_SIZE];
  char *token = strtok(temp,"\n");
  char arguments[32];

  system("clear");
  while (1) {
    if (((user == getlogin()) == NULL) || (getcwd(dir, sizeof(dir)) == NULL)) {
      fprintf(
          stderr,
          "We can't access username or working directory\n\t System Exits!!");
      sleep(5);
      exit(1);
    }
    printf("csd4801-hy345sh@%s:%s", user, dir);
    if (fgets(line, BUFFER_SIZE, stdin) == NULL) {
      // xrhsimopoioume fgets epeidh mporoume na valoume max length se antithesi
      // me thn gets
      fprintf(stderr,"we can't read any arguments from the terminal\n\t System Exits!!");
      sleep(5);
      exit(1);//edo eixe valei break alla mou fainetai lathos kathos to systyma tha epistrepsei 0 ean ginei break 
    }
    strcpy(temp,line);/*Copy the input line to a temporary buffer because strtok modifies the original string*/
    linelen = 0;
    while(token != NULL){
        arguments[linelen] = token;
        token = strtok(NULL,"\n");
        linelen++;
    }
    arguments[linelen] = NULL;/*we want the string to be correct*/
    if(strchr(line,'>'))
    {
        for(int i=0;i<linelen;i++){
            
        }
    }


  }
  return 0;
}
