# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = gcc
CFLAGS = -lm

# ****************************************************
# Entries to bring the executable up to date

tp2: main.c masks.o ppmReader.o seam_carved.o TAD_ADJ_LIST.o
	$(CC) -o tp2 main.c masks.o ppmReader.o seam_carved.o TAD_ADJ_LIST.o -lm


ppmReader.o: ppmReader.c ppmReader.h
	$(CC) -c ppmReader.c

masks.o: masks.c masks.h ppmReader.h
	$(CC) -c masks.c

seam_carved.o: seam_carved.c seam_carved.h
	$(CC) -c seam_carved.c -lm

TAD_ADJ_LIST.o: TAD_ADJ_LIST.c TAD_ADJ_LIST.h
	$(CC) -c TAD_ADJ_LIST.c
