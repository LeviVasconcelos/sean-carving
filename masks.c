/*
 * masks.c
 *
 *  Created on: 11/06/2013
 *      Author: Levi
 */

#include "masks.h"

mask* init_mask(double** k,int h,int w) {
	mask* m = (mask*)malloc(sizeof(mask));
	m->h = h; m->w = w;
	m->kernel = k;
	return m;
}

double** copy_matrix(double** m,int h,int w) {
	int i,j;
	double** r = (double**) malloc(sizeof(double*) * h);
	for(i = 0; i < h; i++) {
		r[i] = (double*)malloc(sizeof(double)*w);
		for(j = 0; j < w; j++)
			r[i][j] = m[i][j];
	}
	return r;
}

double apply_local_mask(double** target,int h,int w, mask* m,int posI,int posJ) {
	double r;
	int idxIo,idxIf,normx,normy;
	int idxJo,idxJf;
	int idxI_soma,idxJ_soma;
	int idxIm, idxJm;

	if (!((m->h % 2) && (m->w %2))) { printf("Maskara precisa ser impar...\n"); return 0; }

	if((m->h % 2)) {
		normy = (m->h -1) / 2;
		idxIo = 0 - normy;
		idxIf = (m->h -1) - normy;
	}
	//todo: conversao se for par.

	if((m->w % 2)) {
		normx = (m->w -1) / 2;
		idxJo = 0 - normx;
		idxJf = (m->w -1) - normx;
	}
	r = 0;
	for(idxI_soma = idxIo; idxI_soma <= idxIf; idxI_soma++) {
		idxIm = posI + idxI_soma;
		if(idxIm < 0) idxIm = 0;
		if(idxIm > (h-1)) idxIm = h-1;
		for(idxJ_soma = idxJo; idxJ_soma <= idxJf; idxJ_soma++) {
			idxJm = posJ + idxJ_soma;
			if(idxJm < 0) idxJm = 0;
			if(idxJm > (w-1)) idxJm = w-1;
			r += target[idxIm][idxJm] * m->kernel[idxI_soma + normy][idxJ_soma + normx];
		}
	}
	r *= m->normalizador;
	return r;
}

double** apply_mask(double** target,int h,int w, mask* m) {
	int i,j;
	double** r;
	/*aplicando mascara*/
	r = (double**)malloc(sizeof(double*)*h);
	for(i = 0; i < h; i++) {
		r[i] = (double*)malloc(sizeof(double) * w);
		for(j = 0; j < w;j++){
			r[i][j] = apply_local_mask(target,h,w, m,i,j);
		}
	}

	return r;
}

mask* read_mask(char* filename) {
	int count = 0;
	int w,h,i,j;
	mask* m = (mask*)malloc(sizeof(mask) *2);
	FILE* fp = fopen(filename,"r");
	if(!fp) { printf("erro ao abrir %s",filename); return NULL; }
	while(count < 2) { /*Lendo tamanho da matriz*/
		fscanf(fp,"%d %d",&h,&w);
		m[count].kernel = init_matrix(h,w);
		m[count].h = h;
		m[count].w = w;
		m[count].normalizador = 0;
		for(i = 0; i < h;i++)
			for(j = 0; j < w;j++){
				fscanf(fp,"%lf",&(m[count].kernel[i][j]));
				m[count].normalizador += m[count].kernel[i][j];
			}
		m[count].normalizador = (m[count].normalizador == 0) ? 1:1/m[count].normalizador;
		count++;
	}
	fclose(fp);
	return m;
}

double** init_matrix(int h,int w) {
	double** r;
	int i,j;
	r = (double**)malloc(sizeof(double*)*h);
	for(i = 0; i< h; i++) {
		r[i] = (double*)malloc(sizeof(double)*w);
		for(j = 0; j < w; j++)
			r[i][j] = 0;
	}
	return r;
}

