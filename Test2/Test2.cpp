
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

void Bubble_sort(unsigned char* Sort, unsigned char* median_value,int size) {
	int i, x;
	unsigned char temp, swap;

	for (x = 0; x < size; x++) {
		temp = Sort[x];
		for (i = x; i < size - 1; i++) {
			if (temp >= Sort[i + 1]) {
				swap = temp;
				temp = Sort[i + 1];
				Sort[i + 1] = swap;
			}
		}
		Sort[x] = temp;
	}


	*median_value = (unsigned char)Sort[size / 2];


}

void median(unsigned char** img, unsigned char** outimg, int Row, int Col, int size) {

	int i, j, x, y, z, count = 0;
	unsigned char median_value, * Sort;

	Sort = (unsigned char*)calloc(size, sizeof(unsigned char*));

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			outimg[i][j] = img[i][j];
		}
	}

	for (i = 0; i < Row - size; i++) {
		for (j = 0; j < Col; j++) {
			for (x = 0; x < size; x++) {
				Sort[x] = img[i + x][j];
			}

			Bubble_sort(Sort, &median_value, size);
			outimg[i + size / 2][j + size / 2] = median_value;
		}
	}

}





int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int size;
	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	size = atoi(argv[4]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	median(img, outimg, Row, Col,size);
	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}