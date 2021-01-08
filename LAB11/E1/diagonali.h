#ifndef diagonali_h
#define diagonali_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef enum {
	transizione,
	indietro,
	avanti
}Tipologia;

typedef enum {
	spalle,
	frontalmente
} Direzione;

typedef struct {
	char nome[MAX_SIZE];
	Tipologia tipologia;
	Direzione ingresso;
	Direzione uscita;
	int precedenza; /* if == 1 deve essere preceduto da un elemento */
	int finale; /* if == 1 deve essere l'ultimo elemento di una diagonale */
	float punteggio;
	int difficolta;
}Elemento;

typedef struct {
	Elemento* elementi;
	int numero_elementi;
} Diagonale;

void stampa_soluzione_ottima(char* file_name, int DD, int DP);


#endif /* diagonali_h */
