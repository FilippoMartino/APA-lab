#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEMSTONE_NUMBER 4

#define ZAFFIRO 'z'
#define RUBINO 'r'
#define TOPAZIO 't'
#define SMERALDO 's'

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
	int value;
}Gemstone;

typedef struct _collana Collana;

struct _collana{
	int collana[GEMSTONE_NUMBER];
	char* composizione;
	int pietre_totali;
	int valore_totale;
	Collana* next;
};


Gemstone* get_gem(int*);

Collana* dup_node(Collana*);
Collana* scroll_collana(Collana*);
Collana* init_collane(void);

void make_collane(Gemstone*, Collana**, gem_type, int, Collana*);
void get_max_lenght(void);
void free_list(Collana*);
void print_collane(Collana*);

int get_max(Collana*);
int check_rep(char*, int);
int check_z_s(char*);

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
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++) {
		printf("Inserire il numero di %s: ", gem_name_p[i]);
		scanf("%d", &gems_number[i]);
	}
	
//	gems_number[0] = 4;
//	gems_number[1] = 8;
//	gems_number[2] = 2;
//	gems_number[3] = 10;
	
	Gemstone* my_gems = get_gem(gems_number);
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++) {
		printf("Inserire il valore della pietra %s: ", gem_name_s[i]);
		scanf("%d", &my_gems[i].value);
	}
	
//	my_gems[0].value = 13;
//	my_gems[1].value = 1;
//	my_gems[2].value = 8;
//	my_gems[3].value = 23;
	
	if (gems_number == NULL){
		printf("Problemi di allocazione memoria");
		exit(EXIT_FAILURE);
	}
	
	int max_rep;
	
	printf("Inserire numero massimo di ripetizioni: ");
	scanf("%d", &max_rep);
	
//	int max_rep = 3;
	
	Collana* collane = init_collane();
	Collana* max = init_collane();
	
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++) {
		make_collane(my_gems, &collane, i, max_rep, max);
		free_list(collane);
		collane = init_collane();
	}
	
	printf("Soluzione ottima di valore %d usando %d gemma/e\nComposizione collana: %s\n",
		   max->valore_totale,
		   max->pietre_totali,
		   max->composizione);
	
	
	free(my_gems);
	free_list(collane);
	
}

void make_collane(Gemstone* gemstones, Collana** last_node, gem_type gem, int max_rep, Collana* max){
	
	Collana* current = *last_node;
	
//	if (current->collana[zaffiro] == current->collana[smeraldo] && gem == zaffiro)
//		return;
	
	/* Controllo se posso continuare ad aggiungere pietre o se le ho finite */
	
	if (current->collana[gem] == gemstones[gem].quantity)
		return;
	
	current->collana[gem] = current->collana[gem] + 1;
	
	current->valore_totale = current->valore_totale + gemstones[gem].value;
	
	switch (gem) {
		case zaffiro:
			current->composizione[current->pietre_totali] = ZAFFIRO;
			current->composizione[current->pietre_totali + 1] = '\0';
			break;
		case rubino:
			current->composizione[current->pietre_totali] = RUBINO;
			current->composizione[current->pietre_totali + 1] = '\0';
			break;
		case topazio:
			current->composizione[current->pietre_totali] = TOPAZIO;
			current->composizione[current->pietre_totali + 1] = '\0';
			break;
		case smeraldo:
			current->composizione[current->pietre_totali] = SMERALDO;
			current->composizione[current->pietre_totali + 1] = '\0';
			break;
	}
	
	current->composizione = realloc(current->composizione, sizeof(char) * (current->pietre_totali + 1) + 1);
	current->pietre_totali = current->pietre_totali + 1;
	
	if (check_z_s(current->composizione) || check_rep(current->composizione, max_rep)) {
		return;
	}
	
	
	if (current->valore_totale > max->valore_totale) {
		max->valore_totale = current->valore_totale;
		max->composizione = strdup(current->composizione);
		max->pietre_totali = current->pietre_totali;
	}
	
//	printf("%s - [Tot val: %d] [Tot pietre: %d]\n", current->composizione, current->valore_totale, current->pietre_totali);
	
	Collana* right = dup_node(current);
	
	make_collane(gemstones, &current, gemstones[gem].links[0], max_rep, max);

	current->next = right;
	
	make_collane(gemstones, &right, gemstones[gem].links[1], max_rep, max);
	
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
	
	head->composizione = (char*) malloc(sizeof(char) * 2);
	head->valore_totale = 0;
	head->pietre_totali = 0;
	head->next = NULL;
	
	return head;
	
}

Collana* dup_node(Collana* collana){
	
	Collana* my_collana = (Collana*) malloc(sizeof(Collana));
	
	if (my_collana == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++)
		my_collana->collana[i] = collana->collana[i];
	
	my_collana->composizione = strdup(collana->composizione);
	my_collana->pietre_totali = collana->pietre_totali;
	my_collana->valore_totale = collana->valore_totale;
	
	my_collana->next = NULL;
	
	return my_collana;
	
}


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
		free(temp->composizione);
		free(temp);
	}
}

void print_collane(Collana* collane){
	
	for (; collane != NULL; collane = collane->next) {
		printf("[%d %d %d %d] - pietre totali: %d - disposizione: %s\n",
			   collane->collana[0],
			   collane->collana[1],
			   collane->collana[2],
			   collane->collana[3],
			   collane->pietre_totali,
			   collane->composizione);
	}
	
}

int check_rep(char* string, int max_rep){
	
	int counter = 0;
	
	for (int i = 0; string[i] != '\0'; i++) {
		
		if (i) {
			if (string[i] == string[i - 1])
				counter ++;
			else
				counter = 0;
			
			if (counter == max_rep)
				return 1;
		}
		
	}
	
	return 0;
	
}

int check_z_s(char* string){
	
	int z_counter = 0;
	int s_counter = 0;
	
	for (int i = 0; string[i] != '\0' ; i++) {
		if (string[i] == ZAFFIRO)
			z_counter ++;
		if (string[i] == SMERALDO)
			s_counter ++;
	}
	
	if (z_counter > s_counter)
		return 1;
	else
		return 0;
	
}
