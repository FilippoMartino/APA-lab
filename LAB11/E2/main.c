#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_DIAG 3
#define MAX_ELEMENTS 5
#define MIN_ELEMENTS 1
#define BOUNS 1.5
#define ERROR_MESSAGE "EMPTY"

/* Enumerazioni per rendere più comprensibili gli attributi degli elementi */
typedef enum {
	transizione,
	indietro,
	avanti
}Tipologia;

typedef enum {
	spalle,
	frontalmente
} Direzione;

/* Struttura elemento */
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

/* Strutura diagonale per facilitarmi la gestione di uno spettacolo */
typedef struct {
	Elemento elementi[MAX_ELEMENTS];
	int numero_elementi;
	int difficolta_restante;
} Diagonale;

int acquisici_elementi(char*, Elemento**);
Elemento get_first_of_row(Elemento*, int, int*, int);
Elemento get_elemento(Elemento*, Elemento, int, int*, int, int, int);

void stampa_elementi(Elemento*, int);
void print_show(Elemento*, int, int, int);

int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		printf("ERROR! PARAM %s : ELEMENTI\n", argv[0]);
		return -1;
	}
	
	Elemento* elementi;
	int numero_elementi = acquisici_elementi((char*) argv[1], &elementi);
	
	int DD, DP;
	printf("Inserire [DD] [DP]: ");
	scanf("%d %d", &DD, &DP);
	
	
	print_show(elementi, numero_elementi, DD, DP);
	
	free(elementi);
	return 0;
}


/* Semplice funzione di lettura di un file */
int acquisici_elementi(char* file_name, Elemento** el_array){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Errore durante il tentativo di lettura del file %s\n", file_name);
		return 0;
	}
	
	int elem_n;
	
	fscanf(fp, "%d", &elem_n);
	
	Elemento* current_el_array = (Elemento*) malloc(sizeof(Elemento) * elem_n);
	
	for (int i = 0; i < elem_n; i++) {
		fscanf(fp, "%s %d %d %d %d %d %f %d\n",
			   current_el_array[i].nome,
			   &current_el_array[i].tipologia,
			   &current_el_array[i].ingresso,
			   &current_el_array[i].uscita,
			   &current_el_array[i].precedenza,
			   &current_el_array[i].finale,
			   &current_el_array[i].punteggio,
			   &current_el_array[i].difficolta);
	}
	
	fclose(fp);
	
	*el_array = current_el_array;
	
	return elem_n;
	
}

/* Funzione di stampa */
void stampa_elementi(Elemento* elem_array, int n){
	
	for (int i = 0; i < n; i++)
	printf("%s ", elem_array[i].nome);
	
	printf("\n");
	
}

/**
 Cuore dell'algoritmo Greedy:
 Si occupa di generare una soluzione valida che si avvicini il più possibile alla solizione migliore.
 Ho "forzato" il Greedy in più punti, ad esempio:
 - Mantengo inizialmente una difficoltà più bassa con un vincolo alla funzione di generazione dell'elemento, per poi andarla a "liberare" dopo la seconda colonna (precisamente nel secondo
  elemento della terza diagonale, in modo da intercettare un possibile punto bouns)
 - Forzo la genrazione di elementi acrobatici (avanti/indietro) e non nelle prime due colonne in modo da cercare di soddisfare i vincoli imposti dal probema
 */
