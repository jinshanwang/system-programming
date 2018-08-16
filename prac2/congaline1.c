
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 4096
int main(int argc, char const *argv[]) {

  char commandList[100][STRING_LENGTH]; //declare a two dimensional char array
  int number_command=0;                 //initialize the number of the command to 0

// acquire the commands from the local file
  while (1) {
    if (scanf("%s\n", commandList[number_command])==EOF) {
      break;
    }
    number_command++;
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
         close(new_pipe_fds[1]);
      }
      execlp(commandList[index],commandList[index],NULL);
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
      wait(NULL);
    }
}
  return 0;
}
