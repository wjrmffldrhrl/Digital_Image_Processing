//#include <opencv\cv.h>
#include<stdlib.h>
//#include <opencv\cxcore.h>
#include <stdio.h>
#include<math.h>

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
double probability(unsigned char** img, int Row, int Col) {
	double m = 0.0;
	int i, j;


	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			m += img[i][j] / ((double)Row * Col);
		}
	}

	printf("probability = %lf", m);
	return m;

}
void makeBinary(unsigned char** img, unsigned char** out, int Row, int Col, double avg) {
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if (img[i][j] > avg)
				out[i][j] = 255;
			else
				out[i][j] = 0;


		}
	}
}

void PowImg(unsigned char** img, unsigned char** outimg, int Row, int Col, double gamma) {
	int i, j;
	double tmp;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			tmp = pow(img[i][j] / 255., 1 / gamma);

			if (tmp * 255 > 255)
				tmp = 1;
			else if (tmp * 255 < 0)
				tmp = 0;

			outimg[i][j] = tmp * 255;


		}
	}
}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	double gamma = 0.0, avg, i;


	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	gamma = 2.45;
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	PowImg(img, outimg, Row, Col, gamma);
	

	write_ucmatrix(Col, Row, outimg, argv[4]);


	getchar();
	getchar();
	return 0;
}