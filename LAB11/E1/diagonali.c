#include "diagonali.h"
#include <math.h>

#define MAX_DIAG_SIZE 5
#define MAX_DIAG 3
#define BONUS_VALUE 1.5

int acquisici_elementi(char*, Elemento** elem_array);
void stampa_elementi(Elemento* elem_array, int n);
void printPowerSet(char *set, int set_size);
Diagonale* getDiag(Elemento* set, int set_size, int DD, int* sol_size);
void printBestShow(Diagonale* diag_array, int diag_array_size, int DP);
int diag_cmp(Diagonale diag_1, Diagonale diag_2);
int diag_ok(Diagonale diag, int DD);
int esibizione_ok(Diagonale* diag_array, int DP);
int is_diag_present(Diagonale* pow, int n, Diagonale diag);
int disp_rip(int pos, Diagonale* val, Diagonale* sol, int n, int k, int count, Diagonale** sol_array, int* sol_array_index, int DP);

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

void stampa_elementi(Elemento* elem_array, int n){
	
	for (int i = 0; i < n; i++)
	//	printf("%s %d %d %d %d %d %.2f %d\n",
	//		   elem_array[i].nome,
	//		   elem_array[i].tipologia,
	//		   elem_array[i].ingresso,
	//		   elem_array[i].uscita,
	//		   elem_array[i].precedenza,
	//		   elem_array[i].finale,
	//		   elem_array[i].punteggio,
	//		   elem_array[i].difficolta);
	printf("%s ", elem_array[i].nome);
}
/**
 Richiede il set di elementi che si hanno a disposizione, la dimensione dello stesso, la massima difficoltà della diagonale e, passata
 per referenza in modo da poter essere aggiornata, la dimensione dello spazio delle soluzioni generato
 
 Genera un array dinamico di diagonali "acettabili" e lo restitusice
 */
Diagonale* getDiag(Elemento* set, int set_size, int DD, int* sol_size){
	
	/* Ampiezza spazio insieme delle parti */
	unsigned int pow_set_size = pow(2, set_size);
	
	/* Dichiarazione della singola diagonale in cui salvare di volta in volta le partizioni degli elementi */
	Diagonale single_diag;
	single_diag.elementi = (Elemento*) malloc(sizeof(Elemento) * MAX_DIAG_SIZE);
	
	/* Dichiarazione array, per adesso considerato come singolo puntatore, che conterrà l'insieme effettivo delle parti accetabili */
	Diagonale* pow_array = (Diagonale*) malloc(sizeof(Diagonale));
	
	int counter = 0, pow_size = 0, i, j;
	
	for(i = 0; i < pow_set_size; i++) {
		
		/* Ciclo di generazione singola diagonale */
		for(j = 0; j < set_size; j++) {
			/* L'operatore di bitwise mi permette di gestire le distribuzione delle parti sfruttando i byte */
			if(i & (1<<j)){
				
				/* Controllo per limitare la dimensione delle soluzioni generate */
				if (counter < MAX_DIAG_SIZE) {
					
					/* Aggiornamento diagonale */
					single_diag.elementi[counter] = set[j];
					counter ++;
					single_diag.numero_elementi = counter;
					
				}
			}
		}
		
		/* Controllo se la diagonale è accettabile, controllo inoltre che la diagonale non sia già presente nello spazio delle soluzioni */
		if (counter && diag_ok(single_diag, DD) && !is_diag_present(pow_array, pow_size, single_diag)) {
		
				/* Nel caso si fosse già aggiunto il primo elemento, procedo ad "allargare" l'array di diagonali */
				if (pow_size)
					pow_array = (Diagonale*) realloc(pow_array, sizeof(Diagonale) * (pow_size + 1));
				
				/* Alloco il giusto spazio per gli elementi della singola diagonale */
				pow_array[pow_size].elementi = (Elemento*) malloc(sizeof(Elemento) * single_diag.numero_elementi);
				
				/* Copio i valori corrispondenti */
				for (int k = 0; k < single_diag.numero_elementi; k++)
					pow_array[pow_size].elementi[k] = single_diag.elementi[k];
			
				pow_array[pow_size].numero_elementi = single_diag.numero_elementi;
				
				/* Incremento lo spazio delle soluzioni */
				pow_size++;
			
		}
		
		counter = 0;
		
	}
	
	/* Dealloco lo spazio usato per la singola diagonale */
	free(single_diag.elementi);
	
	*sol_size = pow_size;
	return pow_array;
}

