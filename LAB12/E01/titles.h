#ifndef titles_h
#define titles_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"
#include "quotes.h"

#define MAX_CODE_SIZE 20


typedef struct _title* Title;

typedef struct _title_list* TL;

/*_________________________TITLE_________________________*/

/**
 Inizializzazione di titolo
 */
Title title_init(void);

/**
 Fornito il puntatore ad un file aperto in lettura
 inizializza un titolo, ne legge il codice e gestisce
 anche la lettura delle n transazioni, allocate dinamicamente
 
 @param fin puntatore a file aperto un lettura
 */
Title title_read(FILE* fin);

/**
 Si limita a stampare le informazioni riguardo ad un titolo
 
 @param title titolo da stampare
 */
void title_show(Title title);

/**
 Confronta due titoli, basandosi sul codice
 
 @param t_1 primo titolo da confrontare
 @param t_2 secondo titolo da confrontare
 
 @return 0 se i titoli sono uguali
 @return -1 se il primo è maggiore del secondo
 @return 1 se il secondo è maggiore del primo
 */
int title_cmp(Title t_1, Title t_2);

/**
 Stampa la quotazione di un titolo alla data specificata
 
 @param title il titolo del quale stampare la data
 @param date data della quotazione da stampare
 */
void title_print_quote_by_date(Title title, Date date);

/**
 Stampa la quotazione massima e quella minima in un intrevallo di due date (se presente)
 
 @param title titolo del quale stampare le quotazioni
 @param date_1 limite inferiore
 @param date_2 limite superiore
 */
void title_print_max_min_quote_between_date(Title title, Date date_1, Date date_2);

/**
 Stampa la quotazione massima e quella minima assolute
 
 @param title titolo del quale stampare le quotazioni
 */
void title_print_max_min_quote(Title title);

/**
 Bilancia il BST delle quotazioni
 
 @param title titolo di cui si vuole bilanciare l'array delle quotazioni
 */
void title_balance_bst(Title title);

/**
 Restituisce il BST associato al titolo
 
 @param title titolo da cui si vuole estrarre il bst
 */
Q_BST title_get_bst(Title title);

/**
 Libera lo spazio in memoria occupato da un ciclo
 @param title l'adt da deallocare
 */
void title_free(Title title);

/*_________________________TL_________________________*/

/**
 Inizializza una lista di titoli
 */
TL TL_init(void);

/**
 Inserisce un nuovo titolo nella lista ordinata tenendo conto dell'ordine
 @param tl  puntatore a lista in cui inserire
 @param title titolo da inserire in lista
 */
void TL_insert(TL* tl, Title title);

/**
 Legge da un file (formato dato dai file di input) e inserisce
 i dati in una lista ordinata, preferibilmente istanziare la lista tl
 prima con la TL_init(), la funzione gestisce comunque liste non istanziate
 
 @param tl puntatore a lista in cui verranno inseriti i titoli letti
 @param fin puntatore a file che rispetti il formato richiesto, da cui leggere
 */
void TL_store(TL* tl, FILE* fin);

/**
 Restitiusce il numero di elementi presenti in lista
 
 @param tl lista di cui contare gli elementi
 */
int TL_size(TL tl);

/**
 Cerca un titolo nella lista tl sulla base del codice code passato
 
 @param tl lista nella quale cercare il titolo
 @param code codice sulla base del quale cecare
 
 Gestisce la potenziale assenza si un titolo con il codice fornito, genera un messaggio di errore
 
 @return Title: il titolo trovato
 @return NULL in caso non sia presente un titolo con il codice dato
 */
Title TL_search_by_index(TL tl, char* code);

/**
 Semplice funzione di stampa
 @param tl lista da stampare
 */
void TL_show(TL tl);

/**
 Dealloca lo spazio occupato da una lista
 @param tl lista da deallocare
 */
void TL_free(TL tl);

#endif /* titles_h */
