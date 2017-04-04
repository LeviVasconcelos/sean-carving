/*
 * ppmReader.c
 *
 *  Created on: 08/06/2013
 *      Author: Levi
 */

#include "ppmReader.h"

void jumpLine(FILE *fp) {
	char t;
	while((t = fgetc(fp)) != '\n' && t != EOF);
}

double grayValue(double* s) {
	return (0.3 * s[0]) + (0.59 * s[1]) + (0.11 * s[2]);
}

double ** img2gray(double ***img,int* size) {
	unsigned int i,j;
	double** r = (double**) malloc(sizeof(double*) * size[0]);
	for(i = 0; i < size[0];i++) {
		r[i] = (double*)malloc(sizeof(double) * size[1]);
		for(j = 0; j < size[1];j++)
			r[i][j] = grayValue(img[i][j]);
	}
	return r;
}

double ***loadPPM(const char * filename, int* size) {
	FILE * fp;

	char s[10];
	int j,k;
	unsigned int i;
	unsigned int count;
	int lIdx,cIdx,zIdx;
	int totalElLinha;
	int maxValue;
	double ***r;
	char trash;

	fp = fopen(filename,"r");
	if (fp == 0) {
		printf("Erro ao abrir o arquivo %s\n",filename);
		return NULL;
	}
	count = 0;

	//discarta P3
	fgetc(fp); fgetc(fp);


	while(!feof(fp)) {
		trash = fgetc(fp);
		if(trash == '#') { jumpLine(fp); continue; }
		if(trash == ' ' || trash == '\n') continue;
		ungetc(trash,fp);
		fscanf(fp,"%d",&i);
		if (count < 2)
			size[count++] = i;
		if (count >= 2) break;
	}

	i = size[0];
	size[0] = size[1];
	size[1] = i;

	/*
	 * Declaração e inicialização da matriz da imagem...
	 */
	r = (double***) malloc(sizeof(double**) * size[0]);
	if(!r) { return NULL; }
	for(i = 0; i < size[0];i++) {
		r[i] = (double**) malloc(sizeof(double*) * size[1]);
		if(!r[i]) { return NULL; }
		for(j = 0; j < size[1];j++) {
			r[i][j] = (double*) malloc(sizeof(double) * 4);
			if(!r[i][j]) { return NULL; }
			for(k = 0; k < 4; k++)
				r[i][j][k] = 0.0;
		}
	}
	/*
	 * preenchimento da matriz
	 */
		count = 0;
		totalElLinha = size[1] * 3;
		//printf("size1: %d\n",2 % 6);
		maxValue = -1;
		while(!feof(fp)) {
				trash = fgetc(fp);
				if(trash == '#') { jumpLine(fp); continue; }
				if(trash == ' ' || trash == '\n' || trash == EOF) continue;
				ungetc(trash,fp);
				fscanf(fp,"%u",&i);
				if (maxValue == -1) {
					maxValue = i; continue;
				}
				lIdx = abs(count / totalElLinha);
				cIdx = abs((count /3)) % (totalElLinha/3);
				zIdx = count % 3;
				//printf("indice a ser adicionado: (%d,%d,%d) - %u [%u]\n",lIdx,cIdx,zIdx,i,count);
				r[lIdx][cIdx][zIdx] = (double) i;
				count++;
			}
		fclose(fp);
		return r;
	}

void savePPM(double*** matrix,int h,int w, char* filename) {
	int i,j,k;
	FILE *fp;
	if(!strcmp(filename,"stdout"))
		fp = stdout;
	else
		 fp = fopen(filename,"w");
	if(!fp) { printf("erro ao escrever no arquivo %s\n",filename); return; }
	fprintf(fp,"P3\n");
	fprintf(fp,"%d %d\n",w,h);
	fprintf(fp,"%d\n",255);
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			for(k = 0; k < 3; k++)
				fprintf(fp,"%d ",(int) matrix[i][j][k]);
			fprintf(fp,"\n");
		}

	}
	fclose(fp);
}

void savePPM_gray(double** matrix,int h,int w, char* filename) {
	int i,j,k;
	FILE *fp = fopen(filename,"w");
	if(!fp) { printf("erro ao escrever no arquivo %s\n",filename); return; }
	fprintf(fp,"P3\n");
	fprintf(fp,"%d %d\n",w,h);
	fprintf(fp,"%d\n",255);
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			for(k = 0; k < 3; k++)
				fprintf(fp,"%d ",(int) matrix[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}


