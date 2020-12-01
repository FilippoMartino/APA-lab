#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_SIZE 7
#define MAX_STRING 50
#define EQ_SLOT_NUMBER 8
#define COMMAND_NUMBER 7

/*
 
 Mi sono accorto, a programma finito, che "gli equipaggiamenti" andavano caricati
 in un vettore di strutture, io gli ho invece salvati in una lista...
 Credo che hai fini dell'esercizio non cambi, sono comunque consapevole di non aver
 sequito al 100% la richiesta dell'esercizio
 
 */

/*
 Nell'array qui sotto possono essere cambiati (mantenendo naturalmente l'ordine)
 i comandi che l'utente dovra inserire da console (in modo da velocizzare l'inserimento input e la
 manutenzione del codice)
 */

const char* commands_array[COMMAND_NUMBER] = {
	"load_p",
	"load_e",
	"add_p",
	"del",
	"add_e",
	"cal",
	"fine"};

typedef enum {
	load_player,
	load_equipment,
	add_player,
	delete_player,
	add_eq_to_player,
	get_stat,
	end
} _comando;

typedef struct {
	int hp;
	int mp;
	int atk;
	int def;
	int mag;
	int spr;
} Stat;

typedef struct {
	char name[MAX_STRING];
	char class[MAX_STRING];
	Stat eq_stat;
} Item;

typedef struct _equipment Equipment;

struct _equipment {
	Item* item;
	Equipment* next;
};

typedef struct {
	char id[ID_SIZE];
	char player_name[MAX_STRING];
	char class[MAX_STRING];
	Item player_inv[EQ_SLOT_NUMBER];
	int item_number;
	Stat player_stat;
} Bio;

typedef struct _player Player;

struct _player {
	Bio* bio;
	Player* next;
};

Player* load_player_list(char*);
Player* new_player(char[ID_SIZE], char[MAX_STRING], char[MAX_STRING], Stat);

Equipment* load_equipment_list(char*);
Equipment* new_equipment(char[MAX_STRING], char[MAX_STRING], Stat);

_comando get_command(void);

int get_file_lenght(char*);

void add_player_by_console(Player**);
void new_players(Player**);
void delete_player_by_id(Player**);
void add_equipments(Equipment**);
void equip_player(Equipment**, Player**);
void get_player_stat(Player*);
void print_player(Bio);
void print_players(Player*);
void free_players(Player*);
void print_equipment(Item);
void print_equipment_list(Equipment*);
void free_equipment(Equipment*);

