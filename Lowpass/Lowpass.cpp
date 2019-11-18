
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include<assert.h>

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
double** d_alloc(int size_x, int size_y) {
	double** m;
	int i;

	if ((m = (double**)calloc(size_y, sizeof(double*))) == NULL) {

		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (double*)calloc(size_x, sizeof(double))) == NULL) {
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


int rearrange(double* X, int N) {
	int i, j, * power_of_2, * pos, stage, num_of_stages = 0;
	double temp;

	for (i = N; i > 1; i >>= 1, num_of_stages++);

	if ((power_of_2 = (int*)malloc(sizeof(int) * N)) == NULL)
		return -1;
	if ((pos = (int*)malloc(sizeof(int) * N)) == NULL)
		return -1;

	power_of_2[0] = 1;
	for (stage = 1; stage < num_of_stages; stage++)
		power_of_2[stage] = power_of_2[stage - 1] << 1;

	for (i = 1; i < N - 1; i++)
		pos[i] = 0;
	for (i = 1; i < N - 1; i++) {
		if (!pos[i]) {
			for (j = 0; j < num_of_stages; j++) {
				if (i & power_of_2[j])
					pos[i] += power_of_2[num_of_stages - 1 - j];

			}
			temp = X[i];
			X[i] = X[pos[i]];
			X[pos[i]] = temp;
			pos[pos[i]] = 1;

		}
	}
	free(power_of_2);
	free(pos);
	return 0;
}

void fft(double* X_re, double* X_im, int N) {
	double X_temp_re, X_temp_im;
	double phase;
	int num_of_stages = 0, num_of_elements, num_of_sections, size_of_butterfly;
	int i, j, stage, m1, m2;
	for (i = N; i > 1; i >>= 1, num_of_stages++);
	num_of_elements = N;
	num_of_sections = 1;
	size_of_butterfly = N >> 1;

	for (stage = 0; stage < num_of_stages; stage++) {
		m1 = 0;
		m2 = size_of_butterfly;
		for (i = 0; i < num_of_sections; i++) {
			for (j = 0; j < size_of_butterfly; j++, m1++, m2++) {
				X_temp_re = X_re[m1] - X_re[m2];
				X_temp_im = X_im[m1] - X_im[m2];
				X_re[m1] = X_re[m1] + X_re[m2];
				X_im[m1] = X_im[m1] + X_im[m2];

				phase = -2.0 * M_PI * j / num_of_elements;
				X_re[m2] = X_temp_re * cos(phase) - X_temp_im * sin(phase);
				X_im[m2] = X_temp_re * sin(phase) + X_temp_im * cos(phase);
			}

			m1 += size_of_butterfly;
			m2 += size_of_butterfly;
		}

		num_of_elements >>= 1;
		num_of_sections <<= 1;
		size_of_butterfly >>= 1;

	}

	rearrange(X_re, N);
	rearrange(X_im, N);
}
int fft_2d(double** X_re, double** X_im, int N, int Mode) {
	int i, j;
	double* temp_re, * temp_im;

	if ((temp_re = (double*)malloc(sizeof(double) * N)) == NULL)
		return -1;
	if ((temp_im = (double*)malloc(sizeof(double) * N)) == NULL)
		return -1;

	if (Mode == 0) {
		for (i = 0; i < N; i++) {
			fft(X_re[i], X_im[i], N);
		}

		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				temp_re[i] = X_re[i][j];
				temp_im[i] = X_im[i][j];

			}
			fft(temp_re, temp_im, N);
			for (i = 0; i < N; i++) {
				X_re[i][j] = temp_re[i] / N;
				X_im[i][j] = temp_im[i] / N;

			}
		}
	}
	else if (Mode == 1) {
		for (i = 0; i < N; i++) {
			fft(X_re[i], X_im[i], N);
		}

		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				temp_re[i] = X_re[i][j];
				temp_im[i] = X_im[i][j];

			}
			fft(temp_re, temp_im, N);
			for (i = 0; i < N; i++) {
				X_re[i][j] = temp_re[i] / N;
				X_im[i][j] = temp_im[i] / N;

			}
		}
	}
	else if (Mode == 2) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				temp_re[j] = X_re[i][j] * pow(-1, j);
				temp_im[j] = X_im[i][j] * pow(-1, j);

			}

			fft(temp_re, temp_im, N);

			for (j = 0; j < N; j++) {
				X_re[i][j] = temp_re[j];
				X_im[i][j] = temp_im[j];
			}
		}

		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				temp_re[i] = X_re[i][j] * pow(-1, i);
				temp_im[i] = X_im[i][j] * pow(-1, i);

			}

			fft(temp_re, temp_im, N);
			for (i = 0; i < N; i++) {
				X_re[i][j] = temp_re[i] / N;
				X_im[i][j] = temp_im[i] / N;
			}
		}
	}

	free(temp_re);
	free(temp_im);
	return 0;
}


int main(int argc, char* argv[]) {
	
	int i, j, Mode = 2;
	double max = -10E30, min = 10E30;

	unsigned char** input_img, ** output_img, ** output2_img;

	FILE* fp1;
	double** fourier_img, ** imaginary_img;


	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	input_img = uc_alloc(Row, Col);
	output_img = uc_alloc(Row, Col);
	output2_img = uc_alloc(Row, Col);
	fourier_img = d_alloc(Row, Col);
	imaginary_img = d_alloc(Row, Col);

	read_ucmartrix(Row, Col, input_img, argv[1]);

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			fourier_img[i][j] = (double)input_img[i][j];
			imaginary_img[i][j] = 0.;
		}
	}

	fft_2d(fourier_img, imaginary_img, Row, Mode);

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			double diameter;

			diameter = sqrt
		}
	}

	write_ucmatrix(Col, Row, outimg, argv[4]);


	getchar();
	getchar();
	return 0;
}