void print_show(Elemento* elem_array, int n, int DD, int DP){
	
	Elemento temp;
	
	/*
	 Necessito di questo "elemento indietro" per riciclare la funzione di generazione
	 di un elemento appetibile nel secondo elemento della seconda diagonale, in questo
	 modo forzo il greedy a comprendere anche un elemento indietro nella diagonale e
	 di conseguenza nello spettacolo.
	 Per generarla prendo il primo elemento "indietro" disponibile nel array degli elementi
	 */
	Elemento elemento_indietro;
	
	for (int i = 0; i < n; i++){
		if (elem_array[i].tipologia == indietro){
			elemento_indietro = elem_array[i];
			break;
		}
	}
	
	/* Istanzio l'array di diagonali in cui verrà salvato lo spettacolo */
	Diagonale spettacolo[MAX_DIAG];
	
	for (int i = 0; i < MAX_DIAG; i ++) {
		spettacolo[i].numero_elementi = 0;
		spettacolo[i].difficolta_restante = DD;
	}
	
	/* Generazione della soluzione greedy, riempienedo le colonne */
	for (int j = 0; j < MAX_ELEMENTS; j++) {
		
		/* Conotrollo sempre di non aver raggiunto la difficoltà massima */
		if (!DP)
			break;
		
		
		for (int i = 0; i < MAX_DIAG; i++) {
			
			/* Controllo anche qui sia la difficoltà massima che la difficoltà della singola colonna */
			if (!DP)
				break;
			
			/* Vado avanti solo se la diagonale attuale accetta ancora difficoltà */
			if (spettacolo[i].difficolta_restante) {
				
				//				printf("Diagonale #%d, diff_res: %d\n", i + 1, spettacolo[i].difficolta_restante);
				
				/* Dobbiamo anzitutto gestire il primo elemento delle colonne */
				if (!j) {
					
					/* Includo nella prima riga un elemento acrobatico, nelle seguenti no*/
					if (!i) {
						spettacolo[i].elementi[j] = get_first_of_row(elem_array, n, &DP, 1);
						spettacolo[i].numero_elementi = spettacolo[i].numero_elementi + 1;
						spettacolo[i].difficolta_restante = spettacolo[i].difficolta_restante - spettacolo[i].elementi[j].difficolta;
					} else {
						spettacolo[i].elementi[j] = get_first_of_row(elem_array, n, &DP, 0);
						spettacolo[i].numero_elementi = spettacolo[i].numero_elementi + 1;
						spettacolo[i].difficolta_restante = spettacolo[i].difficolta_restante - spettacolo[i].elementi[j].difficolta;
					}
					
					
				}
				
				/* Andiamo poi, con la seconda colonna, a chiudere i requisiti per lo spettacolo */
				else if (j == 1) {
					
					/* Unica ulteriore differenza, se mi trovo nell'ultima riga provo, se possibile, a massimizzare il punteggio */
					if (i != (MAX_DIAG - 1) && i != 1) {
						temp = get_elemento(elem_array, spettacolo[i].elementi[j - 1], n, &DP, spettacolo[i].difficolta_restante, 1, 1);
						if (strcmp(temp.nome, ERROR_MESSAGE)) {
							spettacolo[i].elementi[j] = temp;
							spettacolo[i].numero_elementi = spettacolo[i].numero_elementi + 1;
							spettacolo[i].difficolta_restante = spettacolo[i].difficolta_restante - spettacolo[i].elementi[j].difficolta;
						}
					} else if (i == 1) {
						temp = get_elemento(elem_array, elemento_indietro, n, &DP, spettacolo[i].difficolta_restante, 1, 1);
						if (strcmp(temp.nome, ERROR_MESSAGE)) {
							spettacolo[i].elementi[j] = temp;
							spettacolo[i].numero_elementi = spettacolo[i].numero_elementi + 1;
							spettacolo[i].difficolta_restante = spettacolo[i].difficolta_restante - spettacolo[i].elementi[j].difficolta;
						}
					}
					
					else {
						temp = get_elemento(elem_array, spettacolo[i].elementi[j - 1], n, &DP, spettacolo[i].difficolta_restante, 1, 0);
						if (strcmp(temp.nome, ERROR_MESSAGE)) {
							spettacolo[i].elementi[j] = temp;
							spettacolo[i].numero_elementi = spettacolo[i].numero_elementi + 1;
							spettacolo[i].difficolta_restante = spettacolo[i].difficolta_restante - spettacolo[i].elementi[j].difficolta;
						}
					}
					
				} else {
					/* Se ne abbiamo ancora la possibilità cerchiamo adesso di massimizzare il punteggio */
					temp = get_elemento(elem_array, spettacolo[i].elementi[j - 1], n, &DP, spettacolo[i].difficolta_restante, 1, 0);
					if (strcmp(temp.nome, ERROR_MESSAGE)) {
						spettacolo[i].elementi[j] = temp;
						spettacolo[i].numero_elementi = spettacolo[i].numero_elementi + 1;
						spettacolo[i].difficolta_restante = spettacolo[i].difficolta_restante - spettacolo[i].elementi[j].difficolta;
					}
				}
				
				
			}
			
			
			
		}
	}
	
	float punteggio_esibizione = 0;
	float punteggio_diagonale = 0;
	int bouns = 0;
	
	for (int i = 0; i < MAX_DIAG; i ++){
		for (int j = 0; j < spettacolo[i].numero_elementi; j++) {
			punteggio_diagonale += spettacolo[i].elementi[j].punteggio;
			if (i + 1 == MAX_DIAG && spettacolo[i].elementi[j].difficolta > 8)
				bouns = 1;
		}
		
		if (bouns) {
			punteggio_diagonale = punteggio_diagonale * BOUNS;
			bouns = 0;
		}
		
		punteggio_esibizione += punteggio_diagonale;
		punteggio_diagonale = 0;
		
	}
	
	printf("TOT = %.2f\n", punteggio_esibizione);
	for (int i = 0; i < MAX_DIAG; i ++){
		for (int j = 0; j < spettacolo[i].numero_elementi; j++) {
			punteggio_diagonale += spettacolo[i].elementi[j].punteggio;
			if (i + 1 == MAX_DIAG && spettacolo[i].elementi[j].difficolta > 8)
				bouns = 1;
		}
		if (bouns)
			printf("DIAG #%d > %.2f * %.2f (BONUS)\n", i + 1, punteggio_diagonale, BOUNS);
		else
			printf("DIAG #%d > %.2f\n", i + 1, punteggio_diagonale);
		
		stampa_elementi(spettacolo[i].elementi, spettacolo[i].numero_elementi);
		punteggio_diagonale = 0;
	}
	
}

