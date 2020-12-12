#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char col_1;
	int val_1;
	char col_2;
	int val_2;
	int is_used;
}Tile;

typedef struct {
	int index;
	int rotation;
} Cell;

Cell** get_table(int*, int*, Tile*, char*);

int fill_tiles(char*, Tile**);
int get_score(Cell**, int, int, Tile*);
int check(Cell**, int, int, int, int, Tile*);

void get_best_disp(Cell**, int, int, Tile*, int, int, int*, Cell**);

int main(int argc, const char * argv[]) {
	
	/* Controllo parametri in input */
	if (argc < 3) {
		printf("ERROR: PARAM %s TILES_FILE BOARD_FILE\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	Tile* my_tiles = NULL;
	int rows = 0;
	int col = 0;
	int max = 0;
	
	/* Lettura tessere e rispettivo numero da file */
	int tiles_number = fill_tiles((char*) argv[1], &my_tiles);
	
	if (!tiles_number)
		exit(EXIT_FAILURE);
	
	/* Lettura tavolo di gioco iniziale */
	Cell** my_board = get_table(&rows, &col, my_tiles, (char*) argv[2]);
	
	/* inizializzo la scacchiera finale */
	Cell**  final = (Cell**) malloc((rows) * sizeof(Cell*));
	for (int i = 0; i < rows; i++) {
			//per ogni riga crea un puntatore a un vettore di Caselle
			final[i] = (Cell*) malloc(sizeof(Cell) * col);
	}
	
	get_best_disp(my_board, rows, col, my_tiles, tiles_number, 0, &max, final);
	
	printf("Il punteggio massimo è %d\n", max);
	
	for (int i = 0; i < rows; i++) {
			for (int j = 0; j < col; j++) {
				printf("%d/%d ", final[i][j].index, final[j][j].rotation);
			}
		printf("\n");
	}
	printf("\n");
	
	return 0;
}

Cell** get_table(int* rows, int* col, Tile* tiles, char* file_name){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Errore durante tentativo apertura file %s\n", file_name);
		return NULL;
	}
	
	fscanf(fp,"%d %d", rows, col);
	
	Cell** board = (Cell**) malloc((*rows) * sizeof(Cell*));
	
	for (int i = 0; i < *rows; i++) {
		
		board[i] = (Cell*) malloc((*col) * sizeof(Cell));
		
		for (int j = 0; j < *col; j++) {
			fscanf(fp, "%d/%d", &board[i][j].index, &board[i][j].rotation);
			tiles[board[i][j].index].is_used = 1;
		}
	}
	
	fclose(fp);
	return board;
}

int fill_tiles(char* file_name, Tile** tiles){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL){
		printf("Error opening file %s\n", file_name);
		return 0;
	}
	
	int tiles_number;
	fscanf(fp, "%d\n", &tiles_number);
	
	Tile* current = (Tile*) malloc(sizeof(Tile) * tiles_number);
	
	if (current == NULL) {
		printf("Errore allocazione memoria\n");
		return 0;
	}
	
	for (int i = 0; i < tiles_number; i++)
	fscanf(fp, "%c %d %c %d\n", &current[i].col_1, &current[i].val_1, &current[i].col_2, &current[i].val_2);
	
	fclose(fp);
	
	*tiles = current;
	return tiles_number;
	
}

int get_score(Cell** board, int rows, int col, Tile* tiles){
	
	int score = 0;
	char current_color = 0;
	int is_ok = 1;
	
	/* Controllo righe */
	for (int i = 0; i < rows; i++) {
		
		/* Acquisio il coolre della riga */
		if (!board[i][0].rotation)
			current_color = tiles[board[i][0].index].col_1;
		else
			current_color = tiles[board[i][0].index].col_2;
		
		/* Controllo se tutte le righe hanno lo stesso colore, tenendo conto della rotazione */
		for(int j = 1; j < col && is_ok; j++){
			
			if(!board[i][j].rotation){
				if(tiles[board[i][j].index].col_1 != current_color){
					is_ok = 0;
				}
			}else{
				if(tiles[board[i][j].index].col_2 != current_color){
					is_ok = 0;
				}
			}
		}
		
		/* Se la riga è valida, conteggio */
		if(is_ok){
			for (int j = 0; j < col; j++) {
				
				if(!board[i][j].rotation)
					score = score + tiles[board[i][j].index].val_1;
				else
					score = score + tiles[board[i][j].index].val_2;
				
			}
		}
	}
	
	/* Passo adesso alle colonne */
	for(int j = 0, is_ok = 1; j < col; j++){
		
		/* Acquisizione colore colonna tenendo conto della possibile rotazione */
		if(!board[0][j].rotation)
			current_color = tiles[board[0][j].index].col_2;
		else
			current_color = tiles[board[0][j].index].col_1;
		
		/* Controllo in caso tutte le colonne avessero lo stesso colore */
		for(int i = 1; i < rows && is_ok; i++){
			if(!board[i][j].rotation){
				if(tiles[board[i][j].index].col_2 != current_color){
					is_ok = 0;
				}
			}else{
				if(tiles[board[i][j].index].col_1 != current_color){
					is_ok = 0;
				}
			}
		}
		
		/* Se la colonna è valida, conteggio */
		if(is_ok){
			
			for (int i = 0; i < rows; i++) {
				
				if(!board[i][j].rotation)
					score = score + tiles[board[i][j].index].val_2;
				else
					score = score + tiles[board[i][j].index].val_1;
					
			}
		}
		
	}
	
	return score;
	
}

