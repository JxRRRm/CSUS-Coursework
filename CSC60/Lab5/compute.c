/* Jeremy Vuong                                                     */
/* Function to compute the area and the circumference of a circle   */

#include "lab5.h"
#include "math.h"

void compute (double radius, double *area, double *cir)
{
    *area = M_PI * radius * radius;
    *cir = M_PI * 2 * radius;
    return;
}

   
