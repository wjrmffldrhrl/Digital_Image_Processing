//#include <opencv\cv.h>
#include<stdlib.h>
//#include <opencv\cxcore.h>
#include <stdio.h>

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
void AdaptiveBinary2(unsigned char** img, unsigned char** out, int Row, int Col) {
	int i,j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if (img[i][j] > 50 && img[i][j] < 120)
				out[i][j] = img[i][j];
			else
				out[i][j] = 0;
		}
	}
}
void AdaptiveBinary1(unsigned char** img, unsigned char** out, int Row, int Col) {
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if (img[i][j] > 50 && img[i][j] < 120)
				out[i][j] = 200;
			else
				out[i][j] = img[i][j];
		}
	}
}

void AdaptiveBinary0(unsigned char** img, unsigned char** out, int Row, int Col) {
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if (img[i][j] > 50 && img[i][j] < 120)
				out[i][j] = 200;
			else
				out[i][j] = 0;
		}
	}
}


int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	double avg;
	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	AdaptiveBinary0(img, outimg, Row, Col);
	write_ucmatrix(Col, Row, outimg, argv[4]);

	AdaptiveBinary1(img, outimg, Row, Col);
	write_ucmatrix(Col, Row, outimg, argv[5]);

	AdaptiveBinary2(img, outimg, Row, Col);
	write_ucmatrix(Col, Row, outimg, argv[6]);


	getchar();
	getchar();
	return 0;
}