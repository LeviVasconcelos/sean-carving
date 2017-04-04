
#include <stdio.h>
double ***loadPPM(const char * filename, int* size);
double grayValue(double* c);
void savePPM(double*** matrix,int h,int w, char* filename);
void savePPM_gray(double** matrix,int h,int w, char* filename);

