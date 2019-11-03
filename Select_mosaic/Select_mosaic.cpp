
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
void mosaic(unsigned char** img, unsigned char** out, int block) {
	int i, j, x, y, tmp, count;
	for (i = 0; i < Row; i += block) {

		for (j = 0; j < Col; j += block) {

			tmp = 0;
			count = 0;

			for (y = 0; y < block; y++) {
				for (x = 0; x < block; x++) {
					tmp += img[i + y][j + x];
					count++;
				}
			}

			tmp /= count;

			for (y = 0; y < block; y++) {
				for (x = 0; x < block; x++) {
					out[i + y][j + x] = tmp;
				}
			}


		}
	}
}

void Circle_mosaic(unsigned char** img, unsigned char** Result, int block, double diameter) {
	int i, j, x, y, count;
	double tmp, xSqure, ySqure;
	unsigned char** mosaic_img;
	mosaic_img = uc_alloc(Row, Col);

	mosaic(img, mosaic_img, block);

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {

			ySqure = (abs(Row / 2 - i)) * (abs(Row / 2 - i));
			xSqure = (abs(Row / 2 - j)) * (abs(Row / 2 - j));

			tmp = sqrt(ySqure + xSqure);

			if (tmp < diameter)
				Result[i][j] = mosaic_img[i][j];
			else
				Result[i][j] = img[i][j];

		}
	}
}

void Square_mosaic(unsigned char** img, unsigned char** Result, int block, double diameter) {
	int i, j, x1, x2, y1, y2;
	double tmp, xSqure, ySqure;
	unsigned char** mosaic_img;
	mosaic_img = uc_alloc(Row, Col);

	mosaic(img, mosaic_img, block);


	x1 = Row / 2 + diameter;
	x2 = Row / 2 - diameter;
	y1 = Col / 2 + diameter;
	y2 = Col / 2 - diameter;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {

			if (i < x1 && i > x2 && j < y1 && j > y2)
				Result[i][j] = mosaic_img[i][j];
			else
				Result[i][j] = img[i][j];


		}
	}
}


int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int Block, diameter;
	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	diameter = atoi(argv[4]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);
	Circle_mosaic(img, outimg, 8, diameter);
	//Square_mosaic(img,outimg,8,diameter);
	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}