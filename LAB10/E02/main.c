#include <stdio.h>
#include <stdlib.h>

#define STONE_NUMBER 4
#define DEFAULT -1

char *stone_name_s[] = {"zaffiro", "rubino", "topazio", "smeraldo"};
char *stone_name_p[] = {"zaffiri", "rubini", "topazi", "smeraldi"};

/* Enumerazione per redere più leggibili le scelte delle pietre */
typedef enum {
	zaffiro,
	rubino,
	topazio,
	smeraldo
} stone;

/* Struttura pietra, definisce le specifiche sulla singola pietra */
typedef struct {
	stone gem;
	int stone_number;
} Stone;


/* (Globale) Array di puntatori in cui verranno salvate (in ultima posizione) tutte le possibili lunghezze */
int**** sol_array;

int fZ(int, int, int, int);
int fR(int, int, int, int);
int fT(int, int, int, int);
int fS(int, int, int, int);

int get_max_lenght(Stone *val);
int max(int, int);

void array_init(int, int, int, int);
void array_set_def(int, int, int, int);
void array_clear(int, int, int);

int main() {
	
	/* Dichiarazione array di pietre */
	Stone stones[STONE_NUMBER];
	
	/* Acquisizione numero pietre */
	for(int i = 0; i < STONE_NUMBER; i++) {
		printf("inserire numero %s: ", stone_name_p[i]);
		scanf("%d", &stones[i].stone_number);
		stones[i].gem = i;
	}
	
	/* Inizializzo l'array globale sol_array */
	array_init(stones[zaffiro].stone_number, stones[rubino].stone_number, stones[topazio].stone_number, stones[smeraldo].stone_number);
	
	/* Acquisisco la massima lunghezza possibile */
	printf("\nla soluzione migliore ha lunghezza %d\n", get_max_lenght(stones));
	
	return 0;
}


/**
 Nelle quattro funzioni che seguono (praticamente identiche) viene implementato il core risolutivo del problema:
 
 Ognuna di queste quattro funzioni si occupa di una specifica pietra e procede nel seguente modo:
 - controlla che esista la pietra sulla quale dovrà operare, ossia che ne siano ancora disponiibli -> prima condizione di terminazione
 - se siamo arrivati a questo punto possiamo controllare se nell'array, con le pietre disponibili e con l'intenzione di aggiungere la pietra interessata dalla funzione, abbiamo già un valore:
  in quel caso significa che in passato ci siamo già trovati ad aver generato una collana identica (a livello di pietre utilizzate) e abbiamo già aggiunto la pietra attuale e poi continuato la
  ricorsione (e quindi già trovato il valore massimo). Andiamo allora ad implementare la Memoization ossia ci limitiamo a trovare il risultato già trovato in passato -> seconda condizione di terminazione
 - Se invece non abbiamo nessun valore (identificato da un valore di default specificato dalla macro omnima) procediamo alla ricorsione sulla collana con le pietre diponibili in questo momento: chiameremo
  le due funzioni f[pietra da aggiungere 1] e f[pietra da aggiungere 1], a cui vengono passate le pietre diposnibili a questo livello della ricorsione, a cui deve essere sottratta la pietra attuale che sommiamo alla
  massima lunghezza tra le due componibili
 - Restituiamo la massima lunghezza della collana possibile con il numero di pietre passate
 
 */

int fZ(int z, int r, int t, int s){
	
	if(z == 0)
		return 0;
	
	if(sol_array[z][r][t][s] != -1)
		return sol_array[z][r][t][s];
	
	sol_array[z][r][t][s] = 1 + max(fZ(z - 1, r, t, s), fR(z - 1, r, t, s));
	
	return sol_array[z][r][t][s];
}

int fR(int z, int r, int t, int s){
	
	if(r == 0)
		return 0;
	
	if(sol_array[z][r][t][s] != -1)
		return sol_array[z][r][t][s];
	
	sol_array[z][r][t][s] = 1 + max(fS(z, r - 1, t, s), fT(z, r - 1, t, s));
	
	return sol_array[z][r][t][s];

}

