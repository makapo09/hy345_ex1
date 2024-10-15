#include <stdio.h>
#define BUFFER_SIZE 512

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
