/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include "imageloader.h"

//Determines if the cell is alive
uint8_t isAlive(Image *image, int row, int col) {
	Color *cp = &image->image[row][col];
	return cp->B ? 1 : 0;
}

//Determines the next state given the rule, the current state and number of alive neighbors
uint32_t nextState(uint32_t rule, uint32_t state, uint32_t nadj) {
	return (rule >> (state * 9 + nadj)) & 1;
}

//Normalizes the index
int norm(int i, int n) {
	return (i + n) % n;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	uint32_t nadj = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!i && !j) continue;
			if (isAlive(image, norm(row+i, image->rows), norm(col+j, image->cols))) nadj++;
		}
	}
	Color *cp = (Color *) malloc(sizeof(Color));
	cp->R = cp->G = cp->B = nextState(rule, isAlive(image, row, col), nadj) ? 255 : 0;
	return cp;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
			res->image[i][j] = *evaluateOneCell(image, i, j, rule);
		}
	}
	return res;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	// if (sizeof(argv) != sizeof(char *) * 3) {
	// 	printf("usage: ./gameOfLife filename rule\n\
    // 			filename is an ASCII PPM file (type P3) with maximum value 255.\n\
    // 			rule is a hex number beginning with 0x; Life is 0x1808.\n");
	// 	return -1;
	// }

	for (int iter = 0; iter < 100; iter++) {
		char *filename = argv[1];
		Image *img = readData(filename);
		if (img == NULL) return -1;

		uint32_t rule = strtol(argv[2], (char **)NULL, 16);
		Image *nxt = life(img, rule);
		if (nxt == NULL) return -1;

		writeData(nxt);
		FILE *fp = fopen(filename, "w");
		uint32_t n = nxt->rows, m = nxt->cols;
		fprintf(fp, "P3\n");
		fprintf(fp, "%d %d\n", m, n);
		fprintf(fp, "255\n");
		Color **a = nxt->image;
		for (uint32_t i = 0; i < n; i++) {
			for (uint32_t j = 0; j < m; j++) {
				fprintf(fp, "%3hhu %3hhu %3hhu\t ", a[i][j].R, a[i][j].G, a[i][j].B);
				// printf("%3hhu %3hhu %3hhu\t ", a[i*n+j]->R, a[i*n+j]->G, a[i*n+j]->B);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
		
		freeImage(img);
		freeImage(nxt);

		sleep(1);
	}

	return 0;
}
