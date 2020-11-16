#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 30
#define COMMAND_NUMBER 8

/* struttura in cui dividere le parti della data di una singla tratta (viene salvata anche l'ora di partenza) */
typedef struct {
	int anno;
	int mese;
	int giorno;
	int ore;
	int minuti;
	int secondi;
} Data;

/*
 struttura in cui salvare le informazioni relative alla singola tratta ho forzato il funzionamento
 iniziale di questa struttura, aggiungendo il numero di strutture che verrà poi visto al momento della
 stampa per l'array di ogni singolo log
 */

/* Per ovviare al problema della stampa di tratte di lughezza diversa */
typedef struct {
	char codice_tratta[MAX_SIZE];
	char partenza[MAX_SIZE];
	char destinazione[MAX_SIZE];
	Data data;
	char ora_partenza[MAX_SIZE];
	char ora_arrivo[MAX_SIZE];
	int ritardo;
	int numero_strutture;
} Tratta;

/* Menu */
typedef enum {
	stampa,
	data,
	codice,
	partenza,
	arrivo,
	stazione,
	file,
	fine
	
} comando_e;

void salva_tratte(FILE*, int, Tratta*);
void stampa_tratta(Tratta);
void stampa_tratte(int, int, Tratta**);
void stampa_tratte_su_file(int, int, Tratta**, FILE*);
int get_comando(void);
Tratta** seleziona_dati(int*, Tratta**, comando_e, Tratta**, int*, int*);
Tratta* get_struttura(int, Tratta*);
Tratta* rileggi_file(int*);
int confronta_date(Data, Data);
void dealloca_log_tratte(Tratta**, int, int);

void stampa_log(int, int, Tratta**);
void ordina_data(int, Tratta*);
void ordina_codice(int, Tratta*);
void ordina_partenza(int, Tratta*);
void ordina_arrivo(int, Tratta*);
void ricerca_arrivo(int, Tratta*);
void ricerca_partenza_dicotomica(int, Tratta*);