/**
 Funzione quanto più possibile generale, gestisce la generazione di una soluzione ottima con i parametri specificati (ove possibile)
 in caso non sia stata trovata una soluzione accettabile restituisce un Elemento di default con il nome impostato su un messaggio di
 errore, sarà il client a dover controllare il risultato della funzione prima di comprendere l'elemento nella diagonale
 */
Elemento get_elemento(Elemento* elem_array, Elemento elemento_precendente, int n, int* DP, int diff_restante, int is_acrobatico, int minim_diff){
	
	Elemento best_element;
	strcpy(best_element.nome, ERROR_MESSAGE);
	best_element.punteggio = 0;
	best_element.difficolta = 100;
	
	/* Veloce controllo se stiamo cercando di inserire un elemento dopo un elemento che va messo come finale */
	if (elemento_precendente.finale)
		return best_element;
	
	for (int i = 0; i < n; i++) {
		
		/* Caso in cui l'elemento cercato sia acrobatico */
		if (is_acrobatico && elem_array[i].tipologia != transizione) {
			
			/* Cerchiamo solo tra gli elementi con la stessa tipologia */
			if ((elemento_precendente.tipologia == avanti || elemento_precendente.tipologia == transizione) && elem_array[i].tipologia == avanti) {
				
				/* Nel caso della tipologia avanti non sono necessari conotrolli su direzioni di entrata/uscita */
				
				/*
				 Ora devo decidere come gestire il risultato migliore:
				 - Nel caso si abbia il flag minim_diff devo cercare prima per la difficoltà minore, nel caso di più elementi
				 a difficoltà minore procedo a scegliere quello con il punteggio più alto (sempre che la difficoltà trovata
				 non superi la difficoltà restante
				 - Nel caso non ci si preoccupi di cercare la difficoltà minima si imposta come limite la difficoltà restante e si
				 cerca di massimizzare il punteggio
				 */
				if (minim_diff) {
					if (elem_array[i].difficolta <= diff_restante && elem_array[i].difficolta < best_element.difficolta && elem_array[i].difficolta <= *DP)
						best_element = elem_array[i];
					if (elem_array[i].difficolta == best_element.difficolta && elem_array[i].punteggio > best_element.punteggio && elem_array[i].difficolta <= *DP)
						best_element = elem_array[i];
				} else {
					if (elem_array[i].difficolta <= diff_restante && elem_array[i].punteggio > best_element.punteggio && elem_array[i].difficolta <= *DP)
						best_element = elem_array[i];
				}
				
				
			} else if (elem_array[i].tipologia == indietro && (elemento_precendente.tipologia == indietro || elemento_precendente.tipologia == transizione)){
				
				/* Controllo sulla direzione */
				if (elemento_precendente.uscita == elem_array[i].ingresso) {
					
					/* Stessi controlli fatti per la versione in avanti, sul criterio di assegnazione elemento migliore */
					if (minim_diff) {
						if (elem_array[i].difficolta <= diff_restante && elem_array[i].difficolta <= *DP && elem_array[i].difficolta < best_element.difficolta)
							best_element = elem_array[i];
						if (elem_array[i].difficolta == best_element.difficolta && elem_array[i].punteggio > best_element.punteggio && elem_array[i].difficolta <= *DP)
							best_element = elem_array[i];
					} else {
						if (elem_array[i].difficolta <= diff_restante && elem_array[i].punteggio > best_element.punteggio && elem_array[i].difficolta <= *DP)
							best_element = elem_array[i];
					}
				}
				
			}
			
			
			
		}
		
		
		
	}
	
	if (!strcmp(best_element.nome, ERROR_MESSAGE))
		return best_element;
	
	*DP = *DP - best_element.difficolta;
	
	return best_element;
}

