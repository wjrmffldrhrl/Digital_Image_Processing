
#include<stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>


int Row, Col;

#define pi 3.142857
const int m = 8, n = 8;
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
double** d_alloc(int size_x, int size_y) {
	double** m;
	int i;

	if ((m = (double**)calloc(size_y, sizeof(double*))) == NULL) {

		printf("i_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (double*)calloc(size_x, sizeof(double))) == NULL) {
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

//function to calculate discrete cosine tranform
double** dctTransform(int** matrix)
{
	int i, j, k, l;

	// dct will store the discrete cosine transform
	// double dct;
	double** dct;

	dct = d_alloc(8, 8);
	double ci, cj, dct1, sum;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {

			// ci and cj depends on frequency as well as
			// number of row and columns of specified matrix
			if (i == 0)
				ci = 1 / sqrt(m);
			else
				ci = sqrt(2) / sqrt(m);
			if (j == 0)
				cj = 1 / sqrt(n);
			else
				cj = sqrt(2) / sqrt(n);

			// sum will temporarily store the sum of 
			// cosine signals
			sum = 0;
			for (k = 0; k < m; k++) {
				for (l = 0; l < n; l++) {
					dct1 = matrix[k][l] *
						cos((2 * k + 1) * i * pi / (2 * m)) *
						cos((2 * l + 1) * j * pi / (2 * n));
					sum = sum + dct1;
				}
			}
			dct[i][j] = ci * cj * sum;
		}
	}

	return dct;
}

//function to calculate inverse dicrete cosine tranform of matrix 'in'
double** Compute8x8Idct(double** in)
{
	int i, j, u, v;
	double s;

	double** idct;

	idct = d_alloc(8, 8);
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			s = 0;

			for (u = 0; u < 8; u++)
				for (v = 0; v < 8; v++)
					s += in[u][v] * cos((2 * i + 1) * u * M_PI / 16) *
					cos((2 * j + 1) * v * M_PI / 16) *
					((u == 0) ? 1 / sqrt(2) : 1.) *
					((v == 0) ? 1 / sqrt(2) : 1.);

			idct[i][j] = s / 4;
		}
	return idct;
}


int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	double** d_img;
	double** d_outimg;
	int** img8;
	double** dimg8;
	double** outimg8;
	double** dct;

	int i, j, k, l;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	
	dct = d_alloc(8, 8);
	d_img = d_alloc(Row, Col);
	d_outimg = d_alloc(Row, Col);
	img8 = i_alloc(8,8);
	dimg8 = d_alloc(8, 8);
	outimg8 = d_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
			d_img[i][j] = img[i][j];


	for (i = 0; i < Row; i += 8) {
		for (j = 0; j < Col; j += 8) {

			printf("Fdct img[%d][%d] \n", i, j);
			for (k = 0; k < 8; k++) {//8조각 내기
				for (l = 0; l < 8; l++) {

					img8[k][l] = d_img[i + k][j + l];
				}
			}

			outimg8 = dctTransform(img8);

			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {

					d_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}


	for (i = 0; i < Row; i += 8) {
		for (j = 0; j < Col; j += 8) {
			printf("Idct img[%d][%d] \n", i, j);
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {

					img8[k][l] = d_outimg[i + k][j + l];
				}
			}

			outimg8 =Compute8x8Idct(dimg8);

			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {

					d_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}


	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
			outimg[i][j] = d_outimg[i][j];

	write_ucmatrix(Col, Row, outimg, argv[4]);

	printf("end process\n");
	getchar();
	getchar();
	return 0;
}