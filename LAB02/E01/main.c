//
//  main.c
//  E01
//
//  Created by Filippo Martino on 13/10/2020.
//

#define MAX_SIZE 50

#include <stdio.h>
#include <string.h>

/* Struttura contenente le informazioni per ogni regione */
typedef struct {
	int base;
	int height;
	int area;
	char position[MAX_SIZE];
} Region;


int label(int, int, int[MAX_SIZE][MAX_SIZE]);

void fill(int, int, int[MAX_SIZE][MAX_SIZE], int, Region[MAX_SIZE]);
void print_max(int, Region[MAX_SIZE]);
void print_region(Region);
void print_matrix(int, int, int[MAX_SIZE][MAX_SIZE]);

int main(int argc, const char * argv[]) {
	
	/* Gestione errore parametri */
	if (argc < 2){
		printf("Errore: PARAM %s FILE_NAME\n", argv[0]);
		return -1;
	}
	
	/* Apertura file con gestione errore */
	FILE* fp = fopen(strdup(argv[1]), "r");
	if (fp == NULL){
		printf("Errore all'apertura del file\n");
		return -1;
	}
	
	/* Lettura grandezza matrice con gestione overflow */
	int row = 0, col = 0;
	fscanf(fp, "%d %d", &row, &col);
	if((row > MAX_SIZE) || (col > MAX_SIZE)){
		printf("Errore, dimensioni matrice superiori al limite [50]x[50]\n");
		return -1;
	}
	
	/* Dichiarazione matrice con dimensioni appropriate */
	int matrix[row][col];
	
	/* Riempio matrice e chido file */
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			fscanf(fp, "%d", &matrix[i][j]);
		}
	}
	fclose(fp);
	
	/* Assegno etichette diverse per ogni regione e ne salvo il numero in una variabile */
	int region_number = label(row, col, matrix);
	
	/* Definisco la struttura per le n regioni della matrice */
	Region regions[region_number];
	/* Divido le varie regioni e le inserisco nella struttura */
	fill(row, col, matrix, region_number, regions);
	/* Stampo i rispettivi massimi, come da consegna */
	print_max(region_number, regions);
	
	
	return 0;
}

/* Stampa una matrice */
void print_matrix(int row, int col, int matrix[row][col]){
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/*
 Va ad etichettare le varie regioni dando un numero ad ognuna,
 in questo modo sono facilmente individuabili.
 Restituisce il numero di regioni presenti nella matrice.
 */
int label(int row, int col, int matrix[row][col]){
	
	/* Copio matrice */
	int cpy_matrix[row][col];
	/* Parto da 1 */
	int label = 1;
	/* Controllo per evitare sostituzioni inutili */
	int is_labeled = 0;
	
	/* Riempio martice copia */
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cpy_matrix[i][j] = matrix[i][j];
		}
	}
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			
			/* Se ho uno zero non vado avanti */
			if(cpy_matrix[i][j]){
				
				/* Se i = 0, non controllo elementi a NORD, mi limito ad etichettare */
				if (i == 0){
					matrix[i][j] = label;
					is_labeled = 1;
				}
				
				/*
				 Se l'elemento a NORD è uguale zero, etichetto
				 con il nuovo numero (e segno che l'ho fatto),
				 altrimenti mi limito a copiare il valore, appunto, a NORD
				 */
				if (!cpy_matrix[i - 1][j]){
					matrix[i][j] = label;
					is_labeled = 1;
				}else if (i != 0){
					matrix[i][j] = matrix[i - 1][j];
				}
				
				/*
				 se mi trovo ad aver appena etichettato una regione e alla mia
				 destra ho uno 0, allora mi incremento il valore dell'etichetta
				 che sarà da assegnare alla prossima regione
				 */
				if ((matrix[i][j] == label) && !matrix[i][j + 1] && is_labeled){
					label++;
					is_labeled = 0;
				}
			}
		}
	}
	
	/*
	 Se l'etichetta, eventualmente incrementata, non
	 fosse stata usata, devo decrementarre di uno il valore
	 di ritorno, altrmenti lascio invariato
	 */
	if  (!is_labeled)
		label --;
	
	return label;
	
}

/* Formatta la struttura delle regioni */
void fill(int row,
		  int col,
		  int matrix[row][col],
		  int region_number,
		  Region regions[region_number]){
	
	int in_row = 0;
	int row_counter = 0;
	int pos = 0;
	
	/*
	 Cicli annidiati semplici, uniche particolarità
	 sui controlli:
	 - la posizione la salvo solo per la prima ricorrenza controllando
	   la variabile pos
	 - l'altezza la salvo dopo i cicli contando il numero di righe in cui
	   appaiono ricorrenze
	 - la base la salvo facendo l'area diviso l'altezza, in questo modo riesco
	   a risparmiare ulteriori controlli
	 */
	for (int x = 0; x < region_number; x++) {
		regions[x].area = 0;
		regions[x].base = 0;
		for (int i = 0; i < row; i ++) {
			for (int j = 0; j < col; j++) {
				if (matrix[i][j] == (x + 1)){
					if(!pos){
						sprintf(regions[x].position,
								"estr. sup. SX=<%d,%d>",
								i,
								j);
						pos = 1;
					}
					regions[x].area ++;
					in_row = 1;
				}
			}
			if (in_row){
				row_counter ++;
				in_row = 0;
			}
		}
		pos = 0;
		regions[x].height = row_counter;
		row_counter = 0;
		regions[x].base = regions[x].area / regions[x].height;
	}
	
}

/* Produce un output come richiesto da consegna */
void print_max(int region_number, Region regions[region_number]){
	
	/* Dichiaro una variabile di appoggio per stampare i relativi massimi*/
	Region max_region;
	max_region.base = 0;
	max_region.area = 0;
	max_region.height = 0;
	
	/* cerco e stampo quella con la base maggiore */
	for (int i = 0; i < region_number; i++) {
		if (max_region.base < regions[i].base)
			max_region = regions[i];
	}
	
	printf("Max Base: ");
	print_region(max_region);
	
	/* Cerco e stampo quella con l'area maggiore */
	for (int i = 0; i < region_number; i++) {
		if (max_region.area < regions[i].area)
			max_region = regions[i];
	}
	
	printf("Max Area: ");
	print_region(max_region);
	
	/* Cerco e stampo quella con l'altezza maggiore */
	for (int i = 0; i < region_number; i++) {
		if (max_region.height < regions[i].height)
			max_region = regions[i];
	}
	
	printf("Max Altezza: ");
	print_region(max_region);
	
	
}

/* Stampa (con la formattazione indivata dalla consegna) una singola regione */
void print_region(Region region){

	printf("%s b=%d, h=%d, Area=%d\n",
		   region.position,
		   region.base,
		   region.height,
		   region.area
		   );
	
}

