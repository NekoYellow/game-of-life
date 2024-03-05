/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	if (image == NULL) {
		printf("Empty image in Color *evaluateOnePixel(Image *image, int row, int col)\n");
		return NULL;
	}
	Color *cp = (Color *) malloc(sizeof(Color));
	cp->R = cp->G = cp->B = (image->image[row][col].B & 1) ? 255 : 0;
	return cp;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL) {
		printf("Empty image in Image *steganography(Image *image)\n");
		return NULL;
	}
	int n = image->rows, m = image->cols;
	Image *res = (Image *) malloc(sizeof(Image));
	res->rows = n; res->cols = m;
	res->image = (Color **) malloc(sizeof(Color*) * n);
	for (int i = 0; i < n; i++) {
		res->image[i] = (Color *) malloc(sizeof(Color) * m);
		for (int j = 0; j < m; j++) {
			res->image[i][j] = *evaluateOnePixel(image, i, j);
		}
	}
	return res;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	Image *img = readData(argv[1]);
	if (img == NULL) return -1;

	Image *converted = steganography(img);
	if (converted == NULL) return -1;

	writeData(converted);

	freeImage(img);
	freeImage(converted);

	return 0;
}