int main(int argc, const char * argv[]) {
	
	Player* my_players = NULL;
	Equipment* my_equipments = NULL;
	
	for (int command = get_command(); command != end; command = get_command()) {
		switch (command) {
			case load_player: new_players(&my_players);
				break;
			case load_equipment: add_equipments(&my_equipments);
				break;
			case add_player: add_player_by_console(&my_players);
				break;
			case delete_player: delete_player_by_id(&my_players);
				break;
			case add_eq_to_player: equip_player(&my_equipments, &my_players);
				break;
			case get_stat: get_player_stat((my_players);
				break;
			case end:
				break;
		}
	}
	
	print_players(my_players);
	print_equipment_list(my_equipments);
	
	free_players(my_players);
	free_equipment(my_equipments);
	
	
	return 0;
}

/**
 Aggiunge "in lista" un'altra lista letta da un file
 
 @param players puntatore al puntatore alla testa della lista (può) già essere composta da altri elementi
 */
void new_players(Player** players){
	
	printf("Inserire nome del file dal quale caricare lista personaggi: ");
	char file_name[MAX_STRING];
	scanf("%s", file_name);
	
	Player* my_players = load_player_list(file_name);
	
	if (my_players == NULL)
		return;
	
	if (*players == NULL) {
		*players = my_players;
	} else {
		Player* temp = *players;
		for (; temp->next != NULL; temp = temp->next);
		temp->next = my_players;
		
	}
}

/**
 Permette di aggiungere in lista un nodo "personaggio"  acquisito da console
 
 @param players puntatore al puntatore alla testa della lista
 */
void add_player_by_console(Player** players){
	
	char id[ID_SIZE];
	char player_name[MAX_STRING];
	char class[MAX_STRING];
	Stat player_stat;
	
	printf("Inserire id personaggio: ");
	scanf("%s", id);
	printf("Inserire nome personaggio: ");
	scanf("%s", player_name);
	printf("Inserire classe personaggio: ");
	scanf("%s", class);
	printf("Inizio inserimento statistiche\n");
	printf("hp: ");
	scanf("%d", &player_stat.hp);
	printf("mp: ");
	scanf("%d", &player_stat.mp);
	printf("atk: ");
	scanf("%d", &player_stat.atk);
	printf("def: ");
	scanf("%d", &player_stat.def);
	printf("mag: ");
	scanf("%d", &player_stat.mag);
	printf("spr: ");
	scanf("%d", &player_stat.spr);
	
	Player* temp = *players;
	
	if (temp == NULL)
		temp = new_player(id, player_name, class, player_stat);
	
	for (; temp->next != NULL; temp = temp->next);
	
	temp->next = new_player(id, player_name, class, player_stat);
	
	
}

void delete_player_by_id(Player** players){
	
	
	/* Gesitone lista vuota */
	if (players == NULL)
		return;
	
	char player_id[MAX_STRING];
	printf("Inserisci l'id del personaggio da eliminare: ");
	scanf("%s", player_id);
	
	Player* current;
	Player* temp;
	
	/* Scorrimento lista finchè non trovo il nodo oppure arrivo in fondo */
	for (current = *players, temp = NULL;
		 current != NULL;
		 temp = current, current = current->next){
		
		/* Sostituzione puntatori e deallocazione */
		if(!strcmp(current->bio->id, player_id)){
			/* nel caso fossimo nel promo nodo */
			if (*players == current)
				*players = current->next;
			else
				temp->next = current->next;
			
			free(current->bio);
			free(current);
			
			return;
		
		}
	}
	
	printf("Non è stato trovato nessun personaggio corrispondente all'id inserito\n");
	
	return;
	
}

/**
 Genera una lista di giocatori a partire da un file
 
 @param file_name nome del file dal quale leggere l'elenco dei giocatori
 @returns @b Player* puntatore alla testa della lista generata
 */
Player* load_player_list(char* file_name){
	
	FILE* fp = fopen(file_name, "r");
	if (fp == NULL) {
		printf("Errore apertura del file %s\n", file_name);
		return NULL;
	}
	
	int file_lenght = get_file_lenght(file_name);
	
	if (!file_lenght) {
		printf("Hai inserito un file vuoto\n");
		return NULL;
	}
	
	Player* player = NULL;
	Player* head = NULL;
	
	char id[ID_SIZE];
	char name[MAX_STRING];
	char class[MAX_STRING];
	Stat stat;
	
	for (int i = 0; i < file_lenght; i++) {
		fscanf(fp, "%s %s %s %d %d %d %d %d %d",
			   id,
			   name,
			   class,
			   &stat.hp,
			   &stat.mp,
			   &stat.atk,
			   &stat.def,
			   &stat.mag,
			   &stat.spr);
		
		if (!i){
			player = new_player(id, name, class, stat);
			head = player;
		} else {
			player->next = new_player(id, name, class, stat);
			player = player->next;
		}
		
	}
	
	fclose(fp);
	
	return head;
}

/**
 Crea un nodo di tipo @a player, allocando lo spazio sia per il nodo stesso sia per la struttura @a Bio contenente le informazioni sul giocatore
 
 @param id id personaggio
 @param name nome personaggio
 @param class classe personaggio
 @param stat le statistiche del personaggio
 @returns @b Player* restituisce il puntatore al nodo creato
 */
Player* new_player(char id[ID_SIZE], char name[MAX_STRING], char class[MAX_STRING], Stat stat){
	
	Player* new = (Player*) malloc(sizeof(Player));
	Bio* bio = (Bio*) malloc(sizeof(Bio));
	
	if (new == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	stpcpy(bio->id, id);
	stpcpy(bio->player_name, name);
	stpcpy(bio->class, class);
	bio->player_stat = stat;
	bio->item_number = 0;
	
	new->bio = bio;
	new->next = NULL;
	
	return new;
	
	
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
			for (; my_eq->next != NULL; my_eq = my_eq->next) {
				if (!strcmp(my_eq->item->name, item_name)) {
					current->bio->player_inv[current->bio->item_number] = *my_eq->item;
					current->bio->item_number = current->bio->item_number + 1;
					printf("Ho inserito:\n");
					print_equipment(*my_eq->item);
					printf("Oggetto inserito con successo nell'inventario.\n");
					return;
				} else {
					printf("Non è stato trovato nessun equipaggiamento corrsipondente al codice inserito\n");
					return;
				}
			}
			
		}
	}
	
	printf("Non sono stati trovati personaggi con l'id specificato.\n");
	
}

