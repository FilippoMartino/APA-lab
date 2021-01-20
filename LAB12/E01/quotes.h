#ifndef quotes_h
#define quotes_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"

typedef struct {
	Date date;
	int value;
	int quantity;
} Transaction;

/* L'ITEM di questa libreria */
typedef struct {
	float value;
	Date date;
} QUOTE;

/* Enumerazione per la strategia di visita del BST */
typedef enum {
	INORDER,
	PREORDER,
	POSTORDER,
} Strategy;

typedef struct _qbst* Q_BST;


/*_________________________QUOTE_________________________*/


/**
 Restituisce la quotazione di default (data di default e valore a -1)
 */
QUOTE QUOTE_set_default(void);

/**
 Calcola e ritorna la quotazione relativa ad una o più transazioni
 
 @param tr_array array (anche con solo un elemento) con le transazioni effettuata LO STESSO GIORNO
 @param tr_number numero di transazioni nell'array
 */
QUOTE QUOTE_load(Transaction* tr_array, int tr_number);

/**
 Semplice funzione di stampa
 
 @param quote quote da stampare
 */
void QUOTE_show(QUOTE quote);


/*__________________________BST__________________________*/


/**
 Inizializza un BST adattato per il problema in questione
 */
Q_BST Q_BST_init(void);

/**
Dealloca lo spazio occupato dal BST
 
 @param bst il BST da deallocare
 */
void Q_BST_free(Q_BST bst);

/**
 Aggiunge un nodo in foglia

 @param bst BST a cui aggiungere il nodo
 @param quote Item da aggiungere
 */
void Q_BST_insert_leaf(Q_BST bst, QUOTE quote);

/**
 Permette di generare un BST da una lista di trnasizioni (nel caso di questo problema
 viene chiamata duarante la costruzione del titolo)
 
 @param tr_array array  con le transizioni
 @param tr_number dimensione dell'array
 
 @return Q_BST BST con le quotazioni ordinate per data
 */
Q_BST Q_BST_load(Transaction* tr_array, int tr_number);

/**
 Stampa il BST passato secondo la strategia specificata
 
 @param bst BST da stampare
 @param strategy una delle 3 strategie disponibili (INORDER, PREORDER, POSTORDER)
 */
void Q_BST_visit(Q_BST bst, Strategy strategy);

/**
 Conta gli elementi nel bst
 
 @param bst il BST di cui contare gli elementi
 */
int Q_BST_count(Q_BST bst);

/**
 Verifica se un bst ha elementi o meno
 
 @param bst BST da controllare
 */
int Q_BST_empty(Q_BST bst);

/**
 Restituisce, se presente, la quotazione in una data
 
 @param bst BST sul quale effettuare la ricerca
 @param key data per la quale effettuare la ricerca
 
 @return QUOTE la quotazione se trovata
 @return NULL se non è stato trovato nulla
 */
QUOTE Q_BST_get_quote_in_date(Q_BST bst, Date key);

/**
 Cerca all'interno del bst la quotazione e valore massimo tra due date
 
 @param bst BST sul quale effettuare la ricerca
 @param date_1 limite inferiore
 @param date_2 limite superiore
 */
QUOTE Q_BST_get_max_quote_between(Q_BST bst, Date date_1, Date date_2);


/**
 Cerca all'interno del bst la quotazione e valore min tra due date
 
 @param bst BST sul quale effettuare la ricerca
 @param date_1 limite inferiore
 @param date_2 limite superiore
 */
QUOTE Q_BST_get_min_quote_between(Q_BST bst, Date date_1, Date date_2);

/**
 Ritorna la lunghezza del cammino masismo
 
 @param bst BST del quale trovare il cammino massimo
 */
int Q_BST_get_max_path_len(Q_BST bst);

/**
 Ritorna la lunghezza del cammino minimo
 
 @param bst BST del quale trovare il cammino minimo
 */
int Q_BST_get_min_path_len(Q_BST bst);

/**
 Bilancia il bst passato
 
 @param bst BST da bilanciare
 */
void Q_BST_balance(Q_BST bst);

#endif /* quotes_h */