int check(Cell** board, int rows, int col, int cur_row, int cur_col, Tile* tiles){
	
	int find_value = 0;
	int ok_row = 0;
	int ok_col = 0;
	int i = cur_row;
	int j = 0;
	Cell box;
	box.index = -1;
	box.rotation = -1;
	
	/* Ciclo sulle righe fino a quando non ho una casella significativa, che non sia quella appena aggiunta (o una vuota) */
	do{
			
		if(board[i][j].index != -1 && ((i != cur_row) || (j != cur_col))){
			box = board[i][j];
			find_value = 1;
		}
			
		j++;
		
	}while (!find_value && j < col);
	
	Tile significant_tile;
	
	/* Dalla casella creo una tessera, per farlo tengo conto della rotazione */
	if(box.rotation == 0){
		significant_tile = tiles[box.index];
	}else{
		Tile tmp = tiles[box.index];
		significant_tile.col_1 = tmp.col_2;
		significant_tile.col_2 = tmp.col_1;
		significant_tile.val_1 = tmp.val_2;
		significant_tile.val_2 = tmp.val_1;
	}
	
	/* Procedo al confronto della tessera significativa con quella appena inserita */
	Tile current_tile = tiles[board[cur_row][cur_col].index];

	if(current_tile.col_1 == significant_tile.col_1)
		ok_row = 1;
	
	/* Verifico se necessito una rotazione per far coincidere i colori */
	if(current_tile.col_2 == significant_tile.col_1){
		ok_row = 1;
		board[cur_row][cur_col].rotation = 1;
	}
	
	/* Stesso procedimento per le colonne */
	
	find_value = 0;
	i = 0;
	j = cur_col;
	
	/* Trovo una tessera significativa */
	do{
		if(board[i][j].index != -1 && ((i != cur_row) || (j != cur_col))){
			box = board[i][j];
			find_value = 1;
		}
			
		i++;
		
	}while (!find_value && i < rows);
	
	if(!box.rotation){
		significant_tile = tiles[box.index];
	}else{
		/* Gestisco la rotazione */
		Tile tmp = tiles[box.index];
		significant_tile.col_1 = tmp.col_2;
		significant_tile.col_2 = tmp.col_1;
		significant_tile.val_1 = tmp.val_2;
		significant_tile.val_2 = tmp.val_1;
		
	}
	
	/* Confronto tessera significativa con quella appena inserita, controllo se è stata ruotata per far coincidere le righe */
	
	if(!board[cur_row][cur_col].rotation){
		current_tile = tiles[board[cur_row][cur_col].index];
	}else{
		/* Gestisco la rotazione */
		Tile tmp = tiles[board[cur_row][cur_col].index];
		current_tile.col_1 = tmp.col_2;
		current_tile.col_2 = tmp.col_1;
		current_tile.val_1 = tmp.val_2;
		current_tile.val_2 = tmp.val_1;
	}
	
	/* Confronto colori */
	if(current_tile.col_2 == significant_tile.col_2)
		ok_col = 1;

	
	/* Se necessario, ruoto la tessera */
	if(current_tile.col_1 == significant_tile.col_2){
		ok_col = 1;
		board[cur_row][cur_col].rotation = 1;
	}
   
	if(ok_row || ok_col)
		return 1;
	else
		return 0;
	
}

void get_best_disp(Cell** board, int rows, int col, Tile* tiles, int tiles_number, int pos, int* max, Cell** final){
	
	/* Condizione di terminazione */
	if (pos >= (rows * col)) {
		
		/* Calcolo punteggio scacchiera corrente */
		int score = get_score(board, rows, col, tiles);
		
		/* Se necessario aggiorno il campo da gioco finale */
		
		if (score > *max) {
			*max = score;
			
			for (int i = 0; i < rows; i++)
				for (int j = 0; j < col; j++)
					final[i][j] = board[i][j];
		}
		return;
	}
	
	/* Acquisisco indice casela corrente */
	int i = pos / rows;
	int j = pos % col;
	
	/* Se la casella è già occupata procedo alla ricorsione */
	if(board[i][j].index != -1){
		get_best_disp(board, rows, col, tiles, tiles_number, pos + 1, max, final);
		return;
	}
	
	/* Se la casella è vuota */
	for (int k = 0; k < tiles_number; k++) {
		Cell box;
		box.index = k;
		box.rotation = 0;
		
		/* Controllo se posso usare la tessera */
		if(!tiles[box.index].is_used){
			board[i][j] = box;
			
			/* Controllo se la scelta è possibile, nel caso procedo con la ricorsione */
			if(check(board, rows, col, i, j, tiles)){
				tiles[box.index].is_used = 1;
				get_best_disp(board, rows, col, tiles, tiles_number, pos + 1, max, final);
			}
			/* backtrack */
			board[i][j].index = -1;
			tiles[box.index].is_used = 0;
		}
		
	}
	
}
