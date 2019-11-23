
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
void mFdct(int** PEL, int** Coeff) {

	int x, y, u, v;
	//M_PI
	double cv,cu;
	double N = 8;
	long dd;

	for (u = 0; u < 8; u++) {
		for (v = 0; v < 8; v++) {
			dd = 0;

			if ( v == 0)
				cv = 1 / pow(2, 0.5);
			else
				cv = 1;

			if (u == 0)
				cu = 1 / pow(2, 0.5);
			else
				cu = 1;

			for (x = 0; x < 8; x++) {
				for (y = 0; y < 8; y++) {
					dd += (long)PEL[x][y] * cos(((2 * x + 1) * u * M_PI) / 2 * N) * cos(((2 * y + 1) * v * M_PI) / 2 * N);
				}
			}

			Coeff[u][v] =(int)dd * (4 * cv * cu) / pow(N, 2);
			//Coeff[u][v] = dd * (cv * cu) / pow(2 * N, 0.5);
			//Coeff[u][v] =dd * (cv * cu) / 4;

		}
	}


}


void mIdct(int** Coeff, int** PEL) {
	
	int x, y, u, v;
	//M_PI
	double cv,cu;

	double N = 8;
	long dd = 0;


	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {

			dd = 0;
		

			for (u = 0; u< 8; u++) {
				for (v = 0; v < 8; v++) {
					if (v == 0)
						cv = 1 / pow(2, 0.5);
					else
						cv = 1;

					if (u == 0)
						cu = 1 / pow(2, 0.5);
					else
						cu = 1;

					dd += (long)cv*cu*Coeff[u][v]* cos(((2 * x + 1) * u * M_PI) / 2 * N) * cos(((2 * y + 1) * v * M_PI) / 2 * N);
				}
			}

			PEL[x][y] = dd * (4 / N*N);

		}
	}


}


static int dct_buffer[8][8] = {
	{4096,4096,4096,4096,4096,4096,4096,4096},
	{5681,4816,3218,1130,-1130,-3218,-4816,-5681},
	{5352,2217,-2217,-5352,-5352,-2217,2217,5352},
	{4816,-1130,-5681,-3218,3218,5681,1130,-4816},
	{4096,-4096,-4096,4096,4096,-4096,-4096,4096},
	{3218,-5681,1130,4816,-4816,-1130,5681,-3218},
	{2217,-5352,5352,-2217,-2217,5352,-5352,2217},
	{1130,-3218,4816,-5681,5681,-4816,3218,-1130}
};

void Fdct(int** PEL, int** Coeff) {
	int i, j, k;
	long dd;
	int t[8][8];

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			dd = 0;
			for (k = 0; k < 8; k++) {
				dd += (long)PEL[i][k] * dct_buffer[j][k];

			}
			t[i][j] = ((dd + 2048) >> 12);
		}
	}


	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			dd = 0;
			for (k = 0; k < 8; k++) {
				dd += (long)t[k][i] * dct_buffer[j][k];
			}
			Coeff[j][i] = ((dd + 16384) >> 15);
		}
	}
}

void Idct(int** Coeff, int** PEL) {
	int i, j, k;
	long dd;
	int t[8][8];

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			dd = 0;
			for (k = 0; k < 8; k++) {
				dd += (long)Coeff[k][i] * dct_buffer[k][j];
			}
			t[i][j] = ((dd + 2048) / 4096);
		}
	}

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			dd = 0;
			for (k = 0; k < 8; k++) {
				dd += (long)t[k][i] * dct_buffer[k][j];
			}
			PEL[i][j] = ((dd + 16384) / 32768);
		}
	}
}
int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int** i_img;
	int** i_outimg;
	float** f_img;
	float** f_outimg;
	int** img8;
	int** outimg8;
	float** fimg8;
	float** foutimg8;


	int i, j, k, l;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
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

			printf("Fdct img[%d][%d] \n", i, j);
			for (k = 0; k < 8; k++) {//8조각 내기
				for (l = 0; l < 8; l++) {

					img8[k][l] = i_img[i + k][j + l];
				}
			}

			Fdct(img8, outimg8);

			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {

					i_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}

	
	for (i = 0; i < Row; i += 8) {
		for (j = 0; j < Col; j += 8) {
			printf("Idct img[%d][%d] \n", i, j);
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {

					img8[k][l] = i_outimg[i + k][j + l];
				}
			}

			mIdct(img8, outimg8);

			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {

					i_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}
	
	
	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
			outimg[i][j] = i_outimg[i][j];

	write_ucmatrix(Col, Row, outimg, argv[4]);

	printf("end process\n");
	getchar();
	getchar();
	return 0;
}