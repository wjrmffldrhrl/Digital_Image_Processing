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

void mosaic(unsigned char** img, unsigned char** out,int block) {
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


int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;



	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	Block = atoi(argv[4]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);
	printf("start mosaic\n");
	mosaic(img,outimg,Block);
	printf("end mosaic\n");

	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}