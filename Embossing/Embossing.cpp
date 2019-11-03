
#include<stdlib.h>
#include <stdio.h>
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
double** d_alloc(int size_x, int size_y) {
	double** m;
	int i;

	if ((m = (double**)calloc(size_y, sizeof(double*))) == NULL) {

		printf("d_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (double*)calloc(size_x, sizeof(double))) == NULL) {
			printf("d_alloc error 2\7\n");
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

void convolution(double** h, int F_length, int size_x, int size_y, unsigned char** img1, unsigned char** img2) {
	int i, j, x, y;
	int margin, indexX, indexY;
	double sum, coeff;

	margin = (int)(F_length / 2);

	for (i = 0; i < size_y; i++) {
		for (j = 0; j < size_x; j++) {
			sum = 0;
			for (y = 0; y < F_length; y++) {
				indexY = i - margin + y;
				if (indexY < 0)indexY = -indexY;
				else if (indexY >= size_y)indexY = (2 * size_y - indexY - 1);


				for (x = 0; x < F_length; x++) {
					indexX = j - margin + x;
					if (indexX < 0)indexX = -indexX;
					else if (indexX >= size_x)indexX = (2 * size_x - indexX - 1);

					sum += h[y][x] * (double)img1[indexY][indexX];
				}
			}
			sum += 128;
			if (sum < 0) sum = 0;
			else if (sum > 255) sum = 255;
			img2[i][j] = (unsigned char)sum;
		}
	}
}

int main(int argc, char* argv[]) {

	int i, j;
	unsigned char** img;
	unsigned char** outimg;
	double** embosMask;
	int embos_size = 3;


	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	embosMask = d_alloc(embos_size, embos_size);

	for (i = 0; i < embos_size; i++)
		for (j = 0; j < embos_size; j++)
			embosMask[i][j] = 0;

	embosMask[0][0] = -1;
	embosMask[2][2] = 1;

	read_ucmartrix(Row, Col, img, argv[1]);

	convolution(embosMask, embos_size, Row, Col, img, outimg);

	write_ucmatrix(Col, Row, outimg, argv[4]);


	getchar();
	getchar();
	return 0;
}