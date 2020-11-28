#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEMSTONE_NUMBER 4

const int legame_zaffiro[2] = {0, 1};
const int legame_smeraldo[2] = {2, 3};
const int legame_rubino[2] = {2, 3};
const int legame_topazio[2] = {0, 1};

const char* gem_name_s[GEMSTONE_NUMBER] = {
	"zaffiro",
	"rubino",
	"topazio",
	"smeraldo"
};

const char* gem_name_p[GEMSTONE_NUMBER] = {
	"zaffiri",
	"rubini",
	"topazi",
	"smeraldi"
};

typedef enum {
	zaffiro,
	rubino,
	topazio,
	smeraldo
} gem_type;

typedef struct {
	gem_type stone;
	gem_type links[2];
	int quantity;
}Gemstone;

typedef struct _collana Collana;

struct _collana{
	int collana[GEMSTONE_NUMBER];
	Collana* next;
};


Gemstone* get_gem(int*);

Collana* new_node(int*);
Collana* scroll_collana(Collana*);
Collana* init_collane(void);

void make_collane(Gemstone*, Collana**, gem_type, int*);
void get_max_lenght(void);
void free_list(Collana*);
void print_collane(Collana*);

int get_max(Collana*);
int* duplica_collana(int*);


int main(int argc, const char * argv[]) {
	
	/* Ho optato per fa "svolgere tutto il lavoro" alle funzioni */
	get_max_lenght();

	return 0;
}

/**
 Si occupa di generare un array di gemme che segua le specifiche date nelle costanti ad inizio programma (ho cercato di farlo il più dinamico e modulare possiblie)
 
 @param gems_number numero di gemme per ogni pietra (preso in input nel caso del nostro programma)
 @returns @b Gemstone* puntatore alla prima cella di un array di corrispondenti strutture opportunamente formattato
 
 */
Gemstone* get_gem(int* gems_number){
	
	Gemstone* gem = (Gemstone*) malloc(sizeof(Gemstone) * GEMSTONE_NUMBER);
	
	if (gem == NULL) {
		printf("Errore di allocazione memoria\n");
		return NULL;
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++) {
		gem[i].stone = i;
		gem[i].quantity = gems_number[i];
		
		switch (i) {
			case zaffiro:
				gem[i].links[0] = legame_zaffiro[0];
				gem[i].links[1] = legame_zaffiro[1];
				break;
			case rubino:
				gem[i].links[0] = legame_rubino[0];
				gem[i].links[1] = legame_rubino[1];
				break;
			case topazio:
				gem[i].links[0] = legame_topazio[0];
				gem[i].links[1] = legame_topazio[1];
				break;
			case smeraldo:
				gem[i].links[0] = legame_smeraldo[0];
				gem[i].links[1] = legame_smeraldo[1];
				break;
		}
	}
	
	return gem;
}

/**
 Si occupa di gestire la creazione di tutte le possibili soluzioni (di "implamento gemme" corretto)
 Gestisce sia l'input (@a numero_gemme) sia l'output (@a numero_massimo)
 */
void get_max_lenght(void){
	
	int* gems_number = (int*) malloc(sizeof(int) * GEMSTONE_NUMBER);
	
	if (gems_number == NULL){
		printf("Problemi di allocazione memoria");
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i ++) {
		printf("Inserire il numero di %s: ", gem_name_p[i]);
		scanf("%d", &gems_number[i]);
	}
	
	Gemstone* my_gems = get_gem(gems_number);
	
	if (gems_number == NULL){
		printf("Problemi di allocazione memoria");
		exit(EXIT_FAILURE);
	}
	
	int max = 0;
	Collana* collane = init_collane();
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++)
		make_collane(my_gems, &collane, i, &max);
	
	printf("Massima lunghezza collana componibile: %d\n\n", max);
	
	free(my_gems);
	free_list(collane);
	
}

/**
 Funzione ricorsiva che si occupa di generare una lista contenente tutte le possibli combinazioni corrette di pietre in una collana
 
 @param gemstones puntatore all'array di "informazioni" che servono alla funzione per comporre la collana
 @param collane puntatore alla testa della lista di collane @b ATTENZIONE: la lista di collane deve essere passata con la testa inizializzata a 0, si consiglia l'utilizzo della funzione @a init_collana() che si occupa proprio di realizzare una lista "vuota" correttamente
 @param gem enumerazione specificante la prima gemma della collana, ossia quella da cui la ricorsione avrà inizio.. questa funzione ricorsiva deve essere chiamata per ogni pietra disponibile, in modo da trovare delle combinazioni magari maggiori se si inizia con una pietra differente
 @param max puntatore ad un intero in cui verrà salvata, se maggiore, la lunghezza della collana finale ottenuta
 
 */
void make_collane(Gemstone* gemstones, Collana** collane, gem_type gem, int* max){
	
	Collana* current = scroll_collana(*collane);
	
	if ((current->collana[0] + current->collana[1] + current->collana[2] + current->collana[3]) > *max)
		*max = current->collana[0] + current->collana[1] + current->collana[2] + current->collana[3];
	
	
	if (gemstones[gem].quantity - current->collana[gem] == 0) {
		return ;
	}
		
	current->collana[gem] = current->collana[gem] + 1;
	
	make_collane(gemstones, collane, gemstones[gem].links[0], max);
	
	current->next = new_node(duplica_collana(current->collana));
	
	make_collane(gemstones, collane, gemstones[gem].links[1], max);
}

/**
 Duplica una collana, ossia un vettore di n (dove n è il numero di pietre) interi
 
 @param collana puntatre alla collana da duplicare
 @returns @b int* il duplicato della collana fornita alla funzione
 
 */
int* duplica_collana(int* collana){
	
	int* cpy = (int*) malloc(sizeof(int) * GEMSTONE_NUMBER);
	
	if (cpy == NULL) {
		printf("Errore di allocazione memoria\n");
		return NULL;
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++) cpy[i] = collana[i];
	
	return cpy;
}

/**
 Crea una nuova lista, fornendo il puntatore alla testa, i valori delle pietre della collana vengono tutti inizializzati a 0
 
 @returns @b Collana* puntatore alla testa della nuova collana (avrà un solo nodo)
 
 */
Collana* init_collane(){
	
	Collana* head = (Collana*) malloc(sizeof(Collana));
	
	if (head == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++)
	head->collana[i] = 0;
	
	head->next = NULL;
	
	return head;
	
}

/**
 Crea, a partire dal valore di una collana, il nodo corrispondente, per permettere di aggiungere appunto il valore "collana" ad una lista
 
 @param collana puntatore alla collana da "tradurre" in nodo
 @returns @b Collana* un nodo collana
 
 */
Collana* new_node(int* collana){
	
	Collana* my_collana = (Collana*) malloc(sizeof(Collana));
	
	if (my_collana == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++)
	my_collana->collana[i] = collana[i];
	
	my_collana->next = NULL;
	
	return my_collana;
	
}

/**
 Percorre tutta la lista e restituisce il puntatore all'ultimo elemento
 
 @param collana puntatore alla testa della lista
 @returns @b *Collana puntatore all'ultima collana della lista
 
 */
Collana* scroll_collana(Collana* collana){
	
	Collana* last = collana;
	
	if (last == NULL) return NULL;
	
	for (; last->next != NULL; last = last->next);
	
	return last;
}
/**
 Dealloca lo spazio di memoria per una lista di tipo @a Collana
 
 @param collana la testa della lista da eliminare
 */
void free_list(Collana* collana){
	for (; collana != NULL;) {
		Collana* temp = collana;
		collana = collana->next;
		free(temp);
	}
}
