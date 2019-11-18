
#include<stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>


int Row, Col;
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
int** i_alloc(int size_x, int size_y) {
	int** m;
	int i;

	if ((m = (int**)calloc(size_y, sizeof(int*))) == NULL) {

		printf("i_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (int*)calloc(size_x, sizeof(int))) == NULL) {
			printf("i_alloc error 2\7\n");
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
void Fdct(int** PEL, int** Coeff) {

	int i, j, k, l;
	//M_PI
	double cv = 1 / pow(2, 0.5);

	double N = Row * Col;
	long dd=0;

	for (k = 0; k < Row; k++) {
		for (l = 0; l < Col; l++) {

			printf("Coeff[%d][%d] = ",k,l);

			for (i = 0; i < Row; i++) {
				for (j = 0; j < Col; j++) {
					dd += PEL[i][j] * cos(((2 * i + 1) * k * M_PI) / 2 * N) * cos(((2 * j + 1) * l * M_PI) / 2 * N);
				}
			}

			Coeff[k][l] = dd * (4 * cv * cv) / pow(N, 2);

			printf("%d\n",Coeff[k][l]);
		}
	}


}


void Idct(int** Coeff, int** PEL) {
	int i, j, k, l;
	//M_PI
	double cv = 1 / pow(2, 0.5);

	double N = Row * Col;
	long dd = 0;


	for (k = 0; k < Row; k++) {
		for (l = 0; l < Col; l++) {


			for (i = 0; i < Row; i++) {
				for (j = 0; j < Col; j++) {
					dd += cv*cv*Coeff[i][j]* cos(((2 * k + 1) * i * M_PI) / 2 * N) * cos(((2 * l + 1) * j * M_PI) / 2 * N);
				}
			}

			PEL[k][l] = dd * (4 / pow(N, 2));

		}
	}


}
int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int flag;
	int** i_img;
	int** i_outimg;
	int** img8;
	int** outimg8;


	int i, j, k, l;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	flag = atoi(argv[4]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);

	i_img = i_alloc(Row, Col);
	i_outimg = i_alloc(Row, Col);
	img8 = i_alloc(Row, Col);
	outimg8 = i_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
			i_img[i][j] = img[i][j];


	for (i = 0; i < Row; i += 8) {
		for (j = 0; j < Col; j += 8) {
			printf("Fdct img[%d][%d] \n ", i, j);
		
			Fdct(i_img, i_outimg);
		
		}
	}
	printf("end Fdct\n");

	for (i = 0; i < Row; i += 8) {
		for (j = 0; j < Col; j += 8) {

			printf("Idct img[%d][%d] \n ", i, j);
			Idct(i_outimg, i_img);


		}
	}

	printf("end Idct\n");

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
			outimg[i][j] = i_img[i][j];
	write_ucmatrix(Col, Row, outimg, argv[4]);

	printf("end process\n");
	getchar();
	getchar();
	return 0;
}