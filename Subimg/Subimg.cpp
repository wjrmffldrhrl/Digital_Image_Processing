
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

void sub(unsigned char** img1, unsigned char** img2) {
	int i, j;
	int sum=0,cnt=0;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if (img1[i][j] != img2[i][j]) {
				cnt++;
				sum += img1[i][j] - img2[i][j];
			}
		}
	}

	printf("different data count = %d\n", cnt);
	printf("sum = %d\n", sum);
}


int main(int argc, char* argv[]) {

	unsigned char** img1;
	unsigned char** img2;

	Row = atoi(argv[3]);
	Col = atoi(argv[4]);
	img1 = uc_alloc(Row, Col);
	img2 = uc_alloc(Row, Col);

	read_ucmartrix(Row, Col, img1, argv[1]);
	read_ucmartrix(Row, Col, img2, argv[2]);

	sub(img1,img2);




	getchar();
	getchar();
	return 0;
}