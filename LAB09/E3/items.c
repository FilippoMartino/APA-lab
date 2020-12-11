#include "items.h"

/**
 Genera una lista di "equipaggiamenti"
 
 @param file_name nome del file dal quale leggere gli @a equipaggiamenti
 @returns @b Equipment* puntatore alla testa della lista "compilata"
 @returns @b NULL nel caso di problemi di allocazioni memoria
 */
Equipment* load_equipment_list(char* file_name){
	
	Equipment* eq = NULL;
	Equipment* head = NULL;
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Errore apertura del file: %s\n", file_name);
		return NULL;
	}
	
	int file_lenght;
	fscanf(fp, "%d", &file_lenght);
	
	char name[MAX_STRING];
	char class[MAX_STRING];
	Stat stat;
	
	for (int i = 0; i < file_lenght; i++) {
		fscanf(fp, "%s %s %d %d %d %d %d %d",
			   name,
			   class,
			   &stat.hp,
			   &stat.mp,
			   &stat.atk,
			   &stat.def,
			   &stat.mag,
			   &stat.spr);
		
		
		if (!i){
			eq = new_equipment(name, class, stat);
			head = eq;
		} else {
			eq->next = new_equipment(name, class, stat);
			eq = eq->next;
		}
	}
	
	fclose(fp);
	
	return head;
	
}

/**
 Genera un nuovo nodo di equipaggiamento
 
 @param name nome dell'equipaggiamento
 @param class classe dell'equipaggiamento
 @param stat struttura con tutte le statistiche
 @returns @b Equipement* un nuovo nodo di tipo @a Equipaggiamento
 @returns @b NULL in caso di errori di allocazione memoria
 */
Equipment* new_equipment(char name[MAX_STRING], char class[MAX_STRING], Stat stat){
	
	Equipment* eq = (Equipment*) malloc(sizeof(Equipment));
	Item* item = (Item*) malloc(sizeof(Item));
	
	if (eq == NULL || item == NULL) {
		printf("Errore allocazione memoria");
		return NULL;
	}
				
	strcpy(item->name, name);
	strcpy(item->class, class);
	item->eq_stat = stat;
	
	eq->item = item;
	eq->next = NULL;
	
	return eq;
	
}

/**
 Gestisce la lista quando l'utente vuole aggiungere equipaggiamenti
 
 @param eq puntatore al puntatore alla testa
 */
void add_equipments(Equipment** eq){
	
	char file_name[MAX_STRING];
	printf("Inserire nome del file da quale caricare la lista degli equipaggiamenti: ");
	scanf("%s", file_name);
	
	Equipment* my_equipment = load_equipment_list(file_name);
	
	if (my_equipment == NULL)
		return;
	
	if (*eq == NULL) {
		*eq = my_equipment;
	} else {
		Equipment* temp = *eq;
		for (; temp->next != NULL; temp = temp->next);
		
		temp->next = my_equipment;
	}
	
}

/**
 Assegna ad un giocatore (se possibile) un oggetto dalla lsita degli items disponibili
 
 @param eq puntatore a puntatore alla testa della lista degli equipaggiamenti
 @param players puntatore a puntatore alla testa della lista dei personaggi
 */
void equip_player(Equipment** eq, Player** players){
	
	if (*players == NULL) {
		printf("Non sono ancora stati caricati giocatori.\n");
		return;
	}
	
	char id[MAX_STRING];
	printf("Inserire l'id del personaggio al quale aggiungere un item: ");
	scanf("%s", id);
	
	Player* current = *players;
	
	for (; current->next != NULL; current = current->next) {
		if (!strcmp(current->bio->id, id)) {
			
			if (current->bio->item_number == EQ_SLOT_NUMBER) {
				printf("Non restano slot diponibili per questo personaggio\n");
				return;
			}
			
			printf("Giocatore selezionato:\n");
			print_player(*current->bio);
			
			char item_name[MAX_STRING];
			printf("Inserisci il nome dell'item da aggiungere: ");
			scanf("%s", item_name);
			
			Equipment* my_eq = *eq;
			for (; my_eq != NULL; my_eq = my_eq->next) {
				if (!strcmp(my_eq->item->name, item_name)) {
					current->bio->player_inv[current->bio->item_number] = *my_eq->item;
					current->bio->item_number = current->bio->item_number + 1;
					printf("Ho inserito:\n");
					print_equipment(*my_eq->item);
					printf("Oggetto inserito con successo nell'inventario.\n");
					return;
				}
				
			}
			
			printf("Non sono stati trovati oggetti con l'id specificato.\n");
			return;
			
		}
	}
	
	printf("Non sono stati trovati personaggi con l'id specificato.\n");
	
}

/**
 Stampa un singolo equipaggiamento
 
 @param item struttura con le informazioni sul singolo equipaggiamento
 */
void print_equipment(Item item){
	printf("\nNome: %s\nClasse: %s\nhp: %d\nmp: %d\natk: %d\ndef: %d\nmag: %d\nspr: %d\n",
		   item.name,
		   item.class,
		   item.eq_stat.hp,
		   item.eq_stat.mp,
		   item.eq_stat.atk,
		   item.eq_stat.def,
		   item.eq_stat.mag,
		   item.eq_stat.spr
		   );
}

/**
 Stampa una lista di equipaggiamenti
 
 @param eq puntatore alla testa della lista
 */
void print_equipment_list(Equipment* eq){
	for (; eq != NULL; eq = eq->next)
	print_equipment(*eq->item);
}

/**
 Dealloca una lista di @a equipaggiamenti (Equipment*)
 
 @param eq la testa della lista degli equipaggiamenti
 */
void free_equipment(Equipment* eq){
	Equipment* temp;
	for (; eq != NULL;) {
		temp = eq;
		eq = eq->next;
		free(temp->item);
		free(temp);
	}
}
