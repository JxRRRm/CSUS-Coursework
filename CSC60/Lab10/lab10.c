/* Author(s): Ruthann Biel & Jeremy Vuong                   	 
 * This is csc60mshell.c
 * This is lab10.c the csc60mshell
 * This program serves as the base for doing labs 9 and 10.
 * Student is required to use this program to build a mini shell
 * using the specification as documented in the directions.
 * Date: Spring 2022
 */

#include "lab9_10.h"


/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
int main(void)
{
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    int argc;

    // Loop forever to wait and process commands 
    while (TRUE) 
    {
        printf("\ncsc60msh> ");

        /* read the command the user types in */
        fgets(cmdline, MAXLINE, stdin);

        /* Call ParseCmd to build argc/argv; their limits declared in lab9_10.h */
        // You write this call.
        argc = ParseCmd(cmdline, argv);

        // REQUIRED: Just-to-make-sure printfs 
        printf("Argc = %i \n",  argc);
        int i;
        for(i = 0; i < argc; i++)
        {
            printf ("Argv %i = %s \n", i, argv[i]);
        }

        // If user hits enter key without a command, continue to loop again at the beginning 
        // You write this line.  Hint: if argc is zero, no command declared */
        if(argc == 0){
            continue;   // a command was not entered, continue    
        }
        // Handle build-in commands: exit, pwd, or cd 
        // See the directions for the algorithms to do these 3 commands.
        if(strcmp(argv[0], "exit")==0){
            exit(EXIT_SUCCESS); // exit shell
        }
        else if(strcmp(argv[0], "pwd")==0){
            //print working directory
            char path[MAX_PATH_LENGTH];  // to store current directory
            getcwd(path, MAX_PATH_LENGTH);  // get current directory
            printf("%s\n", path);
           continue;
        }
       else if(strcmp(argv[0], "cd")==0){
           char *dir;
           if(argc == 1){
               dir = getenv("HOME");       
           }else{
               dir = argv[1];
           }
           if (chdir(dir) != 0) 
                   perror("error changing directory");
           continue;  
       }


        /* Else, fork off a process */
        else
        {
             RunExtCmd(argc, argv);

        }  /* end of the if-else-if (exit) */

    }	   /* end of the while loop */
}   	   /* end of main */

