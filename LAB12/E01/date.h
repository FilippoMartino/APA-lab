#ifndef date_h
#define date_h

#include <stdio.h>
#include <string.h>
#include <string.h>

typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minutes;
}Date;

/**
 Rendo disponibile una funzione di inizializzazione per velocizzarne il processo
 i parametri sono banali
 */
Date date_init(int year, int month, int day, int hour, int minutes);

/**
 Funzione di confronto di due date
 @param date_1 prima data da confrontare
 @param date_2 seconda data da confrontare
 
 @return 0 se le due date sono uguali
 @return -1 se la prima è maggiore della seconda
 @return 1 se la seconda è maggiore della prima
 
 Per data maggiore intendo la data più recente (indica un tempo maggiore)
 
 */
int date_cmp(Date date_1, Date date_2);

/**
Semplice funzione che ritorna una data di default nulla 
 */
Date date_get_default(void);

/**
 Si occupa, a partire dal puntatore ad un file aperto in lettura
 di leggere, inizializzare e restituire la data letta corrispondente
 Legge la data nel formato imposto dal testo
 */
Date read_date(FILE* fin);

/**
 Stampa un data nel formato abituale
 @param date data da stampare
 */

void date_show_IT(Date date);

/**
 Stampa un data nel formato del testo
 @param date data da stampare
 */
void date_show(Date date);

#endif /* date_h */
