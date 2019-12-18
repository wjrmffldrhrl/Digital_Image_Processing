
#include<stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
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

void nInterpolation(unsigned char** img, unsigned char** outImg, double zoom) {
	int i, j, orgR, orgC, newR, newC;
	double tmpR, tmpC;
	double zoomScaleFactorR, zoomScaleFactorC;
	double iR, iC;

	newR = zoom * Row;
	newC = zoom * Col;

	for (i = 0; i < newR; i++) {
		for (j = 0; j < newC; j++) {
			//printf("new img[%d][%d]\n", i, j);
			tmpR = i / zoom;
			tmpC = j / zoom;

			orgR = (int)floor(tmpR);
			orgC = (int)floor(tmpC);

			outImg[i][j] = img[orgR][orgC];
		}
	}
}

void bInterpolation(unsigned char** img, unsigned char** outImg, double zoomx, double zoomy) {
	int i, j, orgR, orgC, newR, newC;
	double tmpR, tmpC;
	double zoomScaleFactorR, zoomScaleFactorC;
	double iR, iC;

	newR = zoomx * Row;
	newC = zoomy * Col;
	for (i = 0; i < newR; i++) {
		for (j = 0; j < newC; j++) {
		
			tmpR = i / zoomx;
			tmpC = j / zoomy;

			orgR = (int)floor(tmpR);
			orgC = (int)floor(tmpC);

			zoomScaleFactorR = tmpR - orgR;
			zoomScaleFactorC = tmpC - orgC;

			if (orgR < 0 || orgR + 1 >= Row || orgC < 0 || orgC + 1 >= Col)
				outImg[i][j] = 0;
			else {
				iR = img[orgR][orgC] + zoomScaleFactorR * (img[orgR + 1][orgC] - img[orgR][orgC]);
				iC = img[orgR][orgC+1] + zoomScaleFactorC * (img[orgR + 1][orgC+1] - img[orgR][orgC+1]);

				outImg[i][j] = (unsigned char)(iR + zoomScaleFactorC * (iC - iR));
			}
		
		}
	}
}

void cInterpolation(unsigned char** img, unsigned char** outImg, double zoom) {
	double column[4];
	double a0, a1, a2, a3;
	double x_plus_1, y_plus_1;
	double one_minus_x, one_minus_y;
	double two_minus_x, two_minus_y;
	double pixel;

	int i, j,p, orgR, orgC, newR, newC;
	double tmpR, tmpC;
	double zoomScaleFactorR, zoomScaleFactorC,a=-1.0;

	newR = zoom * Row;
	newC = zoom * Col;

	for (i = 0; i < newR; i++) {
		for (j = 0; j < newC; j++) {

			tmpR = i / zoom;
			tmpC = j / zoom;

			orgR = (int)floor(tmpR);
			orgC = (int)floor(tmpC);

			zoomScaleFactorR = tmpR - orgR;
			zoomScaleFactorC = tmpC - orgC;

			if (orgR - 1 < 0 || orgR + 2 >= Row || orgC - 1 < 0 || orgC + 2 >= Col)
				outImg[i][j] = 0;
			else {
				if ((zoomScaleFactorC == 0.0) && (zoomScaleFactorR == 0.0)) {
					outImg[i][j] = img[orgR][orgC];
					continue;
				}
				if (zoomScaleFactorC == 0.0) {
					for (p = 0; p < 4; p++)
						column[p] = (double)img[orgR - 1 + p][orgC];

				}
				else {
					x_plus_1 = zoomScaleFactorC + 1.0;
					one_minus_x = 1.0 - zoomScaleFactorC;
					two_minus_x = 2.0 - zoomScaleFactorC;

					a0 = ((a * x_plus_1 - 5.0 * a) * x_plus_1 + 8.0 * a) * x_plus_1 - 4.0 * a;
					a1 = ((a + 2.0) * zoomScaleFactorC - (a + 3.0)) * zoomScaleFactorC * zoomScaleFactorC + 1;
					a2 = ((a + 2.0) * one_minus_x - (a + 3.0)) * one_minus_x * one_minus_x + 1;
					a3 = ((a * two_minus_x - 5.0 * a) * two_minus_x + 8.0 * a) * two_minus_x - 4.0 * a;


					if (zoomScaleFactorR == 0.0) {
						pixel = a0 * img[orgR][orgC - 1] + a1 * img[orgR][orgC] + a2 * img[orgR][orgC + 1] + a3 * img[orgR][orgC + 2];

						if (pixel < 0)
							pixel = 0;
						else if (pixel > 255)
							pixel = 255;
						outImg[i][j] = (unsigned char)pixel;
						continue;

					}
					else {
						for (p = 0; p < 4; p++)
							column[p] = a0 * img[orgR - 1 + p][orgC - 1] + a1 * img[orgR - 1 + p][orgC] + a2 * img[orgR - 1 + p][orgC + 1] + a3 * img[orgR - 1 + p][orgC + 2];


					}
				}

				y_plus_1 = zoomScaleFactorR + 1.0;
				one_minus_y = 1.0 - zoomScaleFactorR;
				two_minus_y = 2.0 - zoomScaleFactorR;

				a0 = ((a * y_plus_1 - 5.0 * a) * y_plus_1 + 8.0 * a) * y_plus_1 - 4.0 * a;
				a1 = ((a + 2.0) * zoomScaleFactorR - (a + 3.0)) * zoomScaleFactorR * zoomScaleFactorR + 1;
				a2 = ((a + 2.0) * one_minus_y - (a + 3.0)) * one_minus_y * one_minus_y + 1;
				a3 = ((a * two_minus_y - 5.0 * a) * two_minus_y + 8.0 * a) * two_minus_y - 4.0 * a;

				pixel = column[0] * a0 + column[1] * a1 + column[2] * a2 + column[3] * a3;

				if (pixel < 0)
					pixel = 0;
				else if (pixel > 255)
					pixel = 255;

				outImg[i][j] = (unsigned char)pixel;


			}

		}
	}

}

