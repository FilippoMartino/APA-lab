//
//  main.c
//  E03
//
//  Created by Filippo Martino on 16/10/2020.
//

#define MAX_SEQUENCE 20
#define MAX_SEQUENCE_SIZE 4
#define MAX_BUFFER_SIZE 200
#define MAX_WORD_SIZE 25

/* Struttura per la gestione della singola parola positiva ad una ricorrenza */
typedef struct{
	char parola[MAX_WORD_SIZE];
	char sequence[MAX_WORD_SIZE];
	int position;
	int id;
} Word;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void compute(FILE*, int, char[MAX_SEQUENCE][MAX_SEQUENCE_SIZE]);
int is_divider(char);
void clear_buffer(int, char[MAX_BUFFER_SIZE]);
int get_buffer_size(char[MAX_BUFFER_SIZE]);
int id_number(int, int, Word[MAX_WORD_SIZE]);
void to_lower(char[MAX_SEQUENCE_SIZE]);
void print(int, int, Word [MAX_WORD_SIZE]);


int main(int argc, const char * argv[]) {
	
	int sequence_number;
	
	/* Gestione errore parametri */
	if (argc < 3){
		printf("Errore: PARAM %s SEQUENZE TESTO\n", argv[0]);
		return -1;
	}
	
	/* Apertura primo file con controllo errore */
	FILE* fp = fopen(strdup(argv[1]), "r");
	if (fp == NULL){
		printf("Errore nell'apertura del file %s", argv[1]);
		return -1;
	}
	
	/* Lettura numero di sequenze e dichiarazione matrice */
	fscanf(fp, "%d", &sequence_number);
	char sequences[sequence_number][MAX_SEQUENCE_SIZE];
	
	/* Riempimento matrice */
	for (int i = 0; i < sequence_number; i++) {
		fscanf(fp, "%s", sequences[i]);
		to_lower(sequences[i]);
	}
	
	/* Chiusura file sequenze */
	fclose(fp);
	
	/* Apertura file in cui cercare le sequenze con gestione errori */
	fp = fopen(strdup(argv[2]), "r");
	if (fp == NULL){
		printf("Errore nell'apertura del file %s", argv[2]);
		return -1;
	}
	
	/* Ricerca e stampa ricorrenze */
	compute(fp, sequence_number, sequences);
	/* Chiusura secondo file e fine programma */
	fclose(fp);
	
	return 0;
}

void compute(FILE* fp, int sequence_number, char sequences[sequence_number][MAX_SEQUENCE_SIZE]){
	
	/* Nel buffer viene di volta in volta salvata una riga del file */
	char buffer[MAX_BUFFER_SIZE];
	/* Nella variabile seguente viene di volta in volta salvata una singola parola */
	char single_world[MAX_WORD_SIZE];
	
	/* Pulisco il buffer appena inizializzato */
	clear_buffer(MAX_WORD_SIZE, single_world);
	
	int position = 0;
	int j = 0;
	int row_lenght = 0;
	int sequence_found = 0;
	
	/* Inizializzazione vettore strutture */
	Word words[MAX_BUFFER_SIZE];
	
	/* Ciclo fino alla fine del file*/
	for (; fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL;) {
		
		/* Acquisisco la lunghezza effettiva della riga (non la sappiamo a priori) */
		row_lenght = get_buffer_size(buffer);
		
		/* Ciclo per  ogni carattere nella riga */
		for (int i = 0; i < row_lenght; i ++){
			/* Se è una lettera lo aggiungo nella parola singola, dopo averla, eventualmente resa minuscola */
			if (isalpha(buffer[i])){
				single_world[j] = tolower(buffer[i]);
				j ++;
			}else {
				/* Atrimenti, se siamo tra due parole */
				if(isalpha(buffer[i + 1])){
					/* Metto il carattere di fine stringa alla singola parola */
					single_world[j + 1] = '\0';
					/* Inizializzo nuovamente l'indice della parola e aumento il numero di posizioni */
					j = 0;
					position ++;
					
					/* Ora devo controllare se una delle sequenze è contenuta nella parola appena trovata */
					for (int x = 0; x < sequence_number; x++) {
						if (strstr(single_world, sequences[x]) != NULL) {
							strcpy(words[sequence_found].parola, single_world);
							strcpy(words[sequence_found].sequence, sequences[x]);
							words[sequence_found].position = position;
							words[sequence_found].id = x;
							sequence_found ++;
						}
					}
					/* Pulisco il buffer */
					clear_buffer(MAX_WORD_SIZE, single_world);
				}
			}
		}
		/*
		 Devo ripetere l'incremento della posizione e il controllo della
		 sequenza perchè una volta uscito dal ciclo esterno mi resta ancora una
		 parola da controllare
		 */
		position ++;
		for (int x = 0; x < sequence_number; x++) {
			if (strstr(single_world, sequences[x]) != NULL) {
				strcpy(words[sequence_found].parola, single_world);
				strcpy(words[sequence_found].sequence, sequences[x]);
				words[sequence_found].position = position;
				words[sequence_found].id = x;
				sequence_found ++;
			}
		}
		clear_buffer(MAX_WORD_SIZE, single_world);
		j = 0;
	}
	
	/* Stampo le sequenze con la formattazione richiesta dalla consegna */
	print(sequence_number, sequence_found, words);
	
	
}

