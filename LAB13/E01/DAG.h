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
 Funzione di generazione (sfrutta il calcolo combinatorio) dello spazio di soluzione
 comprendente tutti gli insiemi di archi che, se rimossi dal grafo passato, andrebbero
 a generare un DAG, la funzione già restituisce il risultato ottimale richiesto dal testo
 
 @param dag grafo su cui effettuare le operazioni
 @param edges_number cardinalità della soluzione, passata per referenza in modo da poter essere aggiornata dalla funzione
 */
Edge* DAG_get_pow_edges(DAG dag, int* edges_number);

/**
 Rimozione di un singolo arco
 
 @param dag grafo di cui rimuovere un arco
 @param id_1 vertice di partenza
 @param id_2 vertice di arrivo
 */
void DAG_remove_edge(DAG dag, int id_1, int id_2);

/**
 Consenete di rimuovere un insieme di archi da un grafo
 
 @param dag grafo di cui rimuovere insieme di archi
 @param edges array di archi da rimuovere
 @param edges_number numero di archi da rimuovere
 */
void DAG_remove_edges(DAG dag, Edge* edges, int edges_number);

/**
 Funzione che, passato un grafo (paradossale il tipo di dato, sono consapevole di aver costruito una forzatura dal punto
 di vista del nome dei tipi, ma me ne sono accorto alla fine)
 controlla se questo è un grafo aciclico
 
 @param dag grafo da controllare
 
 @return 0 se non è un DAG
 @return 1 se è un DAG
 
 */
int DAG_is_dag(DAG dag);

/**
 Funzione utile per il client, consente di stmpare un insieme di archi
 
 @param dag DAG di cui stampare gli archi
 @param edges insieme degli arcni
 @param size dimensione dell'insieme
 */
void DAG_print_edges(DAG dag, Edge* edges, int size);

/**
 
 */
void DAG_max_wheight_path(DAG dag);

#endif /* DAG_h */

