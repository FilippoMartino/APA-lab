#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_ID_SIZE 6
#define COMMAND_NUMBER 6

/* Stringa globale in cui si possono specificare i comandi da "far inserire" all'utente */
const char* commands_array[COMMAND_NUMBER] = {"inserimento",
			"ricerca_per_id",
			"canc_per_id",
			"canc_per_date",
			"stampa",
			"fine"};

/* Struttura per la gestione delle date */
typedef struct {
	int year;
	int month;
	int day;
} Date;

/* Informazioni relative ad un record anagrafe */
typedef struct{
	char id[MAX_ID_SIZE];
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	Date date_of_birth;
	char street[MAX_SIZE];
	char city[MAX_SIZE];
	int cap;
} Bio;

typedef struct _registry Registry;

/* Nodo */
struct _registry{
	Bio* bio;
	Registry* next;
};

/* enumerazione per gestione più comprensibile del menu */
typedef enum {
	inserimento,
	ricerca_per_id,
	canc_per_id,
	canc_per_date,
	stampa,
	fine
} _comando;

typedef enum {
	id,
	date
} delete_type;

Bio* get_bio_by_file(char*);
Bio* get_bio_by_console(void);

void print_single_record(Bio);
void print_registry(Registry*);
void mem_check(void*);
void sort_bio(Bio*, int);
void find_by_code(Registry*);
Registry* delete(Registry*, delete_type);

int fill_list(Registry**);
int get_file_lenght(char*);
int is_greater(Date, Date);
int get_command(void);

/*
 Serie di funzioni per la gestione delle liste:
 ricordo essere in un contesto di liste ordinate
 */

Registry* insert_sorted_list(Registry*, Bio*);
Registry* new_node(Bio*, Registry*);
Bio* sort_list_search(Registry*, char*);
Bio* delete_by_id(Registry**, char*);
void free_list(Registry*);



int main(int argc, const char * argv[]) {
	
	
	/* Inizializzo la mia lista vuota */
	Registry* my_registry = NULL;
	
	for (int command = get_command(); command != fine ; command = get_command()) {
		
		switch (command) {
			case inserimento:
				if(!fill_list(&my_registry)) exit(EXIT_FAILURE);
				break;
			case stampa:
				print_registry(my_registry);
				break;
			case ricerca_per_id:
				find_by_code(my_registry);
				break;
			case canc_per_id:
				my_registry = delete(my_registry, id);
				break;
			case canc_per_date:
				my_registry = delete(my_registry, date);
				break;
			case fine:
				return 0;
		}
		
	}
	
	free_list(my_registry);
	
}

void print_single_record(Bio bio){
	
	printf("\nCodice: %s\nNome: %s\nCognome: %s\nData di nascita: %d/%d/%d\nVia: %s\nCittà: %s\nCAP: %d\n\n",
		   bio.id,
		   bio.name,
		   bio.surname,
		   bio.date_of_birth.day,
		   bio.date_of_birth.month,
		   bio.date_of_birth.year,
		   bio.street,
		   bio.city,
		   bio.cap);
	
}

void print_registry(Registry* registry){
	
	for (; registry != NULL; registry = registry->next) {
		print_single_record(*registry->bio);
	}
	
	
}

int get_file_lenght(char* file_name){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Error reading line numbers\n");
		return 0;
	}
	
	int lines = 0;
	char ch;
	
	for(;!feof(fp);) {
		ch = fgetc(fp);
		if(ch == '\n')
			lines++;
	}
	fclose(fp);
	return lines;
}

int fill_list(Registry** registry){
	
	fflush(stdin);
	char c;
	printf("Scegliere sorgente input..\nc -> console\nf -> file\nComando: ");
	scanf("%c", &c);
	
	if (c == 'c') {
		
		Bio* my_bio = get_bio_by_console();
		
		*registry = insert_sorted_list(*registry, my_bio);
		
		if (*registry == NULL) {
			printf("problemi di allocazione memoria\n");
			return 0;
		}else
			return 1;
		
	} else if (c == 'f') {
		
		/* Aquisisco le inforamzioni utili all'apertura/letttura del file */
		char file_name[MAX_SIZE];
		printf("Inserisci il nome del file dal quale vuoi leggere: ");
		scanf("%s", file_name);
		
		/* Trovo il numero di record di che compongono il file */
		int record_number = get_file_lenght(file_name);
		
		/* Prendo il record delle singole anagrafi da aggiungere alla lista */
		Bio* file_bio = get_bio_by_file(file_name);
		
		for (int i = 0; i < record_number; i++) *registry = insert_sorted_list(*registry, &file_bio[i]);
		
		if (*registry == NULL) {
			printf("problemi di allocazione memoria\n");
			return 0;
		}else
			return 1;
		
	} else {
		printf("Hai inserito un comando non consentito...\n");
		return 1;
	}
	
}

