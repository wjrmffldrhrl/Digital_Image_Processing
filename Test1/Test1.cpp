
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

void get_cdf(unsigned char** img, int* histogramEqual, int X_Size, int Y_Size) {
	int i, j, tmp;

	int t, tp, range, hrange, cdfOfHisto[256];
	int histogram[256] = { 0 },sum=0;
	int half=0;

	for (i = 0; i < 256; i++) { // 히스토그램 초기화
		histogram[i] = 0;
	}

	for (i = 0; i < Y_Size; i++) { // 히스토그램 값 넣기
		for (j = 0; j < X_Size; j++) {
			histogram[img[i][j]]++;
		}
	}




	cdfOfHisto[0] = histogram[0];
	for (i = 1; i < 256; i++) {// cdf값 누적
		cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];

	}

	range = cdfOfHisto[255] - cdfOfHisto[0]; // cdf의 크기

	histogramEqual[0] = 0;

	for (i = 1; i < 256; ++i) {//cdf 정규화
		t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
		histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;

	}

	for (i = 0; i < 256; i++) {
		printf("%d : %d\n", i, histogramEqual[i]);

		sum += histogramEqual[i];
	}
	printf("sum = %d\n", sum);
	for (i = 0; i < 256; i++)
		if (histogramEqual[i] >= 128) {
			half = i;
			break;
		}

	printf("Histogram 평균값 : ");
	printf("%d\n",i);


}
double average(unsigned char** img, int Row, int Col) {
	double sum = 0, avg;
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			sum += img[i][j];
		}
	}
	avg = sum / ((double)Row * Col);

	return avg;

}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	int cdf[256] = { 0 };
	double avg=0;
	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);
	avg = average(img, Row, Col);

	get_cdf(img, cdf, Row, Col);

	


	printf("실제 평균값 : ");
	printf("%f\n", avg);

	getchar();
	getchar();
	return 0;
}