int main(int argc, const char * argv[]) {
	
	/* Controllo parametri linea di comando */
	if (argc < 2) {
		printf("Errore: PARAM %s FILENAME\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* Apertura file con relativo controllo */
	FILE* fp = fopen(strdup(argv[1]), "r");
	if (fp == NULL){
		printf("Errore all'apertura del file\n");
		exit(EXIT_FAILURE);
	}
	
	int numero_tratte;
	fscanf(fp, "%d", &numero_tratte);
	
	/* Dichiarazione struttura e relativo "riempimento" */
	Tratta* tratte = (Tratta*) malloc(sizeof(Tratta) * numero_tratte);
	salva_tratte(fp, numero_tratte, tratte);
	
	/* Chiusura file */
	fclose(fp);
	
	/* Acquisizione comando utente */
	comando_e comando = get_comando();
	
	/* Controllo sul comando inserito */
	if (comando == - 1)	printf("Hai inserito un comando errato\n");
	
	
	Tratta** log_tratte = (Tratta**) malloc(sizeof(Tratta*));
	*log_tratte = (Tratta*) malloc(sizeof(Tratta) * numero_tratte);
	
	int* ordinato = malloc(sizeof(int));
	
	int* numero_ordinamenti = malloc(sizeof(int));
	
	*numero_ordinamenti = 0;
	
	for	(;comando != fine;){

		if (comando == - 1)
			printf("Hai inserito un comando errato\n");
		
		log_tratte = seleziona_dati(&numero_tratte, &tratte, comando, log_tratte, numero_ordinamenti, ordinato);
		
		comando = get_comando();
	}
	
	dealloca_log_tratte(log_tratte, *numero_ordinamenti, numero_tratte);
	free(ordinato);
	free(numero_ordinamenti);
	free(tratte);
	
	return 0;
	
}

/**
 Si limita a leggere da file e compilare passo passo l'array di strutture
 */
void salva_tratte(FILE* fp, int numero_tratte, Tratta* tratte){
	
	char data[MAX_SIZE];
	
	for (int i = 0; i < numero_tratte; i ++) {
		
		tratte[i].numero_strutture = numero_tratte;
		
		fscanf(fp, "%s %s %s %s %s %s %d",
			   tratte[i].codice_tratta,
			   tratte[i].partenza,
			   tratte[i].destinazione,
			   data,
			   tratte[i].ora_partenza,
			   tratte[i].ora_arrivo,
			   &tratte[i].ritardo);
		
		/* Devo dividere la data nelle sue componenti da inseire nella struttura Data */
		char* ora_partenza = strdup(tratte[i].ora_partenza);
		tratte[i].data.anno = atoi(strtok(data, "/"));
		tratte[i].data.mese = atoi(strtok(NULL, "/"));
		tratte[i].data.giorno = atoi(strtok(NULL, "/"));
		tratte[i].data.ore = atoi(strtok(ora_partenza, ":"));
		tratte[i].data.minuti = atoi(strtok(NULL, ":"));
		tratte[i].data.secondi = atoi(strtok(NULL, ":"));
		
	}
	
}

/**
 Stampa una tratta formattata
 */
void stampa_tratta(Tratta tratta){
	
	char my_date[MAX_SIZE];
	
	sprintf(my_date, "%d/%d/%d",
			tratta.data.anno,
			tratta.data.mese,
			tratta.data.giorno);
	
	printf("\nCodice tratta: %s\nPartenza: %s, Destinazione: %s\nData: %s\nOra di partenza: %s\nOra di arrivo: %s\nRitardo: %d\n\n",
		   tratta.codice_tratta,
		   tratta.partenza,
		   tratta.destinazione,
		   my_date,
		   tratta.ora_partenza,
		   tratta.ora_arrivo,
		   tratta.ritardo);
}

/**
 Stampa le tratte formattate
 */
void stampa_tratte(int numero_tratte, int numero_ordinamenti, Tratta** tratte){
	
	for (int i = 0; i < numero_ordinamenti; i++) {
		printf("Ordinamento numero %d\n", i + 1);
		for (int j = 0; j < tratte[i][0].numero_strutture; j ++) {
			stampa_tratta(tratte[i][j]);
		}
	}
}

/**
 Stampa le tratte formattate sul file passato, non fa nessun tipo di controllo sul puntatore al file
 */
void stampa_tratte_su_file(int numero_tratte, int numero_ordinamenti, Tratta** tratte, FILE* fp){
	
	for (int i = 0; i < numero_ordinamenti; i++) {
		
		fprintf(fp, "Ordinamento numero %d\n", i + 1);
		
		for (int j = 0; j < numero_tratte; j ++) {
			
			char my_date[MAX_SIZE];
			
			sprintf(my_date, "%d/%d/%d",
					tratte[i][j].data.anno,
					tratte[i][j].data.mese,
					tratte[i][j].data.giorno);
			
			fprintf(fp, "\nCodice tratta: %s\nPartenza: %s, Destinazione: %s\nData: %s\nOra di partenza: %s\nOra di arrivo: %s\nRitardo: %d\n\n",
				   tratte[i][j].codice_tratta,
				   tratte[i][j].partenza,
				   tratte[i][j].destinazione,
				   my_date,
				   tratte[i][j].ora_partenza,
				   tratte[i][j].ora_arrivo,
				   tratte[i][j].ritardo);
		}
	}
	
	printf("File salvato\n");
}

Tratta* get_struttura(int n, Tratta* a){

	Tratta* tratte = (Tratta*) malloc(sizeof(Tratta) * n);
	
	for (int i = 0; i < n; i ++) tratte[i] = a[i];
	
	return tratte;
}

/**
 Gestisce l'input
 */
int get_comando(){
	
	printf("Inserire comando (h per una lista dei possibili comandi): ");
	char comando_s[MAX_SIZE];
	scanf("%s", comando_s);
	
	if (!strcmp(comando_s, "h")) {
		printf("############################################################\n");
		printf("Lista comandi possibili:\n");
		printf("stampa -> stampa log\n");
		printf("data -> ordina il log per data\n");
		printf("codice -> ordina il log per codice tratta\n");
		printf("partenza -> ordina il log per stazione di partenza\n");
		printf("arrivo -> ordina il log per stazione di arrivo\n");
		printf("stazione -> ricerca di una tratta per stazione di partenza\n");
		printf("file -> sovrascrivo le tracce sulle quali sto facendo la ricerca con quelle contenute nel file desiderato\n");
		printf("fine -> termina il programma\n");
		printf("############################################################\n");
		return get_comando();
		
	}
	
	char v[COMMAND_NUMBER][MAX_SIZE] = {"stampa", "data", "codice", "partenza", "arrivo", "stazione", "file", "fine"};
	
	for (int i = 0; i < COMMAND_NUMBER; i++)
	if (!strcmp(comando_s, v[i]))
		return i;
	
	
	return -1;
	
}

Tratta* rileggi_file(int* numero_tratte){
	
	char file_name[MAX_SIZE];
	int numero;
	
	printf("Inserire nuovo file dal quale leggere: ");
	scanf("%s", file_name);
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Errore durante la lettura del file\n");
		return NULL;
	}
	
	fscanf(fp, "%d", &numero);
	
	*numero_tratte = numero;
	
	Tratta* tratte = malloc(sizeof(Tratta) * numero);
	
	salva_tratte(fp, numero, tratte);
	
	printf("Numero tratte di 0 -> %d\n", tratte[0].numero_strutture);
	
	fclose(fp);
	
	printf("Tratte aggiornate\n");
	
	return tratte;
	
}

/**
 Smista ed esaurisce la richiesta dell'utente
 */
Tratta** seleziona_dati(int* numero_tratte, Tratta** tratte, comando_e comando, Tratta** log_tratte, int* numero_ordinamenti, int* ordinato){
	
	if (comando != stampa && comando != file && comando != fine) {
		/* Devo di volta in volta re-allocare il log*/
		log_tratte = (Tratta**) realloc(log_tratte, sizeof(Tratta*) * (*numero_ordinamenti + 1));
		printf("Riallocato spazio per %d log strutture\n", *numero_ordinamenti + 1);
		/* Copio ogni volta l'array letto inizialmente nel log */
		log_tratte[*numero_ordinamenti] = get_struttura(*numero_tratte, *tratte);
	}
	
	/* Smisto tra i vari comandi, se prevede un ordinamento, incremento il puntatore */
	switch (comando) {
		case stampa:
			stampa_log(*numero_tratte, *numero_ordinamenti, log_tratte);
			return log_tratte;
		case data:
			ordina_data(*numero_tratte, log_tratte[*numero_ordinamenti]);
			*numero_ordinamenti = *numero_ordinamenti + 1;
			return log_tratte;
		case codice:
			ordina_codice(*numero_tratte, log_tratte[*numero_ordinamenti]);
			*numero_ordinamenti = *numero_ordinamenti + 1;
			return log_tratte;
		case partenza:
			ordina_partenza(*numero_tratte, log_tratte[*numero_ordinamenti]);
			*numero_ordinamenti = *numero_ordinamenti + 1;
			*ordinato = 1;
			return log_tratte;
		case arrivo:
			ordina_arrivo(*numero_tratte, log_tratte[*numero_ordinamenti]);
			*numero_ordinamenti = *numero_ordinamenti + 1;
			return log_tratte;
		case stazione:
			if (*ordinato)
				ricerca_partenza_dicotomica(*numero_tratte, *tratte);
			else
				ricerca_arrivo(*numero_tratte, *tratte);
			return log_tratte;
		case file:
			*tratte = rileggi_file(numero_tratte);
			return log_tratte;
		case fine:
			return log_tratte;
	}
	
}

/**
 Confronta due date, ritorna:
 1 se data_1 > data_2
 0 se data_1 = data_2
 -1 se data_1 < data_2
 */
int confronta_date(Data data_1, Data data_2){
	
	if(data_1.anno == data_2.anno &&
	   data_1.mese == data_2.mese &&
	   data_1.giorno == data_2.giorno &&
	   data_1.ore == data_2.ore &&
	   data_1.minuti == data_2.minuti &&
	   data_1.secondi == data_2.secondi)
		return 0;
	
	if (data_1.anno > data_2.anno)
		return 1;
	if (data_1.anno < data_2.anno)
		return -1;
	
	if (data_1.mese > data_2.mese)
		return 1;
	if (data_1.mese < data_2.mese)
		return -1;
	
	if (data_1.giorno > data_2.giorno)
		return 1;
	if (data_1.giorno < data_2.giorno)
		return -1;
	
	if (data_1.ore > data_2.ore)
		return 1;
	if (data_1.ore < data_2.ore)
		return -1;
	
	if (data_1.minuti > data_2.minuti)
		return 1;
	if (data_1.minuti < data_2.minuti)
		return -1;
	
	if (data_1.secondi > data_2.secondi)
		return 1;
	if (data_1.secondi < data_2.secondi)
		return -1;
	
	return 0;
	
}

/**
 Smista i casi possibili di stampa (a terminale oppure su file) e richiama le rispettive funzioni
 */
void stampa_log(int numero_tratte, int numero_ordinamenti, Tratta **tratte){
	
	char c;
	fflush(stdin);
	printf("Selezionare dove verrà scritto il log:\nc -> console\nf -> file\n");
	printf("Inserire comando: ");
	scanf("%c", &c);
	
	if (c == 'c'){
		stampa_tratte(numero_tratte, numero_ordinamenti, tratte);
	} else if (c == 'f') {
		printf("Scegli il nome del file sul quale salvare il nome (inserire estensione .txt): ");
		char file_name[MAX_SIZE];
		scanf("%s", file_name);
		
		FILE* fp = fopen(file_name, "w");
		
		if (fp != NULL)
			stampa_tratte_su_file(numero_tratte, numero_ordinamenti, tratte, fp);
		else
			printf("Errore sul file di stampa\n");
		
		fclose(fp);
		
	} else {
		printf("Comando errato\n");
	}
	
}

/**
 Ordinamento delle tratte rispetto alla data di partenza, utilizza un algoritmo di sorting e si appoggia alla funzione [confronta_date]
 */
void ordina_data(int numero_tratte, Tratta *tratte){
	
	/* Implemento un bubble sort modificato per le date */
	for (int i = 0; i < numero_tratte - 1; i ++) {
		for (int j = numero_tratte - 1; j > 0; j--) {
			if (confronta_date(tratte[j].data, tratte[j - 1].data) == - 1) {
				Tratta temp = tratte[j];
				tratte[j] = tratte[j - 1];
				tratte[j - 1] = temp;
			}
		}
	}
	
	printf("Tratte ordinate per data\n");
	
	
}

/**
 Ordinamento delle tratte rispetto al codice
 */
void ordina_codice(int numero_tratte, Tratta *tratte){
	
	
	/* Ordino i codici con un bubble sort modificato, sfruttando l'aritmetica dei puntatori */
	for (int i = 0; i < numero_tratte - 1; i ++) {
		for (int j = numero_tratte - 1; j > 0; j--) {
			if (atoi(tratte[j].codice_tratta + 3) < atoi(tratte[j - 1].codice_tratta + 3)) {
				Tratta temp = tratte[j];
				tratte[j] = tratte[j - 1];
				tratte[j - 1] = temp;
			}
		}
	}
	
	printf("Tratte ordinate per codice\n");
	
}

/**
 Ordimento delle tratte rispetto alla stazione di partenza
 */
void ordina_partenza(int numero_tratte, Tratta *tratte){
	
	/* Ordino con un bubble sort adattato alle stringhe */
	for (int i = 0; i < numero_tratte - 1; i ++) {
		for (int j = numero_tratte - 1; j > 0; j--) {
			/* Per ordinare alfabeticamente sfutto la strcmp*/
			if (strcmp(tratte[j].partenza, tratte[j - 1].partenza) < 0) {
				Tratta temp = tratte[j];
				tratte[j] = tratte[j - 1];
				tratte[j - 1] = temp;
			}
		}
	}
	printf("Tratte ordinate per stazione di partenza\n");
}

/**
 Ordimento delle tratte rispetto alla stazione di arrivo
 */
void ordina_arrivo(int numero_tratte, Tratta *tratte){
	
	/* Ordino con un bubble sort adattato alle stringhe */
	for (int i = 0; i < numero_tratte - 1; i ++) {
		for (int j = numero_tratte - 1; j > 0; j--) {
			if (strcmp(tratte[j].destinazione, tratte[j - 1].destinazione) < 0) {
				Tratta temp = tratte[j];
				tratte[j] = tratte[j - 1];
				tratte[j - 1] = temp;
			}
		}
	}
	
	printf("Tratte ordinate per stazione di arrivo\n");
}

/**
 Se l'ordinamento precedente è stato fatto per stazioni di partenza, utilizzo una ricerca dicotomica, più efficente su un vettore ordinato
 */
void ricerca_partenza_dicotomica(int numero_tratte, Tratta *tratte){
	
	char stazione_partenza[MAX_SIZE];
	printf("Inserire la stazione (di partenza) che desideri cercare: ");
	scanf("%s", stazione_partenza);
	
	int start = 0;
	int end = numero_tratte - 1;
	int mid;
	int index = -1;
	
	for	(; start <= end ;){
		
		mid = (end - start) / 2 + start;
		
		if (strstr(tratte[mid].partenza, stazione_partenza) != NULL) {
			index = mid;
			end = mid - 1;
		} else if (strcmp(tratte[mid].partenza, stazione_partenza) > 0){
			end = mid - 1;
			
		} else {
			start = mid + 1;
		}
	}

	if (index != -1) {
		stampa_tratta(tratte[index]);
		for (int i = 1; strstr(tratte[index + i].partenza, stazione_partenza) != NULL; i++) stampa_tratta(tratte[index + i]);
	} else {
		printf("Non sono state trovate partenze alla stazione inserita\n");
	}
	
}

/**
 Se le tratte non sono ordinate per il vettore di partenza, ricerco in maniera linare
 */
void ricerca_arrivo(int numero_tratte, Tratta *tratte){
	
	char stazione_partenza[MAX_SIZE];
	printf("Inserire la stazione (di partenza) che desideri cercare: ");
	scanf("%s", stazione_partenza);
	int stampato = 0;
	
	for (int i = 0; i < numero_tratte; i ++){
		if (strstr(tratte[i].partenza, stazione_partenza) != NULL){
			stampa_tratta(tratte[i]);
			stampato = 1;
		}
	}
	
	if (!stampato)
		printf("Non sono state trovate partenze alla stazione inserita\n");
	
}

void dealloca_log_tratte(Tratta** log_tratte, int numero_ordinamenti, int numero_tratte){
	
	for (int i = 0; i < numero_ordinamenti; i++) free(log_tratte[i]);
	
	free(log_tratte);
	
}
