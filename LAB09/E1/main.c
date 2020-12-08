#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struttura "core" dell'esrcizio */
typedef struct {
	int start;
	int end;
	int d;
} Att;

/*
 Wrapper struttura core, aggiunge informazioni utili
 quali il numero di strutture allocate e il tempo totale
 */
typedef struct {
	Att* v;
	int v_lenght;
	int total_d;
} Att_table;

/* Permette la realizzazione della lista */
typedef struct _att_list Att_list;

/* Definizione lista */
struct _att_list {
	Att_table* table;
	Att_list* next;
};

Att_list* init_att(Att*, int);
Att_list* scroll_att_list(Att_list*);
Att_list* dup_att_list(Att_list*);

Att_table* init_att_table(Att*, int);
Att_table* dup_att_table(Att_table*);
Att_table* get_max(Att_list*);

Att* dup_att(Att*, int);

int fill_att(char*, Att**);
int is_compatible(Att, Att);

void attSel(int, Att*);
void build_att_list(Att_list*, Att_table*);
void print_table(Att_table*);

int main(int argc, const char * argv[]) {
	
	Att* my_att;
	
	/* Controllo file input */
	if (argc < 2) {
		printf("Error: PARAM %s FILENAME\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* Acquisizione attese e numero di attese da file */
	int att_number = fill_att((char *) argv[1], &my_att);

	/* Stampa combinaizone migliore */
	attSel(att_number, my_att);
	
	return 0;
}

/**
 Permette l'inizializzazione di una lista
 
 @param v puntatore al vettore si strutture Att
 @param v_lenght numero di strutture Att allocate e puntate da v
 @returns @b Att_list* il putatore alla testa di una "lista pulita"
 @returns @b NULL se ci sono stati problemi
 */
Att_list* init_att(Att* v, int v_lenght){
	
	Att_table* att_table = (Att_table*) malloc(sizeof(Att_table));
	
	if (att_table == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	att_table->v_lenght = v_lenght;
	
	int tot_d = 0;
	for (int i = 0; i < v_lenght; i++) {
		tot_d += v[i].d;
	}
	
	att_table->total_d = tot_d;
	att_table->v = dup_att(v, v_lenght);
	
	Att_list* dup = (Att_list*) malloc(sizeof(Att_list));
	
	if (dup == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	dup->table = att_table;
	dup->next = NULL;
	
	return dup;
	
}

/**
 Scorre tutta la lista passata, restituisce il puntatore all'ultimo nodo
 
 @param att_list la testa della lista che vogliamo scorrere
 @returns @b Att_list* puntatore all'ultimo nodo della lista
 @returns @b NULL se ci sono stati problemi
 */
Att_list* scroll_att_list(Att_list* att_list){
	
	/* Caso lista vuota */
	if (att_list == NULL)
		return NULL;
	
	/* Caso lista con un solo nodo */
	if (att_list->next == NULL)
		return att_list;
	
	Att_list* current = att_list;
	
	/* Scorro fino all'ultimo nodo */
	for (; current->next != NULL; current = current->next);
	
	return current;
	
}

/**
 Supplica una lista
 
 @param att_list lista da duplicare
 @returns @b Att_list* puntatore alla nuova lista duplicata
 @returns @b NULL se ci sono stati problemi
 */
Att_list* dup_att_list(Att_list* att_list){
	
	Att_list* new_node = (Att_list*) malloc(sizeof(Att_list));
	
	if (new_node == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	new_node->table = dup_att_table(att_list->table);
	new_node->next = NULL;
	
	return new_node;
	
}

/**
 Inizalizza una struttura tabella
 
 @param v puntatore a una o più strutture Att
 @param v_lenght numero di strutture Att
 @returns @b Att_table* il puntatore ad una struttura tabella inizializzata
 @returns @b NULL se ci sono stati problemi
 */
Att_table* init_att_table(Att* v, int v_lenght){
	
	Att_table* dup = (Att_table*) malloc(sizeof(Att_table));
	
	if (dup == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	int total_d = 0;
	dup->v_lenght = v_lenght;
	dup->v = dup_att(v, v_lenght);
	
	for (int i = 0; i < v_lenght; i++) total_d += v[i].d;
	
	dup->total_d = total_d;
	
	return dup;
	
}

/**
 Duplica una struttura tabella
 
 @param att_table puntatore alla tabella da duplicare
 @returns @b Att_table* puntatore a tabella duplicata
 @returns @b NULL se ci sono stati problemi
 */
Att_table* dup_att_table(Att_table* att_table){
	
	Att_table* duplicate = (Att_table*) malloc(sizeof(Att_table));
	
	if (duplicate == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	duplicate->v = dup_att(att_table->v, att_table->v_lenght);
	duplicate->v_lenght = att_table->v_lenght;
	duplicate->total_d = att_table->total_d;
	
	return duplicate;
	
}

/**
 Duplica una struttura Att
 
 @param att struttura da duplicare
 @param att_number numero di strutture att da duplicare
 @returns @b Att* puntatore alla prima struttura duplicata
 @returns @b NULL se ci sono stati problemi
 */
Att* dup_att(Att* att, int att_number){
	
	if (att_number == 0) {
		printf("Errore: Numero attese uguale a 0\n");
		return NULL;
	}
	
	Att* dup = (Att*) malloc(sizeof(Att) * att_number);
	
	for (int i = 0; i < att_number ; i++) {
		dup[i].start = att[i].start;
		dup[i].end = att[i].end;
		dup[i].d = att[i].d;
	}
	
	return dup;
	
}

/**
 Legge da file e alloca spazio al puntatore specificato dal chiamante per
 le attese lette
 
 @param file_name nome del file dal quale leggere le attese
 @param my_att puntatore a puntatore attese (vuoto)
 @returns @b int numero di attese lette
 @return 0 se ci sono stati problemi
 */
int fill_att(char* file_name, Att** my_att){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Error opening file %s\n", file_name);
		return 0;
	}
	
	int att_number;
	fscanf(fp, "%d\n", &att_number);
	
	Att* att = (Att*) malloc(sizeof(Att) * att_number);
	
	for (int i = 0; i < att_number; i++) {
		fscanf(fp, "%d %d\n", &att[i].start, &att[i].end);
		att[i].d = att[i].end - att[i].start;
	}
	
	*my_att = att;
	
	fclose(fp);
	
	return att_number;
	
}

/**
 Confronta due attese
 
 @param att_1 prima struttura attesa da confrontare
 @param att_2 seconda struttura attesa da confrontare
 @returns @b 1 se la struttura att_2 può essere inserita dopo att_1
 @returns @b 0 in caso contrario
 */
int is_compatible(Att att_1, Att att_2){
	
	if (att_1.end <= att_2.start)
		return 1;
	else
		return 0;
	
}

/**
 Funzioe wrapper, inizializza tabella e liste, in modo che il chiamante non si debba preoccupare di
 "capire" il funzionamento delle strutture.
 Richiama per N volte la funzione di generazione delle combinazioni possibili, in modo da
 averne per tuttu i tempi di attesa
 
 @param N numero di strutture Att
 @param v puntatore a prima struttura Att
 
 */
void attSel(int N, Att* v){
	
	Att_table* my_table = NULL;
	int max = 0;
	
	for (int i = 0; i < N; i++) {
		
		Att_list* my_att_list = init_att(&v[i], 1);
		Att_table* my_att_table = init_att_table(v, N);
		build_att_list(my_att_list, my_att_table);
		Att_table* max_table = get_max(my_att_list);
		
		if (max_table->total_d > max) {
			max = max_table->total_d;
			my_table = max_table;
		}
	}
	
	printf("Uno dei possibili insiemi di attività che massimizza la somma: ");
	print_table(my_table);
	
}

/**
 Funzione che si occupa della generazione delle possilbi combinazioni corrette
 
 @param head puntatore alla testa della lista in cui verranno inserite tutte le combinazioni possibli linkate alla prima trvata nella info_table
 @param info_table tabella "informativa" in cui sono contenute tutte le "tempistiche"
 */
void build_att_list(Att_list* head, Att_table* info_table){
	
	Att_list* current = scroll_att_list(head);
	
	for (int i = 0; i < info_table->v_lenght; i++) {
		
		Att_list* next = dup_att_list(current);
		
		if (is_compatible(current->table->v[current->table->v_lenght - 1], info_table->v[i])) {
			
			current->table->v_lenght = current->table->v_lenght + 1;
			current->table->total_d = current->table->total_d + info_table->v[i].d;
			
			current->table->v = realloc(current->table->v, sizeof(Att) * current->table->v_lenght);
			current->table->v[current->table->v_lenght - 1] = *dup_att(&info_table->v[i], 1);
			
			/*
			 Ho lasciato il seguente codice perchè ho pensato che potesse rendere più agevole
			 la correzzione, se i commenti vengono infatti rimossi e si esegue, verranno stampate
			 tutte le combinazioni "legali" che possono essere generate con i dati letti dal file.
			 Tutte le righe sono nodi "tabella" della lista che successivamente consulterò per estrarre
			 il valore migliore
			 */
			
//			printf("current: ");
//			for (int j = 0; j < current->table->v_lenght; j++)
//				printf("(%d,%d) ", current->table->v[j].start, current->table->v[j].end);
//
//			printf("per una somma delle durate pari a: %d\n", current->table->total_d);
			
			build_att_list(current, info_table);
			
			current->next = next;
			current = current->next;
			
		}
		
	}
	
}

/**
 Restituisce la tabella massimizzante il tempo
 
 @param head testa della lista dalla quale estrarre l'informazione
 @returns @b Att_table* puntatore alla tabella massimizzante il tempo
 */
Att_table* get_max(Att_list* head){
	
	int max_d = 0;
	Att_table* max_table = NULL;
	
	for (; head != NULL; head = head->next)
	if (head->table->total_d > max_d){
		max_d = head->table->total_d;
		max_table = dup_att_table(head->table);
	}
			
		
	if (!max_d)
		return NULL;
	else
		return max_table;
	
	
}

/**
 Stampa una tabella
 
 @param table puntatore alla tabella da stampare
 */
void print_table(Att_table* table){
	
	for (int j = 0; j < table->v_lenght; j++)
		printf("(%d,%d) ", table->v[j].start, table->v[j].end);
	
	printf("per una somma delle durate pari a: %d\n", table->total_d);
}
