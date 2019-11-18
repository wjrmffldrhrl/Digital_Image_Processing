
#include<stdlib.h>
#include <stdio.h>
#include<math.h>

double** sobelYMask, ** sobelXMask, ** PrewittMask, ** RobertsMask, ** Laplace1Mask, ** Laplace2Mask;
int mask_size = 3;
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
			//sum += 128;
			if (sum < 0) sum = 0;
			else if (sum > 255) sum = 255;
			img2[i][j] = (unsigned char)sum;
		}
	}
}


void make_Mask(int mask_size, double** Mask, int checkMask) {
	int i, j;
	double sobelXMask[3][3] = {1,0,-1,2,0,-2,1,0,-1};
	double sobelYMask[3][3] = { -1,-2,-1,0,0,0,1,2,1 };

	double PrewittXMask[3][3] = { -1,0,1,-1,0,1,-1,0,1 };
	double PrewittYMask[3][3] = { -1,-1,-1,0,0,0,1,1,1 };

	double RobertsXMask[3][3] = { 0,0,-1,0,1,0,0,0,0 };
	double RobertsYMask[3][3] = { -1,0,0,0,1,0,0,0,0 };

	double Laplace1Mask[3][3] = { 0,-1,0,-1,4,-1,0,-1,0 };
	double Laplace2Mask[3][3] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	double Laplace3Mask[3][3] = { 0,-1,0,-1,5,-1,0,-1,0 };
	double Laplace4Mask[3][3] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	switch (checkMask) {

	case 0:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = sobelYMask[i][j];
		break;

	case 1:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = sobelXMask[i][j];
		break;

	case 3:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = PrewittYMask[i][j];
		break;

	case 4:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = PrewittXMask[i][j];
		break;

	case 6:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = RobertsYMask[i][j];
		break;

	case 7:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = RobertsXMask[i][j];
		break;

	case 9:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace1Mask[i][j];
		break;

	case 10:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace2Mask[i][j];
		break;

	case 11:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace3Mask[i][j];
		break;

	case 12:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace4Mask[i][j];
		break;

	default:
		printf("Mask Number is wrong \n");
		exit(1);
	}
}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	double** mask;
	int check_mask=0;

	Row = atoi(argv[3]);
	Col = atoi(argv[4]);
	check_mask = atoi(argv[2]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	mask = d_alloc(mask_size, mask_size);

	read_ucmartrix(Row, Col, img, argv[1]);

	make_Mask(mask_size, mask, check_mask);
	convolution(mask, mask_size, Row, Col, img, outimg);


	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}