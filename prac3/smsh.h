#define	YES	1
#define	NO	0
// default function
char	*next_cmd();
char	**splitline(char *);
void	freelist(char **);
void	*emalloc(size_t);
void	*erealloc(void *, size_t);
int		execute(char **);
void	fatal(char *, char *, int );
int		process();

// extra function
int 	executeMulComm(char **);			//execute the multiple command
char**  execvpArglist(char *);				//parse the cmdline
char**  splitpipe(char *);					//split the command when encounter '|'
int 	number_of_arg(char *commandList[]);	//return the number of the command list
int 	isRedir(char **command);			//check if there is a '>' or '<'
int 	isglobbing(char **command);			//check if there is a '*'
void 	globbingProcess(char **command);	//globbing process
char**  redirec_IN(char **);				//infile
char** 	redirec_OUT(char **);				//outfile