Bio* get_bio_by_console(){
	
	Bio* bio = malloc(sizeof(Bio));
	
	printf("Inserire codice: ");
	scanf("%s", bio->id);
	
	printf("Inserire nome: ");
	scanf("%s", bio->name);
	
	printf("Inserire cognome: ");
	scanf("%s", bio->surname);
	
	printf("Inserire data di nascita (formato gg/mm/aa): ");
	scanf("%d/%d/%d",
		  &bio->date_of_birth.day,
		  &bio->date_of_birth.month,
		  &bio->date_of_birth.year);
	
	printf("Inserire via: ");
	scanf("%s", bio->street);
	
	printf("Inserire città: ");
	scanf("%s", bio->city);
	
	printf("Inserire cap: ");
	scanf("%d", &bio->cap);
	printf("\n");


	
	return bio;
}

Bio* get_bio_by_file(char* file_name){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Error opening file %s\n", file_name);
		return NULL;
	}
	
	int record_number = get_file_lenght(file_name);
	
	if (!record_number)
		exit(EXIT_FAILURE);
	
	Bio* bio = (Bio*) malloc(sizeof(Bio) * record_number);
	mem_check(bio);
	
	for (int i = 0; i < record_number; i++) {
		
		fscanf(fp, "%s %s %s %d/%d/%d %s %s %d",
			   bio[i].id,
			   bio[i].name,
			   bio[i].surname,
			   &bio[i].date_of_birth.day,
			   &bio[i].date_of_birth.month,
			   &bio[i].date_of_birth.year,
			   bio[i].street,
			   bio[i].city,
			   &bio[i].cap);
	
	}
	
	/* Ordino la lista delle anagrafi per data prima di restituirla */
	sort_bio(bio, record_number);
	
	fclose(fp);
	
	printf("Record acquisiti\n");
	
	return bio;
	
}

void mem_check(void* p){
	if (p == NULL) {
		printf("Errore di allocazione memoria\n");
		exit(EXIT_FAILURE);
	}
}

/**
 Genera un nodo, allocandovi lo spazio necesario, inserendoci il valore di riferimento
 e legandolo al nodo successivo, parametro next
 
 Se incorre in problemi (allocazione memoria fallita) restituisce NULL
 */
Registry* new_node(Bio* bio, Registry* next){
	
	Registry* reg = (Registry*) malloc(sizeof(Registry));
	
	if (reg == NULL) return NULL;
	
	reg->bio = bio;
	reg->next = next;
	
	return reg;
	
}

/**
 Inserisce un nuovo nodo secondo il criterio di questo esercizio, ossia la data di nascita
 più recente; si occupa inoltre anche di gestire i casi di lista vuota, generando ed inserrendo una
 nuova testa.
 
 Se incorre in problemi restituisce NULL
 */
Registry* insert_sorted_list(Registry* registry, Bio* bio){
	
	Date my_date = bio->date_of_birth;
	
	if (registry == NULL || is_greater(my_date, registry->bio->date_of_birth) == 1) {
		return new_node(bio, registry);
	}
	
	Registry* current;
	Registry* temp;
	
	for (current = registry->next, temp = registry;
		 current != NULL && is_greater(my_date, current->bio->date_of_birth) != 1;
		 temp = current, current = current->next);
	
	temp->next = new_node(bio, current);
	
	
	return registry;
}

/**
 Confronta due date, ritorna:
 1 se data_1 > date_2
 0 se data_1 = date_2
 -1 se data_1 < date_2
 */
int is_greater(Date date_1, Date date_2){
	
	if(date_1.year == date_2.year &&
	   date_1.month == date_2.month &&
	   date_1.day == date_2.day)
		return 0;
	
	if (date_1.year > date_2.year)
		return 1;
	if (date_1.year < date_2.year)
		return -1;
	
	if (date_1.month > date_2.month)
		return 1;
	if (date_1.month < date_2.month)
		return -1;
	
	if (date_1.day > date_2.day)
		return 1;
	if (date_1.day < date_2.day)
		return -1;

	return 0;
	
}

void sort_bio(Bio* bio, int record_number){
	
	for (int i = 0; i < record_number - 1; i++) {
		for (int j = record_number - 1; j > 0; j--) {
			if (is_greater(bio[j].date_of_birth, bio[j - 1].date_of_birth) == 1) {
				Bio temp = bio[j];
				bio[j] = bio[j - 1];
				bio[j - 1] = temp;
			}
		}
	}
	
}

