
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
double** d_alloc(int size_x, int size_y) {
	double** m;
	int i;

	if ((m = (double**)calloc(size_y, sizeof(double*))) == NULL) {

		printf("i_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (double*)calloc(size_x, sizeof(double))) == NULL) {
			printf("i_alloc error 2\7\n");
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

void read_dmartrix(int size_x, int size_y, double** dmatrix, char* filename) {
	int i;
	FILE* f;

	if ((fopen_s(&f, filename, "rb")) != NULL) {
		printf("%s File open Error!\n", filename);
		exit(0);

	}

	for (i = 0; i < size_y; i++)
		if (fread(dmatrix[i], sizeof(double), size_x, f) != size_x) {
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
void write_dmatrix(int size_x, int size_y, double** dmatrix, char* filename) {
	int i;
	FILE* f;


	if ((fopen_s(&f, filename, "wb")) != NULL) {
		printf("%s File open Error!\n", filename);
		exit(0);

	}



	for (i = 0; i < size_y; i++)
		if (fwrite(dmatrix[i], sizeof(double), size_x, f) != size_x) {
			printf("data write error \n");
			exit(0);
		}


	fclose(f);
}

/*
void row_analysis(double* h, int F_length, int size_x, int size_y, double** image1, double** image2) {
	int i, j, k, margin, index, size_x2;
	double sum, coeff;
	double** temp;

	size_x2 = size_x / 2;
	temp = d_alloc(size_x, size_y);

	for (i = 0, coeff = 0.; i < F_length; i++)
		coeff += h[i];
	printf("%lf\n", coeff);

	margin = (int)(F_length / 2);
	for (i = 0; i < size_y; i++) {
		for (j = 0; j < size_x; j++) {
			for (k = 0, sum = 0.; k < F_length; k++) {
				index = j - margin + k;
				if (index < 0)
					index = -index;
				else if (index >= size_x)
					index = (2 * size_x - index - 2);

				sum += h[k] * image1[i][index];
			}
			if (coeff > 1.)
				sum /= coeff;

			temp[i][j] = sum;

		}
	}

	for (i = 0; i < size_y; i++)
		for (j = 0, k = 0; j < size_x; j += 2, k++)
			image2[i][k] = temp[i][j];

	//d_free(size_x, size_y, temp);
}

void column_analysis(double* h, int F_length, int size_x, int size_y, double** image1, double** image2) {
	int i, j, k, margin, index, size_x2, size_y2;
	double sum, coeff;
	double** temp;

	size_x2 = size_x/2;
	size_y2 = size_y/2;
	temp = d_alloc(size_x2, size_y);

	for (i = 0, coeff = 0.; i < F_length; i++)
		coeff += h[i];
	printf("%lf\n", coeff);

	margin = (int)(F_length / 2);
	for (i = 0; i < size_x2; i++) {
		for (j = 0; j < size_y; j++) {
			for (k = 0, sum = 0.; k < F_length; k++) {
				index = j - margin + k;
				if (index < 0)
					index = -index;
				else if (index >= size_y)
					index = (2 * size_y - index - 2);
				sum += h[k] * image1[index][i];

			}

			if (coeff > 1.)
				sum /= coeff;
	
			temp[i][j] = sum;
		}
	}

	for (i = 0; i < size_x2; i++)
		for (j = 0, k = 0; j < size_y; j += 2, k++)
			image2[k][i] = temp[j][i];


}
*/
// Antonini Filter
double A97L1[9] = {
	0.026749,
	-0.016864,
	-0.078223,
	0.266864,
	0.602949,
	0.266864,
	-0.078223,
	-0.016864,
	0.026749 };

int A97L1_l = 9;
double A97H1[11] = {
	0.0,
	0.0,
	-0.045636,
	0.028772,
	0.295636,
	-0.557543,
	0.295636,
	0.028772,
	-0.045636,
	0.0,
	0.0, };

int A97H1_l = 11;
double A97L2[11] = {
	0.0,
	0.0,
	-0.045636,
	-0.028772,
	0.295636,
	0.557543,
	0.295636,
	-0.028772,
	0.045636,
	0.0,
	0.0, };

int A97L2_l = 11;
// Antonini Filter
double A97H2[9] = {
	-0.026749,
	-0.016864,
	0.078223,
	0.266864,
	-0.602949,
	0.266864,
	0.078223,
	-0.016864,
	-0.026749 };
int A97H2_l = 9;

void row_analysis(double* h, int F_length, int size_x, int size_y, double** image1, double** image2)
{
	int i, j, k, margin, index, size_x2;
	double sum, coeff;
	double** temp;

	size_x2 = size_x / 2;
	temp = d_alloc(size_x, size_y);

	for (i = 0, coeff = 0.; i < F_length; i++) coeff += h[i];
	printf("coeff = %lf\n", coeff);

	margin = (int)(F_length / 2);
	for (i = 0; i < size_y; i++)
		for (j = 0; j < size_x; j++)
		{
			for (k = 0, sum = 0.; k < F_length; k++)
			{
				index = j - margin + k;
				if (index < 0) index = -index;
				else if (index >= size_x) index = (2 * size_x - index - 2);
				sum += h[k] * image1[i][index];
			}
			if (coeff > 1.) sum /= coeff;
			/*	if (sum < 0) sum = 0.;
				else if (sum > 255) sum = 255.;
			*/
			temp[i][j] = sum;
			//			printf("sum = %lf\n", sum);
		}

	for (i = 0; i < size_y; i++)
		for (j = 0, k = 0; j < size_x; j += 2, k++)
		{
			image2[i][k] = temp[i][j];
			//printf("row  image2[%d][%d] = %lf\n", i, k, image2[i][k]);
		}

	//free(temp); // d_free(size_x, size_y, temp);
}

void column_analysis(double* h, int F_length, int size_x, int size_y, double** image1, double** image2)
{
	int i, j, k, margin, index, size_x2, size_y2;
	double sum, coeff;
	double** temp;

	size_x2 = size_x / 2;
	size_y2 = size_y / 2;

	temp = d_alloc(size_x2, size_y);

	for (i = 0, coeff = 0.; i < F_length; i++) coeff += h[i];
	printf("coeff = %lf\n", coeff);

	margin = (int)(F_length / 2);
	for (i = 0; i < size_x2; i++)
		for (j = 0; j < size_y; j++)
		{
			for (k = 0, sum = 0.; k < F_length; k++)
			{
				index = j - margin + k;
				if (index < 0) index = -index;
				else if (index >= size_y) index = (2 * size_y - index - 2);
				sum += h[k] * image1[index][i];
			}
			if (coeff > 1.) sum /= coeff;
			if (sum < 0) sum = 0.;
			else if (sum > 255) sum = 255.;

			temp[j][i] = sum;
			//printf("sum = %lf\n", sum);

		}
	printf("col-----last\n");
	for (i = 0; i < size_x2; i++)
		for (j = 0, k = 0; j < size_y; j += 2, k++)
		{
			image2[k][i] = temp[j][i];
			//printf("col  image2[%d][%d] = %lf\n", k, i, image2[k][i]);

		}
	printf("col ==== end===\n");
	//free(temp); // d_free(size_x2, size_y, temp);
}

void conv_uc_to_d(int Row, int Col, unsigned char** ucimage, double** dimage)
{
	int i, j;

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
		{
			dimage[i][j] = (double)ucimage[i][j];
		}
}
void conv_d_to_uc(int Row, int Col, double** dimage, unsigned char** ucimage)
{
	int i, j;

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
		{
			if (dimage[i][j] > 255) ucimage[i][j] = 255;
			else if (dimage[i][j] < 0) ucimage[i][j] = 0;
			else ucimage[i][j] = (unsigned char)dimage[i][j];
		}
}
void analysis(int argc, char* argv[], int size_x, int size_y) {
	int size_x2, size_y2;
	int i, j;
	unsigned char** image1;
	unsigned char** tmpimg;
	double** Image1, ** Image2, ** Image3;
	FILE* f1;
	size_x2 = size_x / 2;
	size_y2 = size_y / 2;
	Image1 = d_alloc(size_x, size_y);
	Image2 = d_alloc(size_x2, size_y);
	Image3 = d_alloc(size_x2, size_y2);
	image1 = uc_alloc(size_x2, size_y2);
	tmpimg = uc_alloc(size_x, size_y);

	read_ucmartrix(size_x, size_y, tmpimg, argv[1]);
	conv_uc_to_d(size_x, size_y, tmpimg, Image1);



	row_analysis(A97L1, A97L1_l, size_x, size_y, Image1, Image2);
	column_analysis(A97L1, A97H1_l, size_x, size_y, Image2, Image3);
	conv_d_to_uc(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, argv[4]);

	column_analysis(A97H1, A97H1_l, size_x, size_y, Image2, Image3);
	conv_d_to_uc(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, argv[5]);

	row_analysis(A97H1, A97H1_l, size_x, size_y, Image1, Image2);
	column_analysis(A97L1, A97L1_l, size_x, size_y, Image2, Image3);
	conv_d_to_uc(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, argv[6]);

	column_analysis(A97H1, A97H1_l, size_x, size_y, Image2, Image3);
	conv_d_to_uc(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, argv[7]);


}

int main(int argc, char* argv[]) {
	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	analysis(argc, argv, Row, Col);
	
	
	/*
	unsigned char** img;
	unsigned char** outimg;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	int i, j;
	double** dimage1, ** dimage2, ** dimage3;
	int Row2, Col2;
	Row2 = Row / 2;
	Col2 = Col / 2;

	char* name1 = (char*)malloc(20 * sizeof(char));

	dimage1 = d_alloc(Row, Col);
	dimage2 = d_alloc(Row2, Col);
	dimage3 = d_alloc(Row2, Col2);
	/*
	conv_uc_to_d(Row, Col, img, dimage1);
	row_analysis(A97L1, A97H1_l, Row, Col, dimage1, dimage2);
	column_analysis(A97L1, A97L1_l, Row, Col, dimage2, dimage3);
	conv_d_to_uc(Row2, Col2, dimage3, outimg);
	*/
	/*
	conv_uc_to_d(Row, Col, img, dimage1);
	row_analysis(A97L1, A97L1_l, Row, Col, dimage1, dimage2);
	column_analysis(A97L1, A97L1_l, Row, Col, dimage2, dimage3);
	conv_d_to_uc(Row2, Col2, dimage3, outimg);

	printf("complete first\n");

	//column_analysis(A97H1, A97H1_l, Row, Col, dimage2, dimage3);
	//write_dmatrix(Row2, Col2, dimage3, name1);
	
	write_ucmatrix(Col, Row, outimg, argv[4]);
	*/

	getchar();
	getchar();
	return 0;
}