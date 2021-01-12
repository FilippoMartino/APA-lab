#include "Graph.h"

#define MAXC 10
#define MAX_WT INT_MAX

/* Definizione struttura per matrice delle adiacenze */
struct M_graph {
	int V; 		/* Numero di vertici */
	int E;	 	/* Numero di archi*/
	int** mat;	/* Matrice delle adiacenze */
	ST tab;		/* Tabella di simboli */
};

/* Definizione strutture per rappresentazione lista */
typedef struct node* link;

/* Singolo nodo */
struct node {
	int v;
	int wt;
	link next;
};

/* Grafo con lista */
struct L_graph {
	int V;
	int E;
	link* adj_list;
	ST tab;
	link z;
};

/* Funzioni statiche per rappresnetazione a matrice delle adiacenze */

static int** matrix_init(int r, int c, int val);
static void  M_insert_E(M_Graph g, Edge e);

/* Funzioni statiche per rappresentazione a lista */
static link new_node(int v, int wt, link next);


/* Funzioni statiche comuni alle due rappresentazioni */
static Edge  EDGEcreate(int v, int w, int wt) {
	Edge e;
	e.v = v;
	e.w = w;
	e.wt = wt;
	return e;
}

/**
 Inizializza una matrice delle adiacenze
 @param r numero righe
 @param c numero colonne
 @param val valore di inizializzazione
 */
int** matrix_init(int r, int c, int val) {
	
	int **t = (int**) malloc(sizeof(int*) * r);
	if (t == NULL)
		return NULL;
	
	/* Allocazione memoria per la matrice */
	for (int i = 0; i < r; i++) {
		t[i] = (int*) malloc(sizeof(int) * c);
		if (t[i] == NULL)
			return NULL;
	}
	
	/* Inizializzazione */
	for (int i = 0; i < r; i++)
	for (int j = 0; j < c; j++)
	t[i][j] = val;
	
	return t;
	
}