void get_player_stat(Player* players){
	
	char id[MAX_STRING];
				
}
									
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
 Gestisce l'input utente
 
 @returns @b _comando una tipologia di comando
 @returns @b -1 se l'utente ha inserito un output sbagliato
 */
_comando get_command(){
	
	char comando_s[MAX_STRING];
	printf("Inserire comando (h per una lista dei possibili comandi): ");
	scanf("%s", comando_s);
	
	if (!strcmp(comando_s, "h")) {
		printf("############################################################\n");
		printf("Lista comandi possibili:\n");
		printf("%s: caricare una lista di personaggi da un file\n", commands_array[0]);
		printf("%s: caricare lista equipaggiamenti da un file\n", commands_array[1]);
		printf("%s: aggiungere un personaggio (da tastiera)\n", commands_array[2]);
		printf("%s: cancellazione di un personaggio\n", commands_array[3]);
		printf("%s: aggiunta equipaggiamento a personaggio\n", commands_array[4]);
		printf("%s: calcolo statistiche di un personaggio\n", commands_array[5]);
		printf("%s: Terminazione del programma\n", commands_array[6]);
		printf("############################################################\n");
		return get_command();
		
	}
	
	for (int i = 0; i < COMMAND_NUMBER; i++){
		if (!strcmp(comando_s, commands_array[i])){
			return i;
		}
	}
	
	printf("Hai inserito un comando non consentito\n");
	return -1;
	
}

/**
 Permette di conoscere la lunghezza di un file
 
 @param file_name nome del file di cui si vuol conoscere la lunghezza
 @returns @b int lunghezza del file
 */
int get_file_lenght(char* file_name){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Error reading line numbers\n");
		return 0;
	}
	
	int lines = 0;
	char ch;
	char last = '\n';
	
	for(;!feof(fp);) {
		ch = fgetc(fp);
		if(ch == '\n' && last != '\n' && ch != EOF)
			lines++;
		last = ch;
	}
	fclose(fp);
	
	/* Conto anche l'ultima riga */
	lines++;
	return lines;
}

/**
 Stampa un singolo giocatore
 
 @param bio struttura con le informazioni sul singolo giocatore
 */
void print_player(Bio bio){
	printf("\nId: %s\nNome: %s\nClasse: %s\nhp: %d\nmp: %d\natk: %d\ndef: %d\nmag: %d\nspr: %d\n",
		   bio.id,
		   bio.player_name,
		   bio.class,
		   bio.player_stat.hp,
		   bio.player_stat.mp,
		   bio.player_stat.atk,
		   bio.player_stat.def,
		   bio.player_stat.mag,
		   bio.player_stat.spr
		   );
}

/**
 Stampa una lista di giocatori
 
 @param players puntatore alla testa della lista
 */
void print_players(Player* players){
	
	for (; players != NULL; players = players->next)
	print_player(*players->bio);
	
}

/**
 Dealloca una lista di giocatori
 
 @param players puntatore alla testa della lista
 */
void free_players(Player* players){
	Player* temp;
	for (; players != NULL;) {
		temp = players;
		players = players->next;
		free(temp->bio);
		free(temp);
	}
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
