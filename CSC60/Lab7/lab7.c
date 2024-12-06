/*  Jeremy Vuong                           */
/*  Lab 7 - Olympic long jump program      */

#define NCOMPETITORS 4
#define N_TRIES 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUT_FILENAME "lab7.txt"
//#define OUT_FILENAME "lab7sample.txt"

#define IN_FILENAME "lab7.dat"
//#define IN_FILENAME "lab7sample.dat"

//Put your two typedef structs here.
typedef struct 
{
    char name[16];
    double tries[N_TRIES];
    double best_jump;
    double deviation;
}jumper_t;

typedef struct
{
    double average_of_best;
    double winning_jump;
}stats_t;



/* FUNCTION PROTOTYPES */
/* get_data is a function to get and read data */
void get_data (char *filename,                      /* input  */ 
	       jumper_t jump_list[NCOMPETITORS] );  /* output */

/* open_out_file is a function to open the output file */
FILE * open_out_file (void);
		
/* get_stats is a function to do figure the best jump for each     */
/* jumper, compute the all-over average of the best jumps, and     */
/* find the longest jump  on the track and each jumper's deviation */
/* from that winning long jump                                     */
void get_stats(jumper_t jump_list[NCOMPETITORS], /* in & out */
 	       stats_t *jump_stats);             /* output   */

				
/*  print_all is a function to print things out.
    all of its arguments are input  */
void print_all(FILE * out_file,
	       jumper_t jump_list[NCOMPETITORS],
	       stats_t *jump_stats);				 
         

/*-----------------------------------------------------------*/
int main(void)
{
    jumper_t jump_list[NCOMPETITORS]; 
    stats_t jump_stats = {0.0, 0.0};	
	
    FILE * out_file;    /* file pointer for the output file  */
			
    out_file = open_out_file ();
	
    get_data(IN_FILENAME, jump_list);
 
    get_stats(jump_list, &jump_stats); 
		      
    print_all(out_file, jump_list, &jump_stats);
	
    return EXIT_SUCCESS;
}
/*----------------------------------------------------------*/
/* This function will open the output file and return the   */
/* file pointer name to the main program.                   */

FILE * open_out_file (void)
{
    FILE * outfile;

    outfile = fopen(OUT_FILENAME, "w");
    if (outfile == NULL)
    {
	printf("Error on fopen of %s \n", OUT_FILENAME);
	exit(EXIT_FAILURE);
    }
    fprintf(outfile, "\nJeremy Vuong, Lab 7 output. \n");

    return outfile;
}

/*-----------------------------------------------------------*/
/* This function will open and read data into an array.      */
/* It will return the value of the real length of the array. */

void get_data (char *filename,                      /* input  */ 
	       jumper_t jump_list[NCOMPETITORS] )   /* output */
{
    int n, c;
    FILE *in_file;

    /* Open the data file and read in the array */
    in_file = fopen(filename, "r");
    if (in_file == NULL)
    {
	printf("Error on fopen of %s \n", filename);
	exit(EXIT_FAILURE);
    }
	
    /* Read the competitor's names first. */

    for (n=0; n < NCOMPETITORS; n++)	/* "n" for names of jumpers */
    {
	/* "fgets" gets a string including the new line attached */
	fgets(jump_list[n].name, 15, in_file); 

    	/* strings require that the last character is NULL */
	jump_list[n].name[15] = 0;

	/* This code removes the attached new line */
	/* It will find the new line embedded in the name which it 
	   then sets to NULL.  */

	char *new_line_position = strchr(jump_list[n].name, '\n') ;
	if (new_line_position != NULL)
	    *new_line_position = 0;
    }

    /* Read the sets of tries */
    for (c=0; c < N_TRIES; c++) 
    {
  	for(n = 0; n < NCOMPETITORS; n++)
        {
	    fscanf(in_file, "%lf", &jump_list[n].tries[c]); 
	}
    }
    fclose(in_file);
    return;
}

/*-----------------------------------------------------------------*/
/* get_stats is a function to figure the best jump for each        */
/* jumper, compute the all-over average of the best jumps, and     */
/* find the longest jump on the track and each jumper's deviation  */
/* from the winning jump                                           */

void get_stats(jumper_t jump_list[NCOMPETITORS], /* in & out */
 	       stats_t *jump_stats)  	     	 /* output   */
{
    int  r,c;     /* row (competitors) and column (tries) */

    jump_stats->average_of_best = 0.0; 
    jump_stats->winning_jump =  0;	/* initialize the winning jump */

    //Check the directions for the algorithm guide for the code to put here
    for (r = 0; r < NCOMPETITORS; r++)
    {
        jump_list[r].best_jump = jump_list[r].tries[0];
        for (c = 1; c < N_TRIES; c++)
        {
            if (jump_list[r].tries[c] > jump_list[r].best_jump)
            {
                jump_list[r].best_jump = jump_list[r].tries[c];
            }
        }

        jump_stats->average_of_best += jump_list[r].best_jump;

        for (c = 0; c < N_TRIES; c++)
        {
            if (jump_list[r].tries[c] > jump_stats->winning_jump)
            {
                jump_stats->winning_jump = jump_list[r].tries[c];
            }
        }
	}

    jump_stats->average_of_best /= NCOMPETITORS;


    /* compute the deviation of jumper's best jump from the winning jump */
    for (r = 0; r < NCOMPETITORS; r++)
    {
        jump_list[r].deviation = jump_stats->winning_jump - jump_list[r].best_jump;
    }
    return;
}

/*--------------------------------------------------------*/
/*  print_all is a function to print things out.
	all of its arguments are input  */

void print_all(FILE * out_file,
	       jumper_t jump_list[NCOMPETITORS],
	       stats_t *jump_stats)		 
{
    int r,c;

    fprintf(out_file, "\nTrack Results");

    fprintf(out_file,"\n\nName               Try 1   Try 2   Try 3   Try 4   Try 5   "
		"Try 6   Best Jump   Deviation");
    fprintf(out_file,  "\n----------------   -----   -----   -----   -----   -----   "
		"-----   ---------   ---------\n");

    for (r = 0; r< NCOMPETITORS; r++)
    {
	fprintf(out_file, "%-16s  ", jump_list[r].name);
	for (c = 0; c < N_TRIES; c++)
        {
            fprintf(out_file, "%6.2f  ", jump_list[r].tries[c] );
	}	
	fprintf(out_file, "  %6.2f   ", jump_list[r].best_jump );
	fprintf(out_file, "  %6.2f   \n", jump_list[r].deviation );
    }

    fprintf(out_file, "\n\nBest Jump Average  = %6.2f meters ", jump_stats->average_of_best);
    fprintf(out_file, "\n\nWinning Jump       = %6.2f meters\n\n", jump_stats->winning_jump);

    return;
}
/*--------------------------------------------------------*/
