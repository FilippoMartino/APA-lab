#include "date.h"

#define DATE_DEFALUT_VALUE -1

/* Inizializzazione data */
Date date_init(int year, int month, int day, int hour, int minutes){
	
	Date date;
	
	date.year = year;
	date.month = month;
	date.day = day;
	date.hour = hour;
	date.minutes = minutes;
	
	return date;
}

/* Restituisce una data di default, comodo per le inizializzazioni */
Date date_get_default(void){
	
	Date date;
	date.year = DATE_DEFALUT_VALUE;
	date.month = DATE_DEFALUT_VALUE;
	date.day= DATE_DEFALUT_VALUE;
	date.hour = DATE_DEFALUT_VALUE;
	date.minutes = DATE_DEFALUT_VALUE;
	
	return date;
}

/* Legge una data */
Date read_date(FILE* fin){
	
	int y, m, d, h, min;
	
	fscanf(fin, "%d/%d/%d %d:%d", &y, &m, &d, &h, &min);
	
	return date_init(y, m, d, h, min);
	
}

/* Stampa data in formato ?IT */
void date_show_IT(Date date){
	printf("%d/%d/%d %d:%d\n", date.day, date.month, date.year, date.hour, date.minutes);
}

/* Stampa data nel formato specificato dal problema */
void date_show(Date date){
	printf("%d/%d/%d %d:%d", date.year, date.month, date.day, date.hour, date.minutes);
}

/* Funzione di confronto di due date */
int date_cmp(Date date_1, Date date_2){
	
	/* Gestione caso di due date uguali */
	if (date_1.year == date_2.year &&
		date_1.month == date_2.month &&
		date_1.day == date_2.day &&
		date_1.hour == date_2.hour &&
		date_1.minutes == date_2.minutes)
		return 0;
	
	if (date_1.year > date_2.year)
		return -1;
	else if (date_2.year > date_1.year)
		return 1;
	
	if (date_1.month > date_2.month)
		return -1;
	else if (date_2.month > date_1.month)
		return 1;
	
	if (date_1.day > date_2.day)
		return -1;
	else if (date_2.day > date_1.day)
		return 1;
	
	if (date_1.hour > date_2.hour)
		return -1;
	else if (date_2.hour > date_1.hour)
		return 1;
	
	if (date_1.minutes > date_2.minutes)
		return -1;
	else if (date_2.minutes > date_1.minutes)
		return 1;
	
	
	return -1;
		
}
