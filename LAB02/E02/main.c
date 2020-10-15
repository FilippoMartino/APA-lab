//
//  main.c
//  E02
//
//  Created by Filippo Martino on 15/10/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 30
#define COMMAND_NUMBER 6

/* struttura in cui salvare le informazioni relative alla singola tratta */
typedef struct {
	char codice_tratta[MAX_SIZE];
	char partenza[MAX_SIZE];
	char destinazione[MAX_SIZE];
	char data[MAX_SIZE];
	char ora_partenza[MAX_SIZE];
	char ora_arrivo[MAX_SIZE];
	int ritardo;
} Tratta;

/* struttura in cui dividere le parti della data di una singla tratta */
typedef struct {
	int anno;
	int mese;
	int giorno;
} Data;

/* Enumerazione con i comandi */
typedef enum {
	r_date,
	r_partenza,
	r_capolinea,
	r_ritardo,
	r_ritardo_tot,
	r_fine,
} comando_e;


void salva_tratte(FILE*, int, Tratta[MAX_SIZE]);
void stampa_tratta(Tratta);
int get_comando(void);
void seleziona_dati(int, Tratta[MAX_SIZE], comando_e);
void istruzione_1(int, Tratta[MAX_SIZE]);
void istruzione_2(int, Tratta[MAX_SIZE]);
void istruzione_3(int, Tratta[MAX_SIZE]);
void istruzione_4(int, Tratta[MAX_SIZE]);
void istruzione_5(int, Tratta[MAX_SIZE]);

int main(int argc, const char * argv[]) {
	
	/* Controllo parametri linea di comando */
	if (argc < 2) {
		printf("Errore: PARAM %s FILENAME\n", argv[0]);
		return -1;
	}
	
	/* Apertura file con relativo controllo */
	FILE* fp = fopen(strdup(argv[1]), "r");
	if (fp == NULL){
		printf("Errore all'apertura del file\n");
		return -1;
	}
	
	/* Dichiarazione e lettura del numero di tratte */
	int numero_tratte;
	fscanf(fp, "%d", &numero_tratte);
	
	/* Dichiarazione struttura e relativo "riempimento" */
	Tratta tratte[numero_tratte];
	salva_tratte(fp, numero_tratte, tratte);
	
	/* Chiusura file */
	fclose(fp);
	
	/* Acquisizione comando utente */
	comando_e comando = get_comando();
	
	/* Controllo sul comando inserito */
	if (!comando){
		printf("Hai inserito un comando errato\n");
		return -1;
	}
	
	/* Esecuzione comando utente */
	seleziona_dati(numero_tratte, tratte, comando);
	
	return 0;
}

/* Si limita a leggere da file e compilare passo passo l'array di strutture */
void salva_tratte(FILE* fp, int numero_tratte, Tratta tratte[numero_tratte]){
	
	for (int i = 0; i < numero_tratte; i ++) {
		fscanf(fp, "%s %s %s %s %s %s %d",
			   tratte[i].codice_tratta,
			   tratte[i].partenza,
			   tratte[i].destinazione,
			   tratte[i].data,
			   tratte[i].ora_partenza,
			   tratte[i].ora_arrivo,
			   &tratte[i].ritardo);
	}
	
}

/* Comoda funzione per stampare la tratta formattata */
void stampa_tratta(Tratta tratta){
	printf("\nCodice tratta: %s\nPartenza: %s, Destinazione: %s\nData: %s\nOra di partenza: %s\nOra di arrivo: %s\nRitardo: %d\n\n",
		   tratta.codice_tratta,
		   tratta.partenza,
		   tratta.destinazione,
		   tratta.data,
		   tratta.ora_partenza,
		   tratta.ora_arrivo,
		   tratta.ritardo);
}

/*
 "Traduce" il comando inserito dall'utente in un valore presente nell'enumerazione,
 controlla anche se l'utente inserisce una striga non valida, in quel caso ritorna
 0, che andrà gestito dalla funzione chiamante
 */
