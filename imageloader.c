/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Failed to open %s\n", filename);
		return NULL;
	}

	char type[3];
	fscanf(fp, "%s", type);
	if (type[0] != 'P' || type[1] != '3') {
		printf("Wrong ppm format\n");
		return NULL;
	}
	uint32_t n, m;
	fscanf(fp, "%u %u", &m, &n);
	uint8_t cmax;
	fscanf(fp, "%hhu", &cmax);
	if (cmax != 255) {
		printf("Wrong ppm format\n");
		return NULL;
	}

	Image *img = (Image *) malloc(sizeof(Image));
	img->rows = n; img->cols = m;
	Color **a = img->image = (Color **) malloc(sizeof(Color*) * n);
	// Color **a = img->image = (Color **) malloc(sizeof(Color*) * n*m);
	for (uint32_t i = 0; i < n; i++) {
		a[i] = (Color *) malloc(sizeof(Color)*m);
		for (uint32_t j = 0; j < m; j++) {
			fscanf(fp, "%hhu %hhu %hhu", &a[i][j].R, &a[i][j].G, &a[i][j].B);
		}
	}
	// alternative implementation
	// for (int i = 0; i < n*m; i++) {
	// 	Color *pixel = *(img->image + i) = (Color *) malloc(sizeof(Color));
	// 	fscanf(fp, "%hhu %hhu %hhu", &pixel->R, &pixel->G, &pixel->B);
	// }
	fclose(fp);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	uint32_t n = image->rows, m = image->cols;
	printf("P3\n");
	printf("%d %d\n", n, m);
	printf("255\n");
	Color **a = image->image;
	for (uint32_t i = 0; i < n; i++) {
		for (uint32_t j = 0; j < m; j++) {
			printf("%3hhu %3hhu %3hhu\t ", a[i][j].R, a[i][j].G, a[i][j].B);
			// printf("%3hhu %3hhu %3hhu\t ", a[i*n+j]->R, a[i*n+j]->G, a[i*n+j]->B);
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL) {
		printf("Empty image in void freeImage(Image *image)\n");
		return;
	}
	for (uint32_t i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	// for (uint32_t i = 0; i < image->rows * image->cols; i++) {
	// 	free(image->image[i]);
	// }
	free(image);
}