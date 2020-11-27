#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEMSTONE_NUMBER 4

const int legame_zaffiro[2] = {0, 1};
const int legame_smeraldo[2] = {2, 3};
const int legame_rubino[2] = {2, 3};
const int legame_topazio[2] = {0, 1};

const char* gem_name[GEMSTONE_NUMBER] = {
	"zaffiro",
	"rubino",
	"topazio",
	"smeraldo"
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
int* duplica_collana(int*);

void make_collane(Gemstone*, Collana**, gem_type, int*);
void get_max_lenght(void);

/* Gestione lista collane */
Collana* new_node(int*);
Collana* scroll_collana(Collana*);
Collana* init_collane(void);
void free_list(Collana*);

void print_collane(Collana*);
int get_max(Collana*);


int main(int argc, const char * argv[]) {
	
	
	get_max_lenght();
	
	
	
	return 0;
}

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

void get_max_lenght(void){
	
	int* gems_number = (int*) malloc(sizeof(int) * GEMSTONE_NUMBER);
	
	if (gems_number == NULL){
		printf("Problemi di allocazione memoria");
		exit(EXIT_FAILURE);
	}
	
	gems_number[0] = 17;
	gems_number[1] = 20;
	gems_number[2] = 15;
	gems_number[3] = 19;
	
	Gemstone* my_gems = get_gem(gems_number);
	
	if (gems_number == NULL){
		printf("Problemi di allocazione memoria");
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < GEMSTONE_NUMBER; i++) {
		printf("Pietra: %s\n", gem_name[my_gems[i].stone]);
		printf("Possono essere inserite dopo: %s e %s\n", gem_name[my_gems[i].links[0]], gem_name[my_gems[i].links[1]]);
		printf("Numero di pietre: %d\n", my_gems[i].quantity);
	}
	printf("\n");
	
	int max_z = 0;
	int max_r = 0;
	int max_s = 0;
	int max_t = 0;
	
	Collana* collane_zaffiro = init_collane();
	make_collane(my_gems, &collane_zaffiro, zaffiro, &max_z);
	Collana* collane_rubino = init_collane();
	make_collane(my_gems, &collane_rubino, rubino, &max_r);
	Collana* collane_smeraldo = init_collane();
	make_collane(my_gems, &collane_smeraldo, smeraldo, &max_s);
	Collana* collane_topazio = init_collane();
	make_collane(my_gems, &collane_topazio, topazio, &max_t);
	
	printf("\nMax zaffiro: %d\nMax rubino: %d\nMax smeraldo: %d\nMax topazio: %d\n",
		   max_z,
		   max_r,
		   max_s,
		   max_t);
	
//	print_collane(collane_zaffiro);
//	print_collane(collane_rubino);
//	print_collane(collane_smeraldo);
//	print_collane(collane_topazio);

	
	
	
	
	free(my_gems);
	free_list(collane_zaffiro);
	free_list(collane_rubino);
	free_list(collane_smeraldo);
	free_list(collane_topazio);
	
	
	
}

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

void print_collane(Collana* collane){
	
	
	for (; collane != NULL; collane = collane->next) {
		printf("Composizione {%d, %d, %d, %d}\nTotale: %d\n\n",
			   collane->collana[0],
			   collane->collana[1],
			   collane->collana[2],
			   collane->collana[3],
			   collane->collana[0] + collane->collana[1] + collane->collana[2] + collane->collana[3]);
	}
}

int get_max(Collana* collane){
	
	int max = 0;
	int current = 0;
	
	for (; collane != NULL; collane = collane->next) {
		
		for (int i = 0; i < GEMSTONE_NUMBER; i++)
			current = collane->collana[i] + current;
		
		if (current > max) max = current;
		
		current = 0;
		
	}
	
	return max;
}
