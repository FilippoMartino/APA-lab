#ifndef ST_h
#define ST_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 30

/* Struttura che mi serve per salvare le informazioni riguardo al nodo */
typedef struct {
	char pc_name[MAX_SIZE];
	char sub_net[MAX_SIZE];
} Nodo;

typedef struct symboltable* ST;

/**
 Inizializzazione tabella di simboli
 */
ST ST_init(int max_n);

/**
 Libera lo spazio occupato dalla tabella di simboli
 */
void ST_free(ST st);

/**
 Restituisce il numero di elementi nella tabella
 */
int ST_count(ST st);

/**
 Aggiunge un nuovo elemento alla tabella
 */
void ST_insert(ST st, Nodo node);

/**
 Restituisce l'indice corrispondente all'elemento cercato
 */
int ST_search(ST st, Nodo node);

/**
 Dato un indice, restituisce il corrispondente elemento (in questo caso una stringa)
 */
Nodo ST_search_by_index(ST st, int index);

/**
 Duplica una struttura
 */
ST ST_dup(ST st);

#endif /* ST_h */