int get_comando(){
	printf("Inserire comando: ");
	char comando_s[MAX_SIZE];
	scanf("%s", comando_s);
	
	char v[COMMAND_NUMBER][MAX_SIZE] = {"data", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine"};
	
	for (int i = 0; i < COMMAND_NUMBER; i++) {
		if (!strcmp(comando_s, v[i]))
			return i + 1;
	}
	
	return 0;
	
}


/* Applica uno switch per decidere quali funzioni chiamare */
void seleziona_dati(int numero_tratte, Tratta tratte[numero_tratte], comando_e comando){
	
	switch (comando) {
		case 1:
			printf("Hai selezionato 'data'\n");
			/* elencare tutte le corse partite in un certo intervallo di date */
			istruzione_1(numero_tratte, tratte);
			break;
		case 2:
			/* elencare tutti le corse partite da una certa fermata */
			printf("Hai selezionato 'partenza'\n");
			istruzione_2(numero_tratte, tratte);
			break;
		case 3:
			printf("Hai selezionato 'capolinea'\n");
			/* elencare tutti le corse che fanno capolinea in una certa fermata */
			istruzione_3(numero_tratte, tratte);
			break;
		case 4:
			printf("Hai selezionato 'ritardo'\n");
			/* elencare tutte le corse che hanno raggiunto la destinazione in ritardo in un certo intervallo di date*/
			istruzione_4(numero_tratte, tratte);
			break;
		case 5:
			printf("Hai selezionato 'ritardo_tot'\n");
			/* elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta */
			istruzione_5(numero_tratte, tratte);
			break;
		case 6:
			printf("Hai selezionato 'fine'\n");
			break;
		default:
			break;
	}
	
}

/* Cerca quelle tratte comprese tra due date fornite dall'utente, vincolato nella formattazione */
void istruzione_1(int numero_tratte, Tratta tratte[numero_tratte]){
	
	printf("Inserire un intervallo di tempo [formato AAAA/MM/GG], separate da uno spazio:\n");
	Data data_1, data_2;
	int lista_partenze[numero_tratte];
	int anno, mese, giorno;
	char* tok;
	
	for (int i = 0; i < numero_tratte; i++) {
		lista_partenze[i] = 0;
	}
	
	if(!(scanf("%d/%d/%d %d/%d/%d",
		  &data_1.anno,
		  &data_1.mese,
		  &data_1.giorno,
		  &data_2.anno,
		  &data_2.mese,
			 &data_2.giorno) < 6)){
		
		for(int i = 0; i < numero_tratte; i++){
			
			tok = strtok(tratte[i].data, "/");
			anno = atoi(tok);
			tok = strtok(NULL, "/");
			mese = atoi(tok);
			tok = strtok(NULL, "/");
			giorno = atoi(tok);
			
			if (anno > data_1.anno && anno < data_2.anno){
				lista_partenze[i] = 1;
			}
			
			if (anno >= data_1.anno && anno <= data_2.anno){
				if (mese > data_1.mese && mese < data_2.mese){
					lista_partenze[i] = 1;
				}
			}
			
			if (anno >= data_1.anno && anno <= data_2.anno){
				if (mese >= data_1.mese && mese <= data_2.mese){
					if (giorno >= data_1.giorno && giorno <= data_2.giorno) {
						lista_partenze[i] = 1;
					}
				}
			}
		}
		
		for (int i = 0; i < numero_tratte; i++) {
			if (lista_partenze[i])
				stampa_tratta(tratte[i]);
		}
		
	}else {
		printf("Formato non rispettato\n");
	}

}

/* Si limita a stampare tutte le corse in cui compare la partenza specificata */
void istruzione_2(int numero_tratte, Tratta tratte[numero_tratte]){
	
	char partenza[MAX_SIZE];
	
	printf("Inserire la fermata di partenza: ");
	scanf("%s", partenza);
	
	printf("Corse che partono da %s:\n", partenza);
	
	for (int i = 0; i < numero_tratte; i++) {
		if(!strcmp(tratte[i].partenza, partenza))
			stampa_tratta(tratte[i]);
	}
	
}

/* Stessa cosa che fa la funzione precedente, ma con i capolinea */
void istruzione_3(int numero_tratte, Tratta tratte[numero_tratte]){
	char capolinea[MAX_SIZE];
	
	printf("Inserire la fermata di capolinea: ");
	scanf("%s", capolinea);
	
	printf("Corse che arrivano a %s:\n", capolinea);
	
	for (int i = 0; i < numero_tratte; i++) {
		if(!strcmp(tratte[i].destinazione, capolinea))
			stampa_tratta(tratte[i]);
	}
}

/*
 Si limita a cercare filtrare le corse in ritardo dalle tratte generali,
 salvarle in una nuova struttura e richiamare, "riciclando", la funzione 1
 */
void istruzione_4(int numero_tratte, Tratta tratte[numero_tratte]){
	
	int corse_in_ritardo = 0;
	
	for (int i = 0; i < numero_tratte; i++) {
		if (tratte[i].ritardo)
			corse_in_ritardo ++;
	}
	
	Tratta tratte_in_ritardo[corse_in_ritardo];
	
	int j = 0;
	for (int i = 0; i < numero_tratte; i++) {
		if (tratte[i].ritardo){
			tratte_in_ritardo[j] = tratte[i];
			j++;
		}
	}
	
	printf("Stampa delle corse arrivate in ritardo tra due date\n");
	istruzione_1(corse_in_ritardo, tratte_in_ritardo);
	
}

/* Cerca tutte le tratte con il codice desiderato, ne somma i ritardi e stampa il risultato finale */
void istruzione_5(int numero_tratte, Tratta tratte[numero_tratte]){
	
	char codice[MAX_SIZE];
	int ritardo = 0;
	
	printf("Inserire il codice tratta: ");
	scanf("%s", codice);
	
	for (int i = 0; i < numero_tratte; i++) {
		if (!strcmp(tratte[i].codice_tratta, codice))
			ritardo += tratte[i].ritardo;
	}
	
	printf("Ritardo accumulato dalle tratte con codice %s: %d\n", codice, ritardo);
	
}