int fT(int z, int r, int t, int s){
	
	if(t == 0)
		return 0;
	
	if(sol_array[z][r][t][s] != -1)
		return sol_array[z][r][t][s];
	
	sol_array[z][r][t][s] = 1 + max(fZ(z, r, t - 1, s), fR(z, r, t - 1, s));
	
	return sol_array[z][r][t][s];
	
}

int fS(int z, int r, int t, int s){
	
	if(s == 0)
		return 0;
	
	if(sol_array[z][r][t][s] != -1)
		return sol_array[z][r][t][s];
	
	sol_array[z][r][t][s] = 1 + max(fS(z, r, t, s - 1), fT(z, r, t, s - 1));
	
	return sol_array[z][r][t][s];
}

/**
 
 Questa funzione si limita a generare tutte le possibili collane provando come "prima pietra" tutte le pietre disponibili, ossia chiamando le rispettive funzioni f[pietra di inizio]
 Le collane generate vengono poi confrontate e viene restituita la collana a maggior lungheza possibile
 
 L'array di "appoggio" in cui vengono costruite le soluzioni deve essere ogni volta reinizializzato con tutte le lughezze massime al valore di default, viene dunque chiamata la rispettiva funzione
 
 */
int get_max_lenght(Stone* collane){
	
	int max = fZ(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	array_set_def(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	int tmp = fT(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	array_set_def(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	if(tmp > max)
		max = tmp;

	tmp = fR(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	array_set_def(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	if(tmp > max)
		max=tmp;
	
	tmp = fS(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number, collane[smeraldo].stone_number);
	
	if(tmp > max)
		max=tmp;
	
	array_clear(collane[zaffiro].stone_number, collane[rubino].stone_number, collane[topazio].stone_number);
	
	return max;
}

int max(int n_1, int n_2) {
	
	if (n_1 > n_2)
		return n_1;
	else
		return n_2;
	
}

/**
 Inizializzazione array in cui verranno costrutite le massime lunghezze, viene inizializzato al valore di default specificato dall omonima macro
 */
void array_init(int z, int r, int t, int s){
	
	sol_array = (int ****) malloc(sizeof(int ***) * (1 + z));
	for (int i = 0; i <= z; i++) {
		sol_array[i] = (int ***) malloc(sizeof(int **) * (1 + r));
		for (int j = 0; j <= r; j++) {
			sol_array[i][j] = (int **) malloc(sizeof(int *) * (1 + t));
			for (int k = 0; k <= t; k++) {
				sol_array[i][j][k] = (int *) malloc(sizeof(int) * (1 + s));
				for (int h = 0; h <= s; h++) {
					sol_array[i][j][k][h] = DEFAULT;
				}
			}
		}
	}
}

/**
 Funzione necessaria per una "rinizializzazione" dell'array senza allocarne uno nuovo, si limita a resettare tutti le lunghezze al valore di default
 È molto probabile che molte delle lunghezze siano comunque rimaste al valore di default, perchè durante la "generazione" delle lunghezze non vengono
 popolate tutte le combinazioni possibili, siccome ad una pietra ne possono seguire solamente altre due specifiche
 */
void array_set_def(int z, int r, int t, int s){
	for (int i = 0; i <= z; i++) {
		for (int j = 0; j <= r; j++) {
			for (int k = 0; k <= t; k++) {
				for (int h = 0; h <= s; h++){
					sol_array[i][j][k][h] = DEFAULT;
				}
			}
		}
	}
}

/**
 Dealloca l'array
 */
void array_clear(int z, int r, int t) {
	
	for (int i = 0; i <= z; i++) {
		for (int j = 0; j <= r; j++) {
			for (int k = 0; k <= t; k++) {
				free(sol_array[i][j][k]);
			}
			free(sol_array[i][j]);
		}
		free(sol_array[i]);
	}
	free(sol_array);
}
