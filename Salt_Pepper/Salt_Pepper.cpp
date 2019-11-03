
#include<stdlib.h>
#include <stdio.h>
#include<math.h>
#include<time.h>

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

void salt_pepper(unsigned char** img, unsigned char** outimg,int n) {
	int i, j,k;
	int sp;
	srand((int)time(NULL));

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
			outimg[i][j] = img[i][j];

	for (k = 0; k < n; k++) {
		i = rand() % Col;
		j = rand() % Row;

		sp = (rand() % 2) * 255;
		outimg[i][j] = sp;

	}
}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int n = 0;
	Row = atoi(argv[3]);
	Col = atoi(argv[4]);
	n = atoi(argv[2]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	salt_pepper(img, outimg, n);

	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}