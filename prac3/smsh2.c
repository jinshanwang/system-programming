/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	"smsh.h"

/*
In this pipe version, I modified the splitline function to splitpipe function,
firstly, we split the command by '|',and store each one of the line into a
two dimensional array call "arglist" and then, so far, so good.
we just use the template in last assignment we uesd to process the pipe.*/




#define	DFL_PROMPT	"> "

int main()
{
	char	*cmdline, *prompt, **arglist;
	int	result;
	void	setup();

	prompt = DFL_PROMPT ;
	setup();

	while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
		// we use splitpipe() here
		// for example if the cmdline is "ls -a | wc"
		// the arglist will be as followed:
		// arglist[0]="ls -a", arglist[1]="wc", arglist[2]=NULL
		if ( (arglist = splitpipe(cmdline)) != NULL  ){

				result=executeMulComm(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}
	return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}
