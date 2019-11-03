//#include <opencv\cv.h>
//#include <opencv\highgui.h>
//#include <opencv\cxcore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#define unsigned char uchar
int Row, Col, Block;
unsigned char** uc_alloc(int size_x, int size_y) {
	unsigned char** m;
	int i;

	if ((m = (unsigned char**)calloc(size_y, sizeof(unsigned char*))) == NULL) {

		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (unsigned char*)calloc(size_x, sizeof(unsigned char))) == NULL) {
			printf("uc_alloc error 2\7\n");
			exit(0);

		}

	return m;

}

void read_ucmartrix(int size_x, int size_y, unsigned char** ucmatrix, char* filename) {
	int i;
	FILE* f;

	if ((fopen_s(&f, filename, "rb")) != NULL) {
		printf("%s File open Error!\n", filename);
		exit(0);

	}

	for (i = 0; i < size_y; i++)
		if (fread(ucmatrix[i], sizeof(unsigned char), size_x, f) != size_x) {
			printf("data read error \n");
			exit(0);
		}

	fclose(f);
}

void write_ucmatrix(int size_x, int size_y, unsigned char** ucmatrix, char* filename) {
	int i;
	FILE* f;


	if ((fopen_s(&f, filename, "wb")) != NULL) {
		printf("%s File open Error!\n", filename);
		exit(0);

	}



	for (i = 0; i < size_y; i++)
		if (fwrite(ucmatrix[i], sizeof(unsigned char), size_x, f) != size_x) {
			printf("data write error \n");
			exit(0);
		}


	fclose(f);
}

void BitSilcing(unsigned char** img, unsigned char** Result, int Row, int Col, int position) {
	int i, j;

	unsigned char mask = 0x01;
	mask <<= position;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if ((mask & img[i][j]) == pow(2, position)) {
				Result[i][j] = 255;
			}
			else {
				Result[i][j] = 0;
			}
		}

	}
}
double average(unsigned char** img, int Row, int Col) {
	double sum = 0, avg;
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			sum += img[i][j];
		}
	}
	avg = sum / ((double)Row * Col);
	printf("Average of Image = %lf \n", avg);

	return avg;

}
int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int position;
	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	position = atoi(argv[4]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	BitSilcing(img, outimg, Row, Col, position);

	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}