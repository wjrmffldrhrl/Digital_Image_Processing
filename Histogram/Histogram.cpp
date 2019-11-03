#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int histogram[256], cdfOfHisto[256], histogramEqual[256];

//#define unsigned char uchar

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
		if (fread(ucmatrix[i], sizeof(unsigned char), size_x, f) != size_x) {
			printf("data read error \n");
			exit(0);
		}

	fclose(f);
}

void get_hist1(unsigned char** img, int X_Size, int Y_Size) {
	int i, j, tmp;
	double tmp1;
	int t, tp, range, hrange;
	CvSize histoSize, cdfSize;
	IplImage* imgHisto, *cdfImgHisto;

	histoSize.width = 256;
	histoSize.height = 256;// 히스토그램 size

	cdfSize.width = 256;
	cdfSize.height = 256; // cdf size

	imgHisto = cvCreateImage(histoSize, 8, 1);
	cdfImgHisto = cvCreateImage(cdfSize, 8, 1);

	for (i = 0; i < histoSize.height; i++) {
		for (j = 0; j < histoSize.width; j++) {
			((unsigned char*)(imgHisto->imageData + imgHisto->widthStep * i))[j] = 0;

		}
	}

	for (i = 0; i < cdfSize.height; i++) {
		for (j = 0; j < cdfSize.width; j++) {

			((unsigned char*)(cdfImgHisto->imageData + cdfImgHisto->widthStep * i))[j] = 0;
		}
	}

		tp = X_Size * Y_Size;

		for (i = 0; i < 256; i++) { // 히스토그램 초기화
				histogram[i] = 0;
		}

		for (i = 0; i < Y_Size; i++) {
			for (j = 0; j < X_Size; j++) {// 히스토그램 값 넣기
				histogram[img[i][j]]++;
			}
		}


		tmp1 = 0;
		for (i = 0; i < 256; ++i) {
			tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i];
		}

		for (i = 0; i < 256; ++i) {

			tmp = (int)255 * (histogram[i] / tmp1);
			cvLine(imgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
		}

		cvShowImage("Histo Equal", imgHisto);

		cdfOfHisto[0] = histogram[0];
		for (i = 1; i < 256; i++) {
			cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];

		}

		tmp1 = (double)cdfOfHisto[255];
		for (i = 0; i < 256; ++i) {
			tmp = (int)255 * (cdfOfHisto[i] / tmp1);
			cvLine(cdfImgHisto, cvPoint(i, 255), cvPoint(i, 256 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
		}

		cvShowImage("CDF of Histogram", cdfImgHisto);

		range = cdfOfHisto[255] - cdfOfHisto[0];

		histogramEqual[0] = 0;

		for (i = 1; i < 256; ++i) {
			t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
			histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;

		}

		cvReleaseImage(&imgHisto);
		cvReleaseImage(&cdfImgHisto);
		for (i = 0; i < Y_Size; ++i) {
			for (j = 0; j < X_Size; ++j) {
				img[i][j] = histogramEqual[img[i][j]];
			}
		}

	
}

void get_hist(unsigned char** img, int X_Size, int Y_Size) {
	int i, j, tmp;
	double tmp1;
	int t, tp, range, hrange;
	CvSize histoSize, cdfSize;
	IplImage* imgHisto, * cdfImgHisto;

	histoSize.width = 256;
	histoSize.height = 256;

	cdfSize.width = 256;
	cdfSize.height = 256;

	imgHisto = cvCreateImage(histoSize, 8, 1);
	cdfImgHisto = cvCreateImage(cdfSize, 8, 1);	// IplImage 구조체의 메모리를 생성하여 그 포인터를 넘겨 준다.
												// cvCreateImage(이미지 크기, depth, nChennal)
												// depth 일반적으로 한 픽셀은 0~255 값으로 표현하는데 이는 1바이트로 표현
												// nChennal 이미지를 표현하는 채널 갯수 그레이 이미지는 1채녈

	for (i = 0; i < histoSize.height; i++)		//메모리 초기화
		for (j = 0; j < histoSize.width; j++) {
			((unsigned char*)(imgHisto->imageData + imgHisto->widthStep * i))[j] = 0;

		}

	for (i = 0; i < cdfSize.height; i++) {
		for (j = 0; j < cdfSize.width; j++) {

			((unsigned char*)(cdfImgHisto->imageData + cdfImgHisto->widthStep * i))[j] = 0;
		}
	}

	tp = X_Size * Y_Size;

	for (i = 0; i < 256; i++) { // 히스토그램 초기화
		histogram[i] = 0;
	}

	for (i = 0; i < Y_Size; i++) {	// 히스토그램 내용 채우기
		for (j = 0; j < X_Size; j++) {
			histogram[img[i][j]]++;
		}
	}


	tmp1 = 0;
	for (i = 0; i < 256; ++i) {
		tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i]; // 히스토 최대값 구하기
	}

	for (i = 0; i < 256; ++i) {

		tmp = (int)255 * (histogram[i] / tmp1);
		cvLine(imgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
	}

	cvShowImage("Histo Line", imgHisto);

	cdfOfHisto[0] = histogram[0];
	for (i = 1; i < 256; i++) {
		cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];//cdf값 채우기 이전값과 더해서

	}

	tmp1 = (double)cdfOfHisto[255];
	for (i = 0; i < 256; ++i) {
		tmp = (int)255 * (cdfOfHisto[i] / tmp1);
		cvLine(cdfImgHisto, cvPoint(i, 255), cvPoint(i, 256 - tmp), CV_RGB(255, 255, 255), 1, 8, 0); // 그래프 그려주는 함수
		//cvLine(CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color, int thickness, int lineType, int shift)
		/*	- img: 선을 그릴 원본 이미지.
			- pt1: 선의 양 끝 점 중 한쪽 점.
			- pt2: 선의 양 끝 점 중 나머지 한쪽 점.
			- color: 선의 색깔.
			- thickness: 선의 두께
			- lineType:  선의 종류
			8: 8-Connected Line
			4: 4-Connected Line
			CV_AA: Anti-Aliasing Line
		*/
	}

	cvShowImage("Original CDF of Histogram", cdfImgHisto);

	range = cdfOfHisto[255] - cdfOfHisto[0];

	histogramEqual[0] = 0;

	for (i = 1; i < 256; ++i) {// cdf 값을 255에 맞춰 재조정
		t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
		histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;

	}

	cvReleaseImage(&imgHisto);
	for (i = 0; i < Y_Size; ++i) {//cdf 값을 해당 이미지에 넣는다.
		for (j = 0; j < X_Size; ++j) {
			img[i][j] = histogramEqual[img[i][j]];
		}
	}


}

int main(int argc, char* argv[]) {

	int i, j;
	IplImage* cvImg;
	CvSize imgSize;
	unsigned char** img;

	if (argc != 4) {
		printf("exe imgdata xsie ysize \n");
		exit(0);
	}

	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);

	img = uc_alloc(imgSize.width, imgSize.height);
	read_ucmartrix(imgSize.width, imgSize.height, img, argv[1]);

	cvImg = cvCreateImage(imgSize, 8, 1);	

	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++) {
			((unsigned char*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
		}

	cvNamedWindow(argv[1], 1);
	cvShowImage(argv[1], cvImg);

	get_hist(img, imgSize.width, imgSize.height);

	for (i = 0; i < imgSize.height; i++) {
		for (j = 0; j < imgSize.width; j++) {
			((unsigned char*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
		}
	}

	cvShowImage("HistoGram Equalizer...", cvImg);
	get_hist1(img, imgSize.width, imgSize.height);


	cvWaitKey(0);

	cvDestroyWindow(argv[1]);
	cvReleaseImage(&cvImg);

	getchar();
	getchar();
	return 0;
}