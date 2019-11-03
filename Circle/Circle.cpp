//#include <opencv\cv.h>
#include<stdlib.h>
//#include <opencv\cxcore.h>
#include <stdio.h>
#include<math.h>

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

void Circle(unsigned char** Result, int Row, int Col, double diameter) {
	int i,j;

	double tmp, xSqure, ySqure;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {

			ySqure = (abs(Row / 2 - i)) * (abs(Row / 2 - i));
			xSqure = (abs(Row / 2 - j)) * (abs(Row / 2 - j));

			tmp = sqrt(ySqure + xSqure);

			if (tmp < diameter)
				Result[i][j] = 255;
			else
				Result[i][j] = 0;

		}
	}


}

void Square(unsigned char** Result, int diameter) {
	int i, j;
	int x1, x2, y1, y2;
	x1 = Row / 2 +diameter;
	x2 = Row / 2 - diameter;
	y1 = Col / 2 + diameter;
	y2 = Col / 2 - diameter;
	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			
			if(i < x1 && i > x2 && j < y1 && j > y2)
				Result[i][j] = 255;
			else
				Result[i][j] = 0;
		

		}
	}


}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;
	double diameter;


	Row = atoi(argv[1]);
	Col = atoi(argv[2]);
	//diameter = atof(argv[3]);
	
	outimg = uc_alloc(Row, Col);

	//Circle(outimg, Row, Col, 100);
	Square(outimg, 100);
	write_ucmatrix(Col, Row, outimg, argv[3]);


	getchar();
	getchar();
	return 0;
}