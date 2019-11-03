//#include <opencv\cv.h>
#include <opencv\highgui.h>
//#include <opencv\cxcore.h>
#include <stdio.h>

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

void Negative(unsigned char** img, unsigned char** Result) {
	int i, j;

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++) 
			Result[i][j] = 255 - img[i][j];


}
void mosaic(unsigned char** img, uchar** out, int r, int c, int b) {
	int i, j, x, y, tmp, count;

	for (i = 0; i < r; i += b) {

		for (j = 0; j < c; j += b) {
			tmp = 0;
			count = 0;
			for (y = 0; y < b; y++) {
				for (x = 0; x < b; x++) {
					tmp += img[i + y][j + x];
					count++;
				}
			}

			tmp /= count;

			for (y = 0; y < b; y++) {
				for (x = 0; x < b; x++) {
					out[i + y][j + x] = tmp;
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	Block = atoi(argv[4]);
	img = uc_alloc(Row,Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);


	//Negative(img, outimg);
	mosaic(img,outimg,Row,Col,Block);
	write_ucmatrix(Col, Row, outimg, argv[5]);


	getchar();
	getchar();
	return 0;
}