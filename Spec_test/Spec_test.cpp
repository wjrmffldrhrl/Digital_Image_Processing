
#include<stdlib.h>
#include <stdio.h>
#include<math.h>

int origin_Row, origin_Col, match_Row, match_Col;

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
	int histogram[256] = { 0 };

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




}

void get_Match(unsigned char** img, unsigned char** outimg, int X_Size, int Y_Size, int* histogram, int* histogramSpec) {
	int i, j, tmp, matchz = 0;
	int histogramMathch[256];
	float diff;

	printf("Start HistoGram Specification \n");

	for (i = 0; i < 256; i++) {
		histogramMathch[i] = 0;
		for (j = 0; j < 256; j++) {
			if ((i - histogramSpec[j]) > 0) {
				histogramMathch[i] = j;
			}
		}
	}

	for (i = 0; i < Y_Size; ++i) {
		for (j = 0; j < X_Size; ++j) {
			outimg[i][j] = histogramMathch[img[i][j]];
		}
	}

	for (i = 0; i < 256; i++)
		printf("result histomathch [%d] : %d\n", i, histogramMathch[i]);

}
int main(int argc, char* argv[]) {
	int i,j;
	unsigned char** img;
	unsigned char** match;
	unsigned char** outimg;
	int cdfOfHisto_origin[256] = { 0 };
	int cdfOfHisto_match[256] = { 0 };

	origin_Row = atoi(argv[3]);
	origin_Col = atoi(argv[4]);
	match_Row = atoi(argv[5]);
	match_Col = atoi(argv[6]);
	img = uc_alloc(origin_Row, origin_Col);
	match = uc_alloc(match_Row, match_Col);
	outimg = uc_alloc(origin_Row, origin_Col);
	read_ucmartrix(origin_Row, origin_Col, img, argv[1]);
	read_ucmartrix(256, 256, match, argv[2]);



	get_cdf(img, cdfOfHisto_origin, origin_Row, origin_Col); // 입력 이미지의 cdf 얻기
	for (i = 0; i < 256; i++)
		printf("origin cdf histomathch [%d] : %d\n", i, cdfOfHisto_origin[i]); // 얻어온 cdf 출력
	get_cdf(match, cdfOfHisto_match, match_Row, match_Col);// 지정 이미지의 cdf 얻기
	for (i = 0; i < 256; i++)
		printf("match cdf histomathch [%d] : %d\n", i, cdfOfHisto_origin[i]);


	for (i = 0; i < origin_Col; ++i) {
		for (j = 0; j < origin_Row; ++j) {
			img[i][j] = cdfOfHisto_origin[img[i][j]];
		}
	}



	get_Match(img, outimg, origin_Row, origin_Col, cdfOfHisto_origin, cdfOfHisto_match);

	write_ucmatrix(origin_Col, origin_Row, outimg, argv[7]);


	getchar();
	getchar();
	return 0;
}