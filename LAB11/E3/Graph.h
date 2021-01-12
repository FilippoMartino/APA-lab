#ifndef Graph_h
#define Graph_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "ST.h"

typedef struct edge {
	int v;
	int w;
	int wt;
} Edge;

typedef struct M_graph* M_Graph;
typedef struct L_graph* L_Graph;

void graph_load(FILE* fin);

/* ## Metodi grafi implementati con matrice delle adiacenze ## */

/**
 Inizializza un grafo [MAT]]
 @param v numero veritci
 */
M_Graph MAT_GRAPH_init(int v);

/**
 Libera lo spazio occupato da un grafo [MAT]
 @param g grafo da cancellare
 */
void  MAT_GRAPH_free(M_Graph g);

/**
 Carica un grafo leggendo da un file con uno specifico formato, di seguito riportato:
 - Prima riga numero di vertici
 - Seguono n righe con, una per riga, i nomi dei veritici
 - Seguono gli archi, composti nel seguente -> #nodo_1 #nodo_2 #peso
 Esempio:
	 _________________
	 |				|
	 |		5		|
	 |		A		|
	 | 		B		|
	 |		C		|
	 | 	      A B 5	 	|
	 |   	     B C 2	   	|
	 |   	     A C 3    		|
	 |________________	|
 
 @param fp puntatore al file da leggere
 */
M_Graph MAT_GRAPH_load(FILE* fp);

/**
 Si occupa dell'inserimento di un arco nel grafo
 @param g il grafo in cui inserire l'arco
 @param id_1 nodo di partenza
 @param id_2 nodo di arrivo
 @param wt peso arco
 */
void  MAT_GRAPH_insert_E(M_Graph g, int id_1, int id_2, int wt);

/**
 Cerca il nodo specificato come paramentro, se lo trova ne restituisce l'indice (nella ST), in caso contrario -1 (avvisa con un errore)
 @param g grafico nel quale cercare
 @param label nome del nodo di cui si vuole sapere l'indice
 */
int   MAT_GRAPH_getIndex(M_Graph g, Nodo label);

/**
 Inserisce in un dato "arco" *a gli archi del grado g
 ! L'allocazione della memoria dell'array a deve essere fatta dal client
 @param g grafo dal quale estrarre gli archi
 @param a array di archi (deve già essere stato dimensionato dal client) in cui verranno inseriti gli archi di g
 */
void  MAT_GRAPH_edges(M_Graph g, Edge* a);

/**
 Dato un grafo, l'insieme dei suoi archi, l'id del vertice interessato e dove stampare, permette di stampare tutti
 gli archi che insistono su quel vertice
 @param g il grafo in questione
 @param a array di archi
 @param index indice del vertice da stampare
 @param fout puntatore a file sul quale stampare
 */
void MAT_GRAPH_print_edges_by_id(M_Graph g, Edge* a, int index, FILE* fout);

/**
 Stampa il grafo con lo stesso formato del file in lettura specificato nella funzione MAT_GRAPH_load(...);
 @param g grafo da stampare
 @param fp puntatore a file su cui stampare (STDOUT per stampare a console)
 */
void  MAT_GRAPH_store(M_Graph g, FILE *fp);

void  MAT_GRAPH_print_alph(M_Graph g, FILE *fp);

int MAT_IS_SUBGRAPH(M_Graph g, Nodo* node_array, int size);

/* ## METODI GRAFI IMPLEMENTATI CON LISTA ## */

L_Graph LIST_GRAPH_init(int v);
void  LIST_GRAPH_free(L_Graph g);
void  LIST_GRAPH_insertE(L_Graph g, int id_1, int id_2, int wt);
void  LIST_GRAPH_store(L_Graph g, FILE* fout);
void  LIST_GRAPH_edges(L_Graph g, Edge* a);

/* ## METODI DI CONVERSIONE DA LISTA A MATRICE ## */
/**
 Converte un grafo realizzato con matrice delle adiacenze in un grafo realizzato con lista delle adiacenze
 @param g grafo realizzato con matrice
 */
L_Graph GET_LIST_GRAPH(M_Graph g);


#endif /* Graph_h */
