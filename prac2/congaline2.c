#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	<fcntl.h>

#define STRING_LENGTH 4096

int main(int argc, char const *argv[]) {
  char commandList[100][STRING_LENGTH]; //declare a two dimensional char array
  int number_command=0;                 //initialize the number of the command to 0

// acquire the commands from the local file
  while (1) {
    if (fgets(commandList[number_command],STRING_LENGTH,stdin)==NULL) {
      break;
    }
    number_command++;
  }
int length_command;
int i=0;
  for (i = 0; i < number_command; i++) {
    length_command=strlen(commandList[i]);
    if (commandList[i][length_command-1]=='\n') {
        commandList[i][length_command-1]='\0';
    }
  }

// EXECUTE THE COMMAND HERE
// declare new file descriptors and old file descriptors
// to implement the transition of data
int new_pipe_fds[2];
int old_pipe_fds[2];

int processID;
//  this is the index for the for loop below

int index;
for (index = 0; index < number_command; index++) {
    // if there is a command after the current command,
    // we need to build a pipe
    if (index+1<number_command) {
       // We need a pipe if there is NEXT command
      pipe(new_pipe_fds);
    }

    // CREATE A CHILD PROCESS BY fork()
    processID=fork();

    // CHILD process starts from here
    if (processID==0) {
      // if there is PREVIOUS command
      if (index-1>=0) {
         // We read from a pipe – which is the PREVIOUS pipe
         dup2(old_pipe_fds[0],STDIN_FILENO);
         close(old_pipe_fds[0]);
         close(old_pipe_fds[1]);
      }
      // if there is a NEXT command
      if(index+1<number_command){
         // We write to a pipe which is the new pipe


         close(new_pipe_fds[0]);
         dup2(new_pipe_fds[1],STDOUT_FILENO);
         // close(1);
         // dup(new_pipe_fds[1]);
         close(new_pipe_fds[1]);
         // close(1);


      }
      // printf("%d\n",index);
      // process the command
      char *token;
      char *command[100];
      token=strtok(commandList[index]," ");
      int number_token=0;
      do {
        command[number_token]=token;
        // get the next token
        token=strtok(NULL," ");
        number_token++;
      } while(token!=NULL);
      command[number_token]=NULL;
      execvp(command[0],command);
    }

    // PARENT process starts from here
    else{

      // if there is PREVIOUS command
      if (index-1>=0) {
         // Parent no longer needs old pipe (if there is one)
        close(old_pipe_fds[0]);
        close(old_pipe_fds[1]);

      }
      // if there is a NEXT command
      if(index+1<number_command){
          // Parent arranges new pipe to become old pipe ready for next stage
        old_pipe_fds[0]=new_pipe_fds[0];
        old_pipe_fds[1]=new_pipe_fds[1];
      }
        // wait(NULL);
    }
}

  return 0;
}