/* Restituisce 1 se il carattere c è inteso come "carattere divistore" */
int is_divider(char c){
	
	if (isspace(c) || ispunct(c) || c == '\n')
		return 1;
	else
		return 0;
}

/* Conta quanti caratteri ha la riga salvata nel buffer */
int get_buffer_size(char buffer[MAX_BUFFER_SIZE]){
	
	int lenght = 0;
	for (int i = 0; i < MAX_BUFFER_SIZE && buffer[i] != '\n'; i++, lenght++);
	return lenght;
	
}

/* Svuota il buffer */
void clear_buffer(int size, char buffer[size]){
	for (int i = 0; i < size; i++) {
		buffer[i] = 0;
	}
}

/* Converte tutti i caratteri di una striga in minuscole */
void to_lower(char string[MAX_SEQUENCE_SIZE]){
	for (int i = 0; i < MAX_SEQUENCE_SIZE; i++) {
		string[i] = tolower(string[i]);
	}
}

/*
 Restituisce il numero di ricorrenze trovate
 di un certo id nella lista di strutture ricorrenze.
 Si riesce così a capire quante ricorrenze per una certa
 sequenza si hanno
 */
int id_number(int n, int id, Word words[n]){
	
	int id_number = 0;
	
	for (int i = 0; i < n; i++) {
		if (words[i].id == id)
			id_number ++;
	}
	
	return id_number;
	
}

/* Stampa le sequenze trovate rispettando la formattazione richiesta dalla consegna */
void print(int sequence_number, int sequence_found, Word words[sequence_found]){
	
	/* Mi serve per gestire la stampa della prima ricorrenza trovata */
	int first = 1;
	
	/* Ciclo per tutte le n sequenze che erano da trovare */
	for (int i = 0; i < sequence_number; i ++) {
		
		/* Id mi serve per capire quante parole per data sequenza devo stampare */
		int id = 0;
		/* Serve per capire il numero di parole stampate */
		int stampato = 0;
		
		/* Ciclo per le sequenze trovate nel file (tutte, in disordine) */
		for (int x = 0; x < sequence_found; x++) {
			
			/*
			 Ad ogni ciclo voglio sapere, per un dato id (riferimento al ciclo esterno)
			 il numero di parole per la seq corrispondente
			 */
			id = id_number(sequence_found, i, words);
			
			/* Se l'id corrisponde alla sequenza che mi interessa e sono alla prima stampa */
			if (words[x].id == i && first){
				/* Formattazione per la prima stringa */
				printf("La sequenza %s è contenuta in %s (parola in posizione %d nel testo)",
					   words[x].sequence,
					   words[x].parola,
					   words[x].position);
				first = 0;
				stampato ++;
				/* Se l'id corrisponde alla seq interessata e non mi trovo ne alla prima ne all'ultima stampa */
			}else if (words[x].id == i && stampato + 1 < id){
				printf(", %s (posizione %d)",
					   words[x].parola,
					   words[x].position);
				stampato ++;
				/* Se entro nel ramo sotto significa che sono all'ultima stampa, gestisco l'output di conseguenza */
			}else if (words[x].id == i){
				printf(" e %s (posizione %d).",
					   words[x].parola,
					   words[x].position);
			}
			
		}
		/*
		 Una volta uscito dalla stampa di tutte le seq
		 trovate per un certo id, inizializzo nuvamente
		 la variabile di "prima stampa"s
		 */
		first = 1;
		printf("\n");
	}
}
