#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** malloc2dR(int, int, FILE*);
void malloc2dP(int***, int, int, FILE*);
void free_matrix(int**, int);
void print_matrix(int**, int, int);
void print_vector(int*, int);
void separa(int**, int**, int**, int*, int*, int, int);

int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		printf("Error: USAGE %s PARAM FILENAME\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	FILE* fp = fopen(argv[1], "r");
	
	if (fp == NULL) {
		printf("Errore durante la lettura del file\n");
		exit(EXIT_FAILURE);
	}
	
	int row_number, col_number;
	
	fscanf(fp, "%d %d", &row_number, &col_number);
	
	int** matrix;
	int* v_1;
	int* v_2;
	int l_1;
	int l_2;
	
	malloc2dP(&matrix, row_number, col_number, fp);

	separa(matrix, &v_1, &v_2, &l_1, &l_2, row_number, col_number);
	
	printf("Primo vettore: ");
	print_vector(v_1, l_1);
	printf("Secondo vettore: ");
	print_vector(v_2, l_2);
	
	free_matrix(matrix, row_number);
	free(v_1);
	free(v_2);
	fclose(fp);
	
	return 0;
}

void print_matrix(int** matrix, int row, int col){
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

void print_vector(int* v, int n){
	
	for (int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
	
}

void free_matrix(int** matrix, int row){
	
	for (int i = 0; i < row; i++) {
		free(matrix[i]);
	}
	
	free(matrix);
	
}

int** malloc2dR(int row_number, int col_number, FILE* fp){
	
	int** matrix = (int**) malloc(sizeof(int*) * row_number);
	
	for (int i = 0; i < row_number; i++){
		matrix[i] = (int*) malloc(sizeof(int) * col_number);
	}
	
	for (int i = 0; i < row_number; i++) {
		for (int j = 0; j < col_number; j++) {
			fscanf(fp, "%d", &matrix[i][j]);
		}
	}
	return matrix;
	
}

void malloc2dP(int*** matrix, int row_number, int col_number, FILE* fp){
	
	int** local_matrix = (int**) malloc(sizeof(int*) * row_number);
	
	for (int i = 0; i < row_number; i++){
		local_matrix[i] = (int*) malloc(sizeof(int) * col_number);
	}
	
	for (int i = 0; i < row_number; i++) {
		for (int j = 0; j < col_number; j++) {
			fscanf(fp, "%d", &local_matrix[i][j]);
		}
	}
	
	*matrix = local_matrix;

	
}

void separa(int** matrix, int** vect_1, int** vect_2, int* l_1, int* l_2, int row_number, int col_number){
	
	int cells = row_number * col_number;
	
	int lenght_1 = cells/2;
	int lenght_2 = cells - lenght_1;
	
	int* local_vect_1 = (int*) malloc(sizeof(int) * lenght_1);
	int* local_vect_2 = (int*) malloc(sizeof(int) * lenght_2);
	
	int vect_1_counter = 0;
	int vect_2_counter = 0;
	int generic_counter = 1;
	
	for (int i = 0; i < row_number; i ++) {
		for (int j = 0; j < col_number; j++) {
			if (!(generic_counter % 2)) {
				local_vect_1[vect_1_counter] = matrix[i][j];
				generic_counter ++;
				vect_1_counter ++;
			} else {
				local_vect_2[vect_2_counter] = matrix[i][j];
				vect_2_counter ++;
				generic_counter ++;
			}
		}
	}
	
	*vect_1 = local_vect_1;
	*vect_2 = local_vect_2;
	*l_1 = lenght_1;
	*l_2 = lenght_2;
}
