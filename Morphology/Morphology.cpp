
#include<stdlib.h>
#include <stdio.h>
#include<math.h>
#define BLK_SIZE 512
int Row, Col;

unsigned char N_MASK[3][3] = { {0,1,0},{1,1,1},{0,1,0} };
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

void BinaryErosion(unsigned char** Data_In, unsigned char** Data_Out) {
	int i, j, count;
	int indexi, indexj;
	int maski, maskj;

	for (i = 0; i < Row-2; i++) {
		for (j = 0; j <Col-2; j++) {
			count = 0;
			//printf("data_in[%d][%d] start\n",i,j);

			for (maski = 0; maski <= 2 ; maski++) {
				for (maskj = 0; maskj <= 2 ; maskj++) {
					if (N_MASK[maski][maskj] != 0) {
						indexi = i + maski;
						indexj = j + maskj;

						if (Data_In[indexi][indexj] == 255) {
							count++;
						}
					}
				}
			}
			if (count==5)
				Data_Out[i][j] = 255;
			else
				Data_Out[i][j] = 0;

		}
	}
}

void BinaryDilation(unsigned char** Data_In, unsigned char** Data_Out) {
	int i, j, count;
	int indexi, indexj;
	int maski, maskj;

	for (i = 0; i < Row - 2; i++) {
		for (j = 0; j < Col - 2; j++) {
			count = 0;
			//printf("data_in[%d][%d] start\n", i, j);

			for (maski = 0; maski <= 2; maski++) {
				for (maskj = 0; maskj <= 2; maskj++) {
					if (N_MASK[maski][maskj] != 0) {
						indexi = i + maski;
						indexj = j + maskj;

						

						if (Data_In[indexi][indexj] == 255) {
							count++;
						}
					}
				}
			}
			if (count)
				Data_Out[i][j] = 255;
			else
				Data_Out[i][j] = 0;

		}
	}

}
void BinaryOpening(unsigned char** Data_In, unsigned char** Data_Out) {

	unsigned char** TempBlk;

	TempBlk = uc_alloc(Row, Col);

	BinaryErosion(Data_In, TempBlk);
	BinaryDilation(TempBlk, Data_Out);



}

void BinaryClosing(unsigned char** Data_In, unsigned char** Data_Out) {
	unsigned char** TempBlk;

	TempBlk = uc_alloc(Row, Col);

	
	BinaryDilation(Data_In, TempBlk);
	BinaryErosion(TempBlk, Data_Out);

}

void Erosion(unsigned char **Data_In, unsigned char **Data_Out) {
	int i, j,smin,value;
	int indexi, indexj;
	int maski, maskj;

	for (i = 0; i < Row - 2; i++) {
		for (j = 0; j < Col - 2; j++) {
			smin = 256;
			//printf("data_in[%d][%d] start\n",i,j);

			for (maski = 0; maski <= 2; maski++) {
				for (maskj = 0; maskj <= 2; maskj++) {
					if (N_MASK[maski][maskj] != 0) {
						indexi = i + maski;
						indexj = j + maskj;

						if (Data_In[indexi][indexj] <smin) {
							value = (int)Data_In[indexi][indexj] - (int)N_MASK[maski][maskj];

							if (value < smin)
								smin = value;
						}
					}
				}
			}
			
			Data_Out[i][j] = (smin<0)?(unsigned char)0 : (unsigned char)smin;
			
		}
	}
}
void Dilation(unsigned char** Data_In, unsigned char** Data_Out) {
	int i, j, smax, value;
	int indexi, indexj;
	int maski, maskj;

	for (i = 0; i < Row - 2; i++) {
		for (j = 0; j < Col - 2; j++) {
			smax = 0;
			//printf("data_in[%d][%d] start\n",i,j);

			for (maski = 0; maski <= 2; maski++) {
				for (maskj = 0; maskj <= 2; maskj++) {
					if (N_MASK[maski][maskj] != 0) {
						indexi = i + maski;
						indexj = j + maskj;

						if (Data_In[indexi][indexj] > smax) {
							value = (int)Data_In[indexi][indexj] + (int)N_MASK[maski][maskj];

							if (value > smax)
								smax = value;
						}
					}
				}
			}

			Data_Out[i][j] = (smax > 255) ? (unsigned char)255 : (unsigned char)smax;

		}
	}
}
void opening(unsigned char** Data_In, unsigned char** Data_Out) {

	unsigned char** TempBlk;

	TempBlk = uc_alloc(Row, Col);

	Erosion(Data_In, TempBlk);
	Dilation(TempBlk, Data_Out);



}
void Closing(unsigned char** Data_In, unsigned char** Data_Out) {
	unsigned char** TempBlk;

	TempBlk = uc_alloc(Row, Col);


	Dilation(Data_In, TempBlk);
	Erosion(TempBlk, Data_Out);

}
int main(int argc, char* argv[]) {

	unsigned char** img;
	unsigned char** outimg;

	Row = atoi(argv[2]);
	Col = atoi(argv[3]);
	img = uc_alloc(Row, Col);
	outimg = uc_alloc(Row, Col);
	read_ucmartrix(Row, Col, img, argv[1]);

	Erosion(img, outimg);
	//opening(img, outimg);
	write_ucmatrix(Row, Col, outimg, argv[4]);
	Dilation(img, outimg);

	//BinaryErosion(img, outimg);

	//BinaryOpening(img, outimg);
	//BinaryClosing(img, outimg);
	//Closing(img, outimg);
	write_ucmatrix(Row, Col, outimg, argv[5]);

	getchar();
	getchar();
	return 0;
}