/**
 Funzione personalizzata per la generazione della prima colonna dello spettacolo, cerco di tenere la dififcoltà più bassa possibile (discriminando
 eventualmente per punteggio), do la possibilità di scegliere se restituire un elemento acrobatico o meno
 */
Elemento get_first_of_row(Elemento* elem_array, int n, int* DP, int is_acrobatico){
	
	Elemento my_elemento;
	my_elemento.difficolta = 100;
	my_elemento.punteggio = 0;
	
	for (int i = 0; i < n; i++) {
		
		/* Trascuro gli elementi che non possono essere inseriti come primi in una diagonale */
		if (!elem_array[i].precedenza && !elem_array[i].finale && elem_array[i].ingresso) {
			
			/* Degli elementi restanti mi chiedo se voglio ottenere un elemento acrobatico o meno */
			if (is_acrobatico) {
				
				if (elem_array[i].difficolta < my_elemento.difficolta && elem_array[i].tipologia) {
					my_elemento = elem_array[i];
				}
				
				if (elem_array[i].difficolta == my_elemento.difficolta && elem_array[i].punteggio > my_elemento.punteggio  && elem_array[i].tipologia) {
					my_elemento = elem_array[i];
				}
				
			} else {
				
				if (elem_array[i].difficolta < my_elemento.difficolta && !elem_array[i].tipologia) {
					my_elemento = elem_array[i];
				}
				
				if (elem_array[i].difficolta == my_elemento.difficolta && elem_array[i].punteggio > my_elemento.punteggio  && !elem_array[i].tipologia) {
					my_elemento = elem_array[i];
				}
			}
			
		}
		
	}
	
	*DP = *DP - my_elemento.difficolta;
	
	return my_elemento;
	
}