void bsInterpolation(unsigned char** img, unsigned char** outimg, double zoom) {
	double column[4];
	double a0, a1, a2, a3;
	double x_plus_1, y_plus_1;
	double one_minus_x, one_minus_y;
	double two_minus_x, two_minus_y;
	double pixel;

	int i, j, p, orgR, orgC, newR, newC;
	double tmpR, tmpC;
	double zoomScaleFactorR, zoomScaleFactorC, a = -1.0;

	newR = zoom * Row;
	newC = zoom * Col;

	for (i = 0; i < newR; i++) {
		for (j = 0; j < newC; j++) {

			tmpR = i / zoom;
			tmpC = j / zoom;

			orgR = (int)floor(tmpR);
			orgC = (int)floor(tmpC);

			zoomScaleFactorR = tmpR - orgR;
			zoomScaleFactorC = tmpC - orgC;
			if (orgR - 1 < 0 || orgR + 2 >= Row || orgC - 1 < 0 || orgC + 2 >= Col)
				outimg[i][j] = 0;
			else {
				if ((zoomScaleFactorC == 0.0) && (zoomScaleFactorR == 0.0)) {
					outimg[i][j] = img[orgR][orgC];
					continue;
				}
				if (zoomScaleFactorC == 0.0) {
					for (p = 0; p < 4; p++)
						column[p] = (double)img[orgR - 1 + p][orgC];

				}
				else {
					x_plus_1 = zoomScaleFactorC + 1.0;
					one_minus_x = 1.0 - zoomScaleFactorC;
					two_minus_x = 2.0 - zoomScaleFactorC;

					a0 = ((-0.16666666 * x_plus_1 + 1.0) * x_plus_1 - 2.0)*x_plus_1+1.33333;
					a1 = (0.5 * zoomScaleFactorC - 1.0) * zoomScaleFactorC * zoomScaleFactorC + 0.666666667;
					a2 = (0.5 * one_minus_x - 1.0) * one_minus_x * one_minus_x + 0.66666667;
					a3 = 1.0 - a0 - a1 - a2;


					if (zoomScaleFactorR == 0.0) {
						pixel = a0 * img[orgR][orgC - 1] + a1 * img[orgR][orgC] + a2 * img[orgR][orgC + 1] + a3 * img[orgR][orgC + 2];

						if (pixel < 0)
							pixel = 0;
						else if (pixel > 255)
							pixel = 255;
						outimg[i][j] = (unsigned char)pixel;
						continue;

					}
					else {
						for (p = 0; p < 4; p++)
							column[p] = a0 * img[orgR - 1 + p][orgC - 1] + a1 * img[orgR - 1 + p][orgC] + a2 * img[orgR - 1 + p][orgC + 1] + a3 * img[orgR - 1 + p][orgC + 2];


					}
				}

				y_plus_1 = zoomScaleFactorR + 1.0;
				one_minus_y = 1.0 - zoomScaleFactorR;
				two_minus_y = 2.0 - zoomScaleFactorR;


				a0 = ((-0.16666666 * y_plus_1 + 1.0) * y_plus_1 - 2.0) * y_plus_1 + 1.33333;
				a1 = (0.5 * zoomScaleFactorR - 1.0) * zoomScaleFactorR * zoomScaleFactorR + 0.666666667;
				a2 = (0.5 * one_minus_y - 1.0) * one_minus_y * one_minus_y + 0.66666667;
				a3 = 1.0 - a0 - a1 - a2;


				pixel = column[0] * a0 + column[1] * a1 + column[2] * a2 + column[3] * a3;

				if (pixel < 0)
					pixel = 0;
				else if (pixel > 255)
					pixel = 255;

				outimg[i][j] = (unsigned char)pixel;
			}
		}
	}
}

