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


int date_is_between(Date date_1, Date date_2, Date key){
	
	/* Controllo che la prima data sia più piccola della seconda, in modo da avere un potenziale intervallo*/
	if (date_cmp(date_1, date_2) == -1)
		return 0;
	
	/* Caso in cui la data sia pari ad uno degli estremi dell'intervallo */
	if (!date_cmp(date_1, key) || !date_cmp(date_2, key))
		return 1;
	
	if (date_cmp(date_1, key) == 1 && date_cmp(key, date_2) == 1)
		return 1;
	else
		return 0;
	
}

int date_same_day(Date date_1, Date date_2){
	
	if (date_1.year == date_2.year &&
		date_1.month == date_2.month &&
		date_1.day == date_2.day)
		return 1;
	else
		return 0;
	
}

void day_show(Date date){
	printf("%d/%d/%d", date.year, date.month, date.day);
}

Date date_get_max(void){
	
	Date date;
	date.year = 1000000;
	date.month = 12;
	date.day = 31;
	date.hour = 23;
	date.minutes = 59;
	
	return date;
}

Date date_get_min(){
	Date date;
	date.year = 0;
	date.month = 1;
	date.day = 1;
	date.hour = 1;
	date.minutes = 1;
	
	return date;
}
