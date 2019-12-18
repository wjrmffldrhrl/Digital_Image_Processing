
#include<stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>


int* getZigZagMatrix(int** arr, int n, int m)
{
	int row = 0, col = 0;

	int row_inc = 0;
	int array_size = m * n;
	int* w;
	w = (int*)malloc(array_size * sizeof(int));
	int array_counter = 0;

	int mn = (m < n) ? m : n;
	for (int len = 1; len <= mn; ++len) {
		for (int i = 0; i < len; ++i) {
			w[array_counter] = arr[row][col];
			array_counter++;

			if (i + 1 == len)
				break;
			if (row_inc)
				++row, --col;
			else
				--row, ++col;
		}

		if (len == mn)
			break;

		if (row_inc)
			++row, row_inc = 0;
		else
			++col, row_inc = 1;
	}


	if (row == 0) {
		if (col == m - 1)
			++row;
		else
			++col;
		row_inc = 1;
	}
	else {
		if (row == n - 1)
			++col;
		else
			++row;
		row_inc = 0;
	}

	int MAX = ((m > n) ? m : n) - 1;
	for (int len, diag = MAX; diag > 0; --diag) {

		if (diag > mn)
			len = mn;
		else
			len = diag;

		for (int i = 0; i < len; ++i) {
			w[array_counter] = arr[row][col];
			array_counter++;

			if (i + 1 == len)
				break;

			if (row_inc)
				++row, --col;
			else
				++col, --row;
		}

		if (row == 0 || col == m - 1) {
			if (col == m - 1)
				++row;
			else
				++col;

			row_inc = 1;
		}

		else if (col == 0 || row == n - 1) {
			if (row == n - 1)
				++col;
			else
				++row;

			row_inc = 0;
		}
	}

	return w;
}

//this function returns the order of zig-zag traversal in a m*n matrix
int* getZigZagOrder(int n, int m)
{
	int row = 0, col = 0;

	// Boolean variable that will true if we
	// need to increment 'row' value otherwise
	// false- if increment 'col' value
	int row_inc = 0;
	int array_size = m * n;
	int* order;
	order = (int*)malloc(array_size * sizeof(int));
	int array_counter = 0;

	// Print matrix of lower half zig-zag pattern
	int mn = (m < n) ? m : n;
	for (int len = 1; len <= mn; ++len) {
		for (int i = 0; i < len; ++i) {
			order[array_counter] = row * m + col;
			array_counter++;

			if (i + 1 == len)
				break;
			// If row_increment value is true
			// increment row and decrement col
			// else decrement row and increment
			// col
			if (row_inc)
				++row, --col;
			else
				--row, ++col;
		}

		if (len == mn)
			break;

		// Update row or col vlaue according
		// to the last increment
		if (row_inc)
			++row, row_inc = 0;
		else
			++col, row_inc = 1;
	}

	// Update the indexes of row and col variable
	if (row == 0) {
		if (col == m - 1)
			++row;
		else
			++col;
		row_inc = 1;
	}
	else {
		if (row == n - 1)
			++col;
		else
			++row;
		row_inc = 0;
	}

	// Print the next half zig-zag pattern
	int MAX = ((m > n) ? m : n) - 1;
	for (int len, diag = MAX; diag > 0; --diag) {

		if (diag > mn)
			len = mn;
		else
			len = diag;

		for (int i = 0; i < len; ++i) {
			order[array_counter] = row * m + col;
			array_counter++;

			if (i + 1 == len)
				break;

			// Update row or col vlaue according
			// to the last increment
			if (row_inc)
				++row, --col;
			else
				++col, --row;
		}

		// Update the indexes of row and col variable
		if (row == 0 || col == m - 1) {
			if (col == m - 1)
				++row;
			else
				++col;

			row_inc = 1;
		}

		else if (col == 0 || row == n - 1) {
			if (row == n - 1)
				++col;
			else
				++row;

			row_inc = 0;
		}
	}

	return order;
}
int** i_alloc(int size_x, int size_y) {
	int** m;
	int i;

	if ((m = (int**)calloc(size_y, sizeof(int*))) == NULL) {

		printf("i_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (int*)calloc(size_x, sizeof(int))) == NULL) {
			printf("i_alloc error 2\7\n");
			exit(0);

		}

	return m;

}
int main() {
	//int arr[3][3] = { {1,2,3},{4,5,6},{7,8,9} };
	int** arr;
	int* zig;
	int i, j;
	int cnt = 0;
	arr = i_alloc(8, 8);
	
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			arr[i][j] = cnt++;

	zig = (int*)calloc(9, sizeof(int));

	zig = getZigZagMatrix(arr, 8, 8);

	for (i = 0; i <64; i++)
		printf("%d\n", zig[i]);


	getchar();
	getchar();

}