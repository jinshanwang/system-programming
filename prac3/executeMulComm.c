#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include <string.h>
#include	<sys/wait.h>
#include "smsh.h"
#include <fcntl.h>
#include <glob.h>
#define  NOT_REDIRECTION -1
#define  REDIRECTION_IN 1
#define  REDIRECTION_OUT 2


int executeMulComm(char *commandList[])
/*  PIPE
 * purpose: run a multiple program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors

 */
{
  int number_command=number_of_arg(commandList);


/*********************process the command ****************************************/
  // EXECUTE THE COMMAND HERE
  // declare new file descriptors and old file descriptors
  // to implement the transition of data
  int new_pipe_fds[2];
  int old_pipe_fds[2];

  int processID;
  //  this is the index for the for loop below

  int index;
  for (index = 0; index < number_command; index++) {
    // parsing a command string
    char **command=execvpArglist(commandList[index]);
    // isRedir() function:
    // return -1 if there is no REDIRECTION
    // or 1 for ">"
    // or 2 for "<"
    int redir=isRedir(command);

/*********************process the command ****************************************/


/*********************child process****************************************/
      // if there is a command after the current command,
      // we need to build a pipe
      if (index+1<number_command) {
         // We need a pipe if there is NEXT command
        pipe(new_pipe_fds);
      }

      // CREATE A CHILD PROCEss BY fork()
      processID=fork();

      // CHILD process starts from here
      if (processID==0) {
        // if there is PREVIOUs command
        if (index-1>=0) {
           // We read from a pipe – which is the PREVIOUs pipe
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

/*********************child process****************************************/



/*********************execute the program*****************************************/
        // if there is symbol">"
        if (redir==REDIRECTION_IN) {
 
            command=redirec_IN(command);
        }
        // if there is symbol "<"
        else if(redir==REDIRECTION_OUT){

            command=redirec_OUT(command);
        }
        // globbing process
        // if it is "*"
        // run the globbingProcess
        // or run the standard execvp function
        if (isglobbing(command)==1) {

          globbingProcess(command);

        }
        else{
          execvp(command[0],command);
        }



        freelist(command);
/*****************************************************************/
      }

      // PARENT process starts from here
      else{

        // if there is PREVIOUs command
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


