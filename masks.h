/*
 * masks.h
 *
 *  Created on: 11/06/2013
 *      Author: Levi
 */

#ifndef MASKS_H_
#define MASKS_H_
#include "ppmReader.h"

typedef struct mask {
	double** kernel;
	int w,h;
	double normalizador;
}mask;

mask* init_mask(double** k,int h,int w);
double** apply_mask(double** target,int w,int h, mask* m);
mask* read_mask(char* filename);
double** init_matrix(int h,int w);
#endif /* MASKS_H_ */
