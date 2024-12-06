/* Jeradiusremy Vuong                                    */
/* Lab 4                                           */
/* Figure the area of the top of a cylinder        */
/* and the volume of a cylinder                    */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FILE_IN  "lab4.dat"
//#define FILE_IN  "lab4sample.dat"
#define FILE_OUT "lab4.txt"

int main(void)
{
    double radius, height, area, vol;
    FILE * data_in;     // Declare pointer variable
    FILE * data_out;    // for each file

    // put code here to open the data file
     data_in = fopen ("lab4.dat", "r");
    if(data_in == NULL){
        printf("Error on opening the input file \n");
        exit(EXIT_FAILURE);
    }
    // put code here to open the output file
     data_out = fopen("lab4.txt", "w");
    if(data_out == NULL){
        printf("Error on opening the output file \n");
        exit(EXIT_FAILURE);
    }

    fprintf(data_out, "\nJeremy Vuong.  Lab 4.\n");
    fprintf(data_out, "Area & Volume of Cylinders\n\n");
    fprintf(data_out, " Radius    Height      Area      Volume \n");
    fprintf(data_out, " ------    ------    -------    -------- \n");

    // put the while loop here 
    while((fscanf(data_in, "%lf%lf", &radius, &height)) == 2){
        area = M_PI * radius * radius;
        vol = M_PI * radius * radius * height; 
        fprintf(data_out,"%7.1f %9.1f  %9.3f %11.3f \n",
         radius, height, area, vol);
    }
    
    fprintf(data_out, "\n");
    // put the two fclose statements here
    fclose(data_in);
    fclose(data_out);
    return EXIT_SUCCESS;
}