/**
 Confronta due diagonali, se sono uguali ritorna 1, se diverse 0s
 */
int diag_cmp(Diagonale diag_1, Diagonale diag_2){
	
	if (diag_1.numero_elementi != diag_2.numero_elementi)
		return 0;
	
	for (int i = 0; i < diag_1.numero_elementi; i++)
		if (strcmp(diag_1.elementi[i].nome, diag_2.elementi[i].nome))
			return 0;
		
		return 1;
}

/**
 Dato lo spazio delle soluzioni (un array di diagonali) e la sua dimensione n controlla se
 la diagonale specificata è presente, nel caso restituisce 1, altrimenti 0
 */
int is_diag_present(Diagonale* pow, int n, Diagonale diag){
	
	for (int i = 0; i < n; i++)
		if (diag_cmp(diag, pow[i]))
			return 1;
	
	return 0;
	
}

/**
 Gestisce i controlli sulla singola diagonale, specificati nel testo dell'esercizio
 */
int diag_ok(Diagonale diag, int DD){
	
	int sum = 0;
	int n = diag.numero_elementi;
		
	/* Controllo difficoltà diagonale */
	for (int i = 0; i < n; i++)
		sum += diag.elementi[i].difficolta;
	if (sum > DD)
		return 0;
	
	/* Controllo che un elemento da eseguire come ultimo non si trovi prima di altri */
	for (int i = 0; i < n; i++)
		if (diag.elementi[i].finale && ((i + 1) != n))
			return 0;
	
	/* Controllo requisito di precedenza */
	for (int i = 0; i < n; i++)
		if (diag.elementi[i].precedenza && i == 0)
			return 0;
	
	/* Controllo che il ginnasta inizi la diagonale frontalmente */
	if (!diag.elementi[0].ingresso)
		return 0;
	
	/* Devo controllare che la sequenza di due elementi sia valida */
	for (int i = 0; i < n; i++){
		if (i + 1 != n) {
			if (diag.elementi[i].uscita != diag.elementi[i + 1].ingresso) {
				return 0;
			}
		}
	}
		
	/* Controllo presenza di elemento acrobatico come singolo */
	int elemento_acrobatico = 0;
	for (int i = 0; i < n; i++)
		if (diag.elementi[i].tipologia == avanti || diag.elementi[i].tipologia == indietro)
			elemento_acrobatico = 1;
	
	if (!elemento_acrobatico)
		return 0;

	
	
	return 1;
}

int esibizione_ok(Diagonale* diag_array, int DP){
	
	/* Controllo che difficoltà totale non superi difficolta permessa */
	int sum = 0;
	
	for (int i = 0; i < MAX_DIAG; i++)
		for (int j = 0; j < diag_array[i].numero_elementi; j++)
			sum += diag_array[i].elementi[j].difficolta;
		
	if (sum > DP)
		return 0;
	
	/* Controllo che almeno una diagonale presenti due elementi in sequenza*/
	int test_elementi = 0;
	for (int i = 0; i < MAX_DIAG; i++)
		for (int j = 0; j < diag_array[i].numero_elementi; j++)
			if (diag_array[i].numero_elementi > 1 && j != diag_array[i].numero_elementi -1)
				if (diag_array[i].elementi[j].tipologia != transizione && diag_array[i].elementi[j + 1].tipologia != transizione)
					if (diag_array[i].elementi[j].uscita == diag_array[i].elementi[j + 1].ingresso)
						test_elementi = 1;
				
	if (!test_elementi)
		return 0;
	
	/* Controllo che lo spettacolo includa un elemento acrobatico in avanti ed uno indietro nell'esibizione */
	int avanti = 0;
	int indietro = 0;
	for (int i = 0; i < MAX_DIAG; i++){
		for (int j = 0; j < diag_array[i].numero_elementi; j++){
			if (diag_array[i].elementi[j].tipologia == avanti){
				avanti++;
			}
			if (diag_array[i].elementi[j].tipologia == indietro){
				indietro ++;
			}
		}
	}
	
	if ((avanti + indietro) < 2)
		return 0;
	
	return 1;
	
}

