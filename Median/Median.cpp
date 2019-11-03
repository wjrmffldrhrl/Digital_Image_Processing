
#include<stdlib.h>
#include <stdio.h>
#include<math.h>

#define FilterSize 3

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

void Bubble_sort(unsigned char* Sort, unsigned char* median_value, int mode) {
	int i, x;
	unsigned char temp, swap;

	for (x = 0; x < FilterSize * FilterSize; x++) {
		temp = Sort[x];
		for (i = x; i < FilterSize * FilterSize - 1; i++) {
			if (temp >= Sort[i + 1]) {
				swap = temp;
				temp = Sort[i + 1];
				Sort[i + 1] = swap;
			}
		}
		Sort[x] = temp;
	}

	if (mode == -1)//min
		*median_value = (unsigned char)Sort[0];
	else if (mode == 0)//median
		*median_value = (unsigned char)Sort[FilterSize * FilterSize / 2];
	else if (mode == 1)//max
		*median_value = (unsigned char)Sort[FilterSize * FilterSize - 1];


}

void median(unsigned char** img, unsigned char** outimg, int Row, int Col, int mode) {

	int i, j, x, y, z, count = 0;
	unsigned char median_value, Sort[FilterSize * FilterSize];

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			outimg[i][j] = img[i][j];
		}
	}

	for (i = 0; i < Row-FilterSize; i++) {
		for (j = 0; j < Col-FilterSize; j++){
			for (x = 0; x < FilterSize; x++) {
				for (y = 0; y < FilterSize; y++) {
					Sort[FilterSize * x + y] = img[i + x][j + y];
				}
			}


			Bubble_sort(Sort, &median_value, mode);
			outimg[i + 1][j + 1] = median_value;
		}
	}




}

int main(int argc, char* argv[]) {

	int mode;
	unsigned char** img;
	unsigned char** outimg;

	Row = atoi(argv[3]);
	Col = atoi(argv[4]);
	mode = atoi(argv[2]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	median(img, outimg, Row, Col, mode);

	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}