M_Graph MAT_GRAPH_init(int v){
	
	M_Graph g = (M_Graph) malloc(sizeof(M_Graph));
	
	if (g == NULL) {
		printf("Errore allocazine memoria [GRAP]\n");
		free(g);
		return NULL;
	}
	
	g->V = v;
	g->E = 0;
	g->mat = matrix_init(v, v, INT_MAX);
	
	if (g->mat == NULL)
		return NULL;
	
	g->tab = ST_init(v);
	
	if (g->tab == NULL)
		return NULL;
	
	return g;
	
}
void  MAT_GRAPH_free(M_Graph g){
	
	if (g == NULL)
		return;
	
	ST_free(g->tab);
	
	for (int i = 0; i < g->V ; i++)
	free(g->mat[i]);
	
	free(g->mat);
	free(g);
	
}
M_Graph MAT_GRAPH_load(FILE* fp){
	
	int v, id_1, id_2, wt;
	Nodo node_1, node_2;
	
	/* Lettura numero di veritici ed inizializzazione nuovo grafo */
	fscanf(fp, "%d", &v);
	M_Graph g = MAT_GRAPH_init(v);
	
	/* Lettura ed inserimento nomi dei nodi */
	for (int i = 0; i < v; i++) {
		fscanf(fp, "%s %s", node_1.pc_name, node_1.sub_net);
		ST_insert(g->tab, node_1);
	}
	
	for (;fscanf(fp, "%s %s %s %s %d", node_1.pc_name, node_1.sub_net, node_2.pc_name, node_2.sub_net, &wt) == 5;) {
		id_1 = ST_search(g->tab, node_1);
		id_2 = ST_search(g->tab, node_2);
		
		if (id_1 >= 0 && id_2 >= 0){
			MAT_GRAPH_insert_E(g, id_1, id_2, wt);
		}
		
		
	}
	
	return g;
	
}
void  MAT_GRAPH_insert_E(M_Graph g, int id_1, int id_2, int wt){
	M_insert_E(g, EDGEcreate(id_1, id_2, wt));
}
int MAT_GRAPH_getIndex(M_Graph g, Nodo node){
	
	int index = ST_search(g->tab, node);
	
	if (index == -1)
		printf("Il nodo cercato non esiste\n");
	
	return index;
	
}
void  MAT_GRAPH_edges(M_Graph g, Edge *a){
	
	int E = 0;
	
	/*
	 Durante la visita della matrice delle adiacenze
	 ci è suficiente visitare le codiagonali superiori,
	 essendo la matrice simmetrica, ecco perchè w = v + 1
	 */

	for (int v = 0; v < g->V; v++)
		for (int w = v + 1; w < g->V; w++)
			if (g->mat[v][w] != MAX_WT)
				a[E++] = EDGEcreate(v, w, g->mat[v][w]);
	
}
void MAT_GRAPH_print_edges_by_id(M_Graph g, Edge* a, int index, FILE* fout){
	
	for (int i = 0; i < g->E; i++) {
		if (ST_search(g->tab, ST_search_by_index(g->tab, a[i].v)) == index || ST_search(g->tab, ST_search_by_index(g->tab, a[i].w)) == index) {
			fprintf(fout, "{%s <-> %s} ", ST_search_by_index(g->tab, a[i].v).pc_name, ST_search_by_index(g->tab, a[i].w).pc_name);
		}
	}
	fprintf(fout, "\n");
	
}
void  MAT_GRAPH_store(M_Graph g, FILE *fp){
	
	Nodo node_1;
	Nodo node_2;
	
	/* Inizializziamo un vettore di archi, in cui salveremo quelli corrispondenti */
	Edge* a = (Edge*) malloc(sizeof(Edge) * g->E);
	if (a == NULL) {
		printf("Errore allocazione memoria [GRAP.c]\n");
		return;
	}
	
	/* Riempiamo il vettore di archi con la funzione corrispondente */
	MAT_GRAPH_edges(g, a);
	
	/* Iniziamo la procedura di stampa, mantenendo il formato già specificato in passato */
	
	/* Stampa del numero di veritici */
	fprintf(fp, "Numero nodi: %d\n\n", g->V);
	
	printf("Nome nodi:\n");
	/* Stampa nome dei vertici */
	for (int i = 0; i < g->V; i++){
		node_1 = ST_search_by_index(g->tab, i);
		fprintf(fp, "%s - %s\n", node_1.pc_name, node_1.sub_net);
		
	}
	
	/* Stampa archi */
	printf("\nArchi:\n");
	for (int i = 0; i < g->E; i++){
		node_1 = ST_search_by_index(g->tab, a[i].v);
		node_2 = ST_search_by_index(g->tab, a[i].w);
		fprintf(fp, "%s [%s] %s [%s] -> %d\n",
				node_1.pc_name,
				node_1.sub_net,
				node_2.pc_name,
				node_2.sub_net,
				a[i].wt);
	}
	
	free(a);
	
}
void  MAT_GRAPH_print_alph(M_Graph g, FILE *fp){
	
	/* Inizializzazione vettore di archi */
	Edge* a = (Edge*) malloc(sizeof(Edge) * g->E);
	if (a == NULL) {
		printf("Errore allocazione memoria [GRAP.c]\n");
		return;
	}
	
	MAT_GRAPH_edges(g, a);
	
	/* Ordinamento vertici in ordine alfabetico */
	Nodo* node_array = (Nodo*) malloc(sizeof(Nodo) * g->V);
	if (node_array == NULL) {
		printf("Errore allocazione memoria [GRAP.c]\n");
		return;
	}
	
	for (int i = 0; i < g->V; i++)
		node_array[i] = ST_search_by_index(g->tab, i);
	
	Nodo temp;
	
	for (int i = 0; i < g->V; i++) {
		for (int j = i + 1; j < g->V; j++) {
			if (strcmp(node_array[i].pc_name, node_array[j].pc_name) > 0) {
				temp = node_array[i];
				node_array[i] = node_array[j];
				node_array[j] = temp;
			}
		}
	}
	
	/* Stampa */
	for (int i = 0; i < g->V; i++){
		fprintf(fp, "%s - %s\n", node_array[i].pc_name, node_array[i].sub_net);
		MAT_GRAPH_print_edges_by_id(g, a, ST_search(g->tab, node_array[i]), fp);
		fprintf(fp, "\n");
	}
	
	free(a);
	
	
	
}
int MAT_IS_SUBGRAPH(M_Graph g, Nodo* node_array, int size){
	
	Edge* a = (Edge*) malloc(sizeof(Edge) * g->E);
	MAT_GRAPH_edges(g, a);
	
	int link = 0;
	
	for (int i = 0; i < size; i++) {
		
		for (int j = 0; j < g->E; j++) {
			
			if (!strcmp(ST_search_by_index(g->tab, a[j].v).pc_name, node_array[i].pc_name) &&
			!strcmp(ST_search_by_index(g->tab, a[j].w).pc_name, node_array[i + 1].pc_name))
				link ++;
			
			if (!strcmp(ST_search_by_index(g->tab, a[j].v).pc_name, node_array[i + 1].pc_name) &&
			!strcmp(ST_search_by_index(g->tab, a[j].w).pc_name, node_array[i].pc_name))
				link ++;
			
		}
	}
	
	free(a);
	
	if (link >= size - 1)
		return 1;
	else
		return 0;
	
	
}

