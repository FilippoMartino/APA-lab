#include "personaggi.h"

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
 Cancella un personaggio dalla lista in base all'id (specificato da console)
 
 @param players puntatore a puntatore alla testa della lista dei personaggi
 */
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
 Stampa le specifiche di un giocatore (la scelta viene fatta fare dalla funzione mediante inseirmento dell'id) calcolate ponderate in
 base agli elementi presenti nell'inventario
 
 @param players puntatore alla testa della lista dei giocatori
 */
void get_player_stat(Player* players){
	
	char id[MAX_STRING];
	printf("Inserire id del personaggio per il quale calcolare le statistiche: ");
	scanf("%s", id);
	Stat total_stat;
	init_stat(&total_stat);
	
	for (; players->next != NULL; players = players->next) {
		
		if (!strcmp(players->bio->id, id)) {
			
			if (players->bio->item_number == 0) {
				printf("Il personaggio che hai scelto non ha elementi nell'inventario.\n");
				return;
			}
			
			/* Calcolo delle statistiche totale degli oggetti nell'invetario */
			for (int i = 0; i < players->bio->item_number; i++) {
				
				total_stat = sum_stat(total_stat, players->bio->player_inv[i].eq_stat);

			}
			
			/* Aggiunta delle statistiche proprie del personaggio */
			total_stat = sum_stat(total_stat, players->bio->player_stat);
			
			/* Controllo elementi negativi */
			
			if (total_stat.atk < 0) total_stat.atk = 0;
			if (total_stat.def < 0) total_stat.def = 0;
			if (total_stat.hp < 0) total_stat.hp = 0;
			if (total_stat.mag < 0) total_stat.mag = 0;
			if (total_stat.mp < 0) total_stat.mp = 0;
			if (total_stat.spr < 0) total_stat.spr = 0;
			
			printf("Stampa del personaggio con statistiche dell'inventario applicate:\n");
			printf("\nId: %s\nNome: %s\nClasse: %s\nhp: %d\nmp: %d\natk: %d\ndef: %d\nmag: %d\nspr: %d\n",
				   players->bio->player_name,
				   players->bio->player_name,
				   players->bio->class,
				   total_stat.hp,
				   total_stat.mp,
				   total_stat.atk,
				   total_stat.def,
				   total_stat.mag,
				   total_stat.spr
				   );
			
			return;

		}
	}
	
	printf("Non sono stati trovati personaggi corrispondeti all'id inserito\n");
	return;
	
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
 Inizializza a 0 i parametri di una struttura @b Stat
 
 @param stat puntatore alla struttura da inizializzare
 */
void init_stat(Stat* stat){
	stat->atk = 0;
	stat->def = 0;
	stat->hp = 0;
	stat->mag = 0;
	stat->mp = 0;
	stat->spr = 0;
}

/**
 Somma due strutture Stat
 
 @param stat_1 la prima struttura
 @param stat_2 seconda struttura
 @returns @b Stat una struttura con parametri la somma delle due passate
 */
Stat sum_stat(Stat stat_1, Stat stat_2){
	Stat sum;
	init_stat(&sum);
	
	sum.hp = stat_1.hp + stat_2.hp;
	sum.atk = stat_1.atk + stat_2.atk;
	sum.def = stat_1.def + stat_2.def;
	sum.mag = stat_1.mag + stat_2.mag;
	sum.mp = stat_1.mp + stat_2.mp;
	sum.spr = stat_1.spr + stat_2.spr;
	
	return sum;
	
}
