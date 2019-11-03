
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

void contrastStreching(unsigned char** img, unsigned char** outimg, int X_Size, int Y_Size) {
	int i, j, histogram[256];
	int min = 255, max = 0;
	unsigned char LUT[256];
	double scaleFactor, tmp;
	for (i = 0; i < 256; i++) {
		histogram[i] = 0;
	}

	for (i = 0; i < Y_Size; i++) {
		for (j = 0; j < X_Size; j++) {
			histogram[img[i][j]]++;
		}
	}

	for (i = 0; i < 256; i++) {
		if (histogram[i]){
			min = i;
			break;
			}
	}
	for (i = 255; i >= 0; i--) {
		if (histogram[i]) {
			max = i;
				break;
		}
	}

	printf("Low : %d , Hight : %d \n", min, max);

	for (i = 0; i < min; i++) {
		LUT[i] = 0;
	}

	for (i = 255; i >= max; i--) {
		LUT[i] = 255;
	}

	scaleFactor = 255.0 / (double)(max - min);

	for (i = min; i < max; i++) {
		tmp = (i - min) * scaleFactor;

		if (tmp < 0)tmp = 0;
		if (tmp>255)tmp = 255;
		LUT[i] = (unsigned char)tmp;
	}

	for (i = 0; i < Y_Size; i++) {
		for (j = 0; j < X_Size; j++) {
			outimg[i][j] = LUT[img[i][j]];
		}
	}

}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	contrastStreching(img, outimg, Row, Col);
	write_ucmatrix(Col, Row, outimg, argv[4]);


	getchar();
	getchar();
	return 0;
}