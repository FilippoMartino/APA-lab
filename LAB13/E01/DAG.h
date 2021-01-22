#ifndef DAG_h
#define DAG_h

#include "ST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Struttura per descrivere un arco */
typedef struct edge {
	int v; // vertice di partenza
	int w; // vertice di arrivo
	int wt; // peso arco
}Edge;

/* Struttura per il raggruppamento di un insieme di archi con relativa dimensione del raggruppamento */
typedef struct {
	Edge* edges; // array di archi
	int N; // dimensione dell'array
} Edges_set;

typedef struct _dag* DAG;

/**
 Inizializzazione del dag
 
 @param v numero di vertici del grafo
 */
DAG DAG_init(int v);


/**
 Libera la memoria occupata dal DAG passatp
 
 @param dag DAG da liberare
 */
void DAG_free(DAG dag);

/**
 Permette l'inserimento di un arco nel dag
 
 @param dag DAG in cui inserire l'arco
 @param e arco da inserire
 */
void DAG_insert_edge(DAG dag, Edge e);

/**
 Permette di estrarre gli archi salvati nel dag, che verranno inseriti
 nell'array di archi Edge* a che deve essere allocato dal client
 
 @param dag DAG dal quale estrarre gli archi
 @param a Array di archi allocato dal client
 */
void DAG_extract_edges(DAG dag, Edge* a);

/**
 Legge il DAG da file (nel formato del problema) e lo ritorna

@param fin puntatore a file da leggere
 */
DAG DAG_load(FILE* fin);


/**
 Funzione di stampa
 
 @param dag DAG da stampare
 @param fout puntatore a punto di stampa
 */
void DAG_store(DAG dag, FILE* fout);

/**
 
 */
Edge* DAG_get_pow_edges(DAG dag, int* edges_number);

/**
 
 */
void DAG_remove_edge(DAG dag, int id_1, int id_2);

/**
 
 */
void DAG_remove_edges(DAG dag, Edge* edges, int edges_number);

/**
 
 */
int DAG_is_dag(DAG dag);

/**
 
 */
void DAG_print_edges(DAG dag, Edge* edges, int size);

#endif /* DAG_h */