int disp_rip(int pos, Diagonale* val, Diagonale* sol, int n, int k, int count, Diagonale** sol_array, int* sol_array_index, int DP){
	
	int i;
	
	if (pos >= k) {
		
		if(!esibizione_ok(sol, DP))
			return count + 1;
		
		for (i = 0; i < k; i++) {
			sol_array[*sol_array_index][i] = sol[i];
		}
		*sol_array_index = *sol_array_index + 1;
		return count + 1;
	}
	
	for (i = 0; i < n; i++) {
		sol[pos] = val[i];
		count = disp_rip(pos + 1, val, sol, n, k, count, sol_array, sol_array_index, DP);
	}
	
	return count;
}

void printBestShow(Diagonale* diag_array, int diag_array_size, int DP){
	int index = 0;
	
	Diagonale* sol = (Diagonale*) malloc(sizeof(Diagonale) * 3);
	for (int i = 0; i < MAX_DIAG; i++)
		sol[i].elementi = (Elemento*) malloc(sizeof(Elemento) * 5);
	
	int sol_array_size = pow(diag_array_size, MAX_DIAG);
	
	Diagonale** sol_array = (Diagonale**) malloc(sizeof(Diagonale*) * sol_array_size);
	for (int i = 0; i < sol_array_size; i++){
		sol_array[i] = (Diagonale*) malloc(sizeof(Diagonale) * MAX_DIAG);
		for (int j = 0; j < MAX_DIAG; j++) {
			sol_array[i][j].elementi = (Elemento*) malloc(sizeof(Elemento) * MAX_DIAG_SIZE);
		}
	}
	
	disp_rip(0, diag_array, sol, diag_array_size, MAX_DIAG, 0, sol_array, &index, DP);
	
	Diagonale* best_show = NULL;
	
	float totale = 0;
	float temp = 0;
	float points = 0;
	
	int bonus = 0;
	
	for (int i = 0; i < index; i++){
		
		for (int j = 0; j < MAX_DIAG; j ++){
			for (int x = 0; x < sol_array[i][j].numero_elementi ; x++){
				if (sol_array[i][j].elementi[x].difficolta >= 8 && (j + 1) == MAX_DIAG) {
					bonus = 1;
				}
				temp += sol_array[i][j].elementi[x].punteggio;
			}
			if (bonus) {
				bonus = 0;
				temp *= BONUS_VALUE;
			}
			points += temp;
			temp = 0;
		}
	
		if (totale < points){
			best_show = sol_array[i];
			totale = points;
		}
		
		points = 0;
		
	}
	
	
	printf("TOT = %.2f\n", totale);
	for (int i = 0; i < MAX_DIAG; i++){
		float punteggio = 0;
		for (int j = 0; j < best_show[i].numero_elementi; j++)
			punteggio += best_show[i].elementi[j].punteggio;
		
		printf("Diag #%d > %.2f\n", i + 1, punteggio);
		stampa_elementi(best_show[i].elementi, best_show[i].numero_elementi);
		printf("\n");
		
	}
	
	free(sol);
	
	for (int i = 0; i < sol_array_size; i++)
		free(sol_array[i]);
	free(sol_array);
	
	
}


void stampa_soluzione_ottima(char* file_name, int DD, int DP){
	
	Elemento* elem_array;
	Diagonale* diag_array;
	int diag_array_size;
	
	int elem_number = acquisici_elementi(file_name, &elem_array);
	
	if (!elem_number)
		return;
	
	diag_array = getDiag(elem_array, elem_number, DD, &diag_array_size);
	
	printBestShow(diag_array, diag_array_size, DP);
	
	for (int i = 0; i < diag_array_size; i++)
		free(diag_array[i].elementi);
	
	free(diag_array);
	
	
	
}


