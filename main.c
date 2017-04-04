/*
 * main.c
 *
 *  Created on: 08/06/2013
 *      Author: Levi
 */


#include "ppmReader.h"
#include "priorityQueue.h"
#include "TAD_ADJ_LIST.h"
#include "utlist.h"
#include "masks.h"
#include <stdlib.h>
#include "seam_carved.h"
#include "string.h"

double *** transpor(double *** rt,int* size) {
	unsigned int i,j;
	double *** r;
	SWAP(size[0],size[1]);
	r = (double***) malloc(sizeof(double**) * size[0]);
	if(!r) { return NULL; }
	for(i = 0; i < size[0];i++) {
		r[i] = (double**) malloc(sizeof(double*) * size[1]);
		if(!r[i]) { return NULL; }
		for(j = 0; j < size[1];j++) {
			r[i][j] = (double*) malloc(sizeof(double) * 3);
			if(!r[i][j]) { return NULL; }
		}
	}
	SWAP(size[0],size[1]);
	for(i = 0; i < size[0];i++)
		for(j = 0; j < size[1]; j++)
			r[j][i] = rt[i][j];
	return r;
}


int main(int argc, char *argv[]) {

	int size[2];
	char *ppm_file,g_flag,d_flag,h_flag,w_flag,m_flag,*mask_file;
	unsigned int h_num,w_num;
	int i;
	double*** r;
	h_num = 0; w_num = 0;

	if(argc < 5) {
		printf("Comando nao reconhedido... abortando\n"); return 0;
	}
	mask_file = "sobel.txt";
	ppm_file = argv[1];
	g_flag = '0'; d_flag = '0'; h_flag = '0'; w_flag = '0'; m_flag = '0';
	for(i = 0; i < argc; i++) {


		if(!strcmp(argv[i],"-w")){

			w_flag = '1';
			w_num = atoi(argv[i+1]);
			//i = i + 1;
			continue;
		}

		if(!strcmp(argv[i],"-d")) {

			d_flag = '1';
			continue;
		}

		if(!strcmp(argv[i],"-g")) {

			g_flag = '1';
			continue;
		}

		if(!strcmp(argv[i],"-h")) {

			h_flag = '1';
			h_num = atoi(argv[i+1]);
			//i = i+1;
			continue;
		}

		if(!strcmp(argv[i],"-m")) {

			m_flag = '1';
			mask_file = argv[i+1];
			//i = i + 1;
			continue;
		}
	}
	//printf("g_flag: %c\n d_flag: %c\n h_flag: %c [%u] \n w_flag: %c [%u] \n m_flag: %c [%s] \n arquivo de entrada: %s\n",g_flag,d_flag,h_flag,h_num,w_flag,w_num,m_flag,mask_file,ppm_file);

	mask* masks = read_mask(mask_file);
	r = loadPPM(ppm_file,size);
	for(i = 0; i < h_num; i++) {
		if(g_flag == '1')
			seam_Carved(r,size,masks);
		else
			seam_carvedDP(r,size,masks);
		size[1] -= 1;
	}
	if(w_flag == '1') {
		r = transpor(r,size);
		SWAP(size[0],size[1]);
		for(i = 0; i < w_num; i++) {
			if(g_flag == '1')
				seam_Carved(r,size,masks);
			else
				seam_carvedDP(r,size,masks);
			size[1] -= 1;
		}
		r = transpor(r,size);
		SWAP(size[0],size[1]);
	}

	savePPM(r,size[0],size[1],"stdout");





	free(r);

	//printf("Falows!\n");

	return 0;
}

