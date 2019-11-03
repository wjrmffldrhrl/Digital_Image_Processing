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

void combine1(unsigned char** img1, unsigned char** img2, unsigned char** img3, unsigned char** Result, int Row, int Col) {

	int i, j;
	unsigned char mask1 = 0x01;
	unsigned char mask2 = 0x01;
	unsigned char mask3 = 0x01;
	mask1 <<= 7;
	mask2 <<= 6;
	mask3 <<= 5;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			img1[i][j] = mask1 & img1[i][j];
			img2[i][j] = mask2 & img2[i][j];
			img3[i][j] = mask3 & img3[i][j];

			Result[i][j] = img1[i][j] + img2[i][j] +img3[i][j];

		}

	}
}
void combine2(unsigned char** img1, unsigned char** img2, unsigned char** img3, unsigned char** img4, unsigned char** Result, int Row, int Col) {

	int i, j;
	unsigned char mask1 = 0x01;
	unsigned char mask2 = 0x01;
	unsigned char mask3 = 0x01;
	unsigned char mask4 = 0x01;
	mask1 <<= 7;
	mask2 <<= 6;
	mask3 <<= 5;
	mask4 <<= 4;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			img1[i][j] = mask1 & img1[i][j];
			img2[i][j] = mask2 & img2[i][j];
			img3[i][j] = mask3 & img3[i][j];
			img4[i][j] = mask4 & img4[i][j];

			Result[i][j] = img1[i][j] + img2[i][j] +img3[i][j] + img4[i][j];

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

	unsigned char** img1;
	unsigned char** img2;
	unsigned char** img3;
	unsigned char** img4;
	unsigned char** outimg;
	int position;
	Row = atoi(argv[5]);
	Col = atoi(argv[6]);
	img1 = uc_alloc(Row, Col);
	img2 = uc_alloc(Row, Col);
	img3 = uc_alloc(Row, Col);
	img4 = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img1, argv[1]);
	read_ucmartrix(Row, Col, img2, argv[2]);
	read_ucmartrix(Row, Col, img3, argv[3]);
	read_ucmartrix(Row, Col, img4, argv[4]);

	combine1(img1, img2,img3 ,outimg, Row, Col);
	average(outimg, Row, Col);
	write_ucmatrix(Col, Row, outimg, argv[7]);
	outimg = uc_alloc(Row, Col);
	combine2(img1, img2, img3,img4, outimg, Row, Col);
	average(outimg, Row, Col);
	write_ucmatrix(Col, Row, outimg, argv[8]);


	getchar();
	getchar();
	return 0;
}