void rotate(unsigned char** img, unsigned char** outimg, double degree) {
	int i, j;

	double Radius, sinValue, cosValue;
	double newR, newC;
	int orgR, orgC;
	int centerR, centerC;

	Radius = degree*(M_PI / 180.0);

	sinValue = sin(Radius);
	cosValue = cos(Radius);
	centerR = Row / 2;
	centerC = Col / 2;


	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {

			newR = (i - centerR) * cosValue - (j - centerC) * sinValue + centerR;
			newC = (i - centerR) * sinValue + (j - centerC) * cosValue + centerC;
			
			orgR = (int)floor(newR + 0.45);
			orgC = (int)floor(newC + 0.45);
			if (orgR < 0 || orgR >= Row || orgC < 0 || orgC >= Col)
				outimg[i][j] = 0;
			else
				outimg[i][j] = img[orgR][orgC];

		}
	}
	
}


void translate(unsigned char** img, unsigned char** outimg, int movey, int movex) {
	int i, j;
	int newR, newC;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			newR = i + movey;
			newC = j + movex;
			if (newR < 0)
				newR = 0;
			else if (newR >= Row)
				newR = Row - 1;

			if (newC < 0)
				newC = 0;
			else if (newC >= Col)
				newC = Col - 1;
			outimg[newR][newC] = img[i][j];


		}
	}
}

void reflection(unsigned char** img, unsigned char** outimg, double flip) {
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {

			//printf("refletion img[%d][%d]\n", i, j);
			if (flip == 0) {
				//printf("img[%d][%d] = %d\n", i, Col - j - 1, img[i][Col - j - 1]);
				outimg[i][j] = img[i][Col - j - 1];
			}
			else if (flip == 1)
				outimg[i][j] = img[Row - i - 1][j];
			else if (flip == 2)
				outimg[i][j] = img[Row - i - 1][Col - j - 1];
		}
	}
}
int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	double zoomcoeff;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	zoomcoeff = atof(argv[4]);

	img = uc_alloc(Row, Col);
	//outimg = uc_alloc(Row*zoomcoeff, Col*zoomcoeff);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	//nInterpolation(img, outimg, zoomcoeff);

	//write_ucmatrix(Col * zoomcoeff, Row * zoomcoeff, outimg, argv[5]);
	//bInterpolation(img, outimg, zoomcoeff, zoomcoeff);

	//write_ucmatrix(Col * zoomcoeff, Row * zoomcoeff, outimg, argv[6]);
	//cInterpolation(img, outimg, zoomcoeff);

	//write_ucmatrix(Col * zoomcoeff, Row * zoomcoeff, outimg, argv[7]);
	//bsInterpolation(img, outimg, zoomcoeff);
	//rotate(img, outimg, zoomcoeff);
	//translate(img, outimg, -40, -50);
	reflection(img, outimg, zoomcoeff);
	//write_ucmatrix(Col*zoomcoeff, Row*zoomcoeff, outimg, argv[8]);
	write_ucmatrix(Col, Row, outimg, argv[5]);

	getchar();
	getchar();
	return 0;
}