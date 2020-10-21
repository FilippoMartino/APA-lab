#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_R 50

int read_matrix(int [MAX_R][MAX_R], int *, int * , char *);
void print_matrix(int, int, int [MAX_R][MAX_R]);
int is_region(int [MAX_R][MAX_R], int, int, int, int, int *, int *);

int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		printf("Errore: PARAM %s FILENAME\n", argv[0]);
		return -1;
	}
	
	/* Definizione matrice come da consegna */
	int matrix[MAX_R][MAX_R];
	int row;
	int col;
	
	/* Lettura della matrice e uscita nel caso di errore */
	if(!read_matrix(matrix, &row, &col, strdup(argv[1])))
		return -1;

	/* Allocazione dinamica per base e altezza */
	int *base = malloc(sizeof(int));
	int *height = malloc(sizeof(int));
	
	/* Ciclo per ogni cella della matrice */
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			
			/* Per ogni regione trovata devo inizializzare a 0 base ed altezza */
			*base = 0;
			*height = 0;
			
			/* Controllo se la cella è l'indice di inizio di una regione */
			if (is_region(matrix, row, col, i, j, base, height)) {
				
				/* Stampo l'output formattato come da consegna */
				printf("Regione trovata in posizione [%d][%d], di base %d e altezza %d\n",
					   i,
					   j,
					   *base,
					   *height);
			}
		}
	}
	
	/* Alla fine del programma dealloco lo spazio occupato dai due puntatori */
	free(base);
	free(height);
	
	return 1;
	
}

/* Si limita a leggere la matrice */
int read_matrix(int matrix[MAX_R][MAX_R], int *row, int *col, char *filename){
	
	/* Apertura file e controllo */
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Errore nella lettura del file %s\n", filename);
		return 0;
	}
	
	/* acquisizione dimensioni rige e colonne */
	fscanf(fp, "%d %d", row, col);
	
	/* Riempimento matrice */
	for (int i = 0; i < *row; i++) {
		for (int j = 0; j < *col; j++) {
			fscanf(fp, "%d", &matrix[i][j]);
		}
	}
	
	/* Chiusura file */
	fclose(fp);
	
	return 1;
}

/* Stampa la matrice, utile per debug */
void print_matrix(int row, int col, int matrix[MAX_R][MAX_R]){
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("[%d] ", matrix[i][j]);
		}
		printf("\n");
	}
}

/*
 Controlla se la cella corrispondente agli indici i,j è
 l'indice di inizio (alto a sinistra) di una regione:
 in caso positivo restituisce 1, in caso negativo 0
 nel caso positivo inoltre, modifica i parametri base e
 altezza passati alla funzione per referenza
 */
int is_region(int matrix[MAX_R][MAX_R], int row, int col, int i, int j, int *base, int *height){
	
	/* Se non abbiamo un valore di regione */
	if (!matrix[i][j])
		return 0;
	
	/* In questo caso non sono in alto a sinistra nella regione */
	if(j && matrix[i][j - 1])
		return 0;
	
	/* Neanche in questo caso non sono in alto a sinistra nella regione */
	if(i && matrix[i - 1][j])
		return 0;
	
	/*
	 Devo utilizzare un indice diverso per j, in quanto
	 nel caso di regioni quadrate nxn devo ripartire, alla
	 riga successiva, dallo stesso indice di colonna
	 */

	int y = j;
	
	/* ciclo ogni riga per la sua lunghezza e fino a che ho valori diversi da 0 */
	for (; matrix[i][y] != 0 && i < row; i++) {
		
		/* incremento l'altezza */
		*height = *height + 1;
		
		/* Ciclo ogni colonna con lo stesso criterio delle righe, incremento la base di volta in volta */
		for (; matrix[i][y] != 0 && y < col; y++)
			*base = *base + 1;
		
		/* Riporto il contatore della colonna all'inizio regione*/
		y = j;
	}
	
	/* Metodo alternativo ad una variabile di flag per calcolare la base */
	*base = *base / *height;
	
	return 1;
	
}