/* ### Funzioni con grafo rappresentato con lista delle adiacenze ### */

L_Graph LIST_GRAPH_init(int v){
	
	L_Graph graph = malloc(sizeof(L_Graph));
	if (graph == NULL) {
		printf("Errore allocazione memoria\n");
		free(graph);
		return NULL;
	}
	
	graph->V = v;
	graph->E = 0;
	
	graph->z = new_node(-1, 0, NULL);
	if (graph->z == NULL) {
		printf("Errore allocazione memoria\n");
		free(graph->z);
		return NULL;
	}
	
	graph->adj_list = malloc(sizeof(link) * v);
	if (graph->adj_list == NULL) {
		printf("Errore allocazione memoria\n");
		free(graph->adj_list);
		return NULL;
	}
	
	for (int i = 0; i < v; i++)
		graph->adj_list[i] = graph->z;
	
	graph->tab = ST_init(v);
	
	return graph;
	
}
void  LIST_GRAPH_free(L_Graph g){
	
	link temp;
	
	for (int i = 0; i < g->V; i++) {
		for (link node = g->adj_list[i]; node != g->z; node = temp) {
			temp = node->next;
			free(node);
		}
	}
	
	ST_free(g->tab);
	free(g->adj_list);
	free(g->z);
	
	return;
}
void  LIST_GRAPH_store(L_Graph g, FILE* fout){
	
	/* Allocazione spazio per array di archi */
	Edge* a = (Edge*) malloc(sizeof(Edge) * g->E);
	if (a == NULL) {
		printf("Errore allocazione memoria\n");
		return;
	}
	
	/* Riempimento array */
	LIST_GRAPH_edges(g, a);
	
	fprintf(fout, "Numero vertici %d\n", g->V);
	fprintf(fout, "Nome:\n");
	
	Nodo n_1, n_2;
	
	for (int i = 0; i < g->V; i++) {
		n_1 = ST_search_by_index(g->tab, i);
		fprintf(fout, "%s - %s\n", n_1.pc_name, n_1.sub_net);
	}
	printf("\n");
	
	printf("Archi:\n");
	for (int i = 0; i < g->V; i++) {
		n_1 = ST_search_by_index(g->tab, a[i].v);
		n_2 = ST_search_by_index(g->tab, a[i].w);
		fprintf(fout, "%s [%s] %s [%s] -> %d\n", n_1.pc_name, n_1.sub_net, n_2.pc_name, n_2.sub_net, a[i].wt);
	}
	
	
}
void  LIST_GRAPH_edges(L_Graph g, Edge* a){
	
	link node;
	int E = 0;
	
	for (int i = 0; i < g->V; i++)
		for (node = g->adj_list[i]; node != g->z; node = node->next)
			if (i < g->V)
				a[E++] = EDGEcreate(i, node->v, node->wt);
}

/* FUNZIONE CONVERSIONE DA MAT A LIST */
L_Graph GET_LIST_GRAPH(M_Graph g){
	
	L_Graph graph = LIST_GRAPH_init(g->V);
	
	
	if (graph == NULL)
		return NULL;
	
	graph->E = g->E;
	graph->tab = ST_dup(g->tab);
	
	for (int i = 0; i < g->V; i++) {
		for (int j = i; j < g->V; j++) {
			if (g->mat[i][j] != MAX_WT) {
				graph->adj_list[i] = new_node(j, g->mat[i][j], graph->adj_list[i]);
			}
		}
	}
	
	
	return graph;
	
}

/* FUNZIONI INTERNE A GRAPH.C */

/* Gestisce l'inserimento di un arco nel grafo (nella rispettiva matrice di adiacenze) */
static void  M_insert_E(M_Graph g, Edge e) {
	int v = e.v, w = e.w, wt = e.wt;
	
	/* Controllo se non ho già un arco nella posizione specificata, nel caso incremento*/
	if (g->mat[v][w] == MAX_WT)
		g->E ++;
	
	/* Vado ad inserire la corrispondenza nella matrice, dualmente */
	g->mat[v][w] = wt;
	g->mat[w][v] = wt;
	
}

/* Alloca lo spazio, inizializza con i parametri passati e restituisce un nuovo nodo */
static link new_node(int v, int wt, link next){
	
	link node = (link) malloc(sizeof(link));
	
	if (node == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	node->v = v;
	node->wt = wt;
	node->next = next;
	
	return node;
}