/**
 Gestisce l'input
 */
int get_command(){
	
	printf("Inserire comando (h per una lista dei possibili comandi): ");
	char comando_s[MAX_SIZE];
	scanf("%s", comando_s);
	
	if (!strcmp(comando_s, "h")) {
		printf("############################################################\n");
		printf("Lista comandi possibili:\n");
		printf("%s: possibilità di inserire nuovi record\n", commands_array[0]);
		printf("%s: ricerca di un record della anagrafe per il codice\n", commands_array[1]);
		printf("%s: eliminazioned del record (se prensente) corrispondente al codice specificato\n", commands_array[2]);
		printf("%s: eliminazione di tutti i record compresi tra due date\n", commands_array[3]);
		printf("%s: stampa l'elenco dei record\n", commands_array[4]);
		printf("%s: termina il programma\n", commands_array[5]);
		printf("############################################################\n");
		return get_command();
		
	}
	
	for (int i = 0; i < COMMAND_NUMBER; i++)
	if (!strcmp(comando_s, commands_array[i]))
		return i;
	
	printf("Hai inserito un comando non consentito\n");
	return -1;
	
}

Bio* sort_list_search(Registry* registry, char* id){
	
	Registry* current;
	
	for (current = registry;
		 current->next != NULL && strcmp(id, current->bio->id) != 0;
		 current = current->next);
	
	if (!strcmp(id, current->bio->id))
		return current->bio;
	else
		return NULL;
	
}

void find_by_code(Registry* reg){
	
	char id[MAX_ID_SIZE];
	printf("Inserire il codice da cercare: ");
	scanf("%s", id);
	
	Bio* bio = sort_list_search(reg, id);
	
	if (bio == NULL)
		printf("Nessun elemento trovato con il codice fornito");
	else
		print_single_record(*bio);
	
}

Registry* delete(Registry* reg, delete_type dt){
	
	if (dt == id) {
		
		char my_id[MAX_ID_SIZE];
		printf("Inserisci l'id per il quale devo cancellare (se trovato) il record: ");
		scanf("%s", my_id);
		Bio* bio = delete_by_id(&reg, my_id);
		
		if (bio != NULL) {
			printf("Record trovato :\n");
			print_single_record(*bio);
			printf("Record codice %s eliminato, memoria deallocata.\n", my_id);
		} else {
			printf("Nessun record corrispondente a %s trovato...\n", my_id);
		}
		
		return reg;
		
	} else if (dt == date) {
		
		Date date_1, date_2;
		
		printf("Inserisci la prima data (dd/mm/yyyy): ");
		scanf("%d/%d/%d", &date_1.day, &date_1.month, &date_1.year);
		printf("Inserisci la seconda data (dd/mm/yyyy): ");
		scanf("%d/%d/%d", &date_2.day, &date_2.month, &date_2.year);
		
		Registry* my_list = reg;
		char** id_lists = (char**) malloc(sizeof(char*));
		int list_length = 0;
		
		for (; my_list->next != NULL; my_list = my_list->next) {
			
			if (is_greater(date_1, my_list->bio->date_of_birth) == 1 &&
				is_greater(date_2, my_list->bio->date_of_birth) != 1) {
				
				list_length ++;
				id_lists[list_length - 1] = strdup(my_list->bio->id);
				id_lists = realloc(id_lists, sizeof(char*) * (list_length));
				
			}
		}
		
		
		
		if (list_length) {
			printf("Record:\n");
			for (int i = 0; i < list_length; i++) {
				Bio* bio = delete_by_id(&reg, id_lists[i]);
				print_single_record(*bio);
			}
		} else {
			printf("Non sono presenti record tra le due date inserite\n");
		}
		
		return reg;
		
		
	} else {
		return reg;
	}
	
}

Bio* delete_by_id(Registry** reg, char* id){
	
	if (reg == NULL)
		return NULL;
	
	
	
	Registry* current;
	Registry* temp;
	
	
	for (current = *reg, temp = NULL;
		 current != NULL;
		 temp = current, current = current->next){
		
		if(!strcmp(current->bio->id, id)){
			if (*reg == current)
				*reg = current->next;
			else
				temp->next = current->next;
			
			Bio* bio = current->bio;
			free(current);
			
			return bio;
		
		}
		
	}
	
	return NULL;
}

void free_list(Registry* reg){
	for (;reg != NULL;){
		Registry* temp = reg;
		reg = reg->next;
		free(temp);
	}
}
