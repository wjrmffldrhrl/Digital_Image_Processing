//#include <opencv\cv.h>
#include<stdlib.h>
//#include <opencv\cxcore.h>
#include <stdio.h>
#include<math.h>
#include<time.h>
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

int** int_alloc(int size_x, int size_y) {
	int** m;
	int i;

	if ((m = (int**)calloc(size_y, sizeof(int*))) == NULL) {

		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (int*)calloc(size_x, sizeof(int))) == NULL) {
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

double uniform() {
	return ((double)(rand() & RAND_MAX) / RAND_MAX - 0.5);
}

double gaussian() {
	static int ready = 0;
	static double gstore;
	double v1, v2, r, fac, gaus;
	double uniform();

	if (ready == 0) {
		do {
			v1 = 2. * uniform();
			v2 = 2. * uniform();
			r = v1 * v1 + v2 * v2;
		} while (r > 1.0);

		fac = sqrt(-2. * log(r) / r);
		gstore = v1 * fac;
		gaus = v2 * fac;
		ready = 1;

	}

	else {
		ready = 0;
		gaus = gstore;
	}

	return (gaus);
}
void noise(unsigned char** img, unsigned char** Result,int val) {
	int i, j;
	double gaus;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			gaus = gaussian();
			 

			if ((img[i][j] + gaus * val) > 255)
				Result[i][j] = 255;
			else if ((img[i][j] + gaus * val) < 0)
				Result[i][j] = 0;
			else
				Result[i][j] = img[i][j] + gaus * val;
		}
	}
}
void int_noise(unsigned char** img, int** Result, int val) {
	int i, j;
	double gaus;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			gaus = gaussian();

			Result[i][j] = img[i][j] + gaus * val;
		}
	}
}
void add_avg(unsigned char** inputimg, unsigned char** Result, int val,int howmany) {
	int i, j,k;
	int** addimg[100];
	int** addval;

	printf("memory start\n");

	for (i = 0; i < howmany; i++) {
		addimg[i] = int_alloc(Row, Col);
	}
	addval = int_alloc(Row, Col);
	printf("memory end\n");


	printf("addimg noise satrt");
	for (k = 0; k < howmany; k++) {
		
				int_noise(inputimg, addimg[k], val);
				printf("img %d\n", k);
			
		
	}
	printf("addimg noise end");

	for (k = 0; k < howmany; k++) {
		for (i = 0; i < Row; i++) {
			for (j = 0; j < Col; j++) {
				addval[i][j] += addimg[k][i][j];

			}
		}
	}

	printf("add img\n");

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			addval[i][j] /= howmany;

		}
	}
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {

			if (addval[i][j] > 255)
				Result[i][j] = 255;
			else if (addval[i][j] < 0)
				Result[i][j] = 0;
			else
				Result[i][j] =(unsigned char)addval[i][j];
			
			
		}
	}
	printf("make result\n");
}
int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;

	int** int_img;

	double gaus;
	int val,howmany;

	srand(time(NULL));

	Row = 512;
	Col = 512;
	val = atoi(argv[2]);
	howmany = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	int_img = int_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);


	noise(img, outimg,val);


	write_ucmatrix(Col, Row, outimg, argv[4]);

	
	printf("start add func\n");

	add_avg(img, outimg, val,howmany);

	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}