#include "DAG.h"
#include "PQ.h"
#include <limits.h>

#define maxWT INT_MAX

typedef struct _node* link;

struct _node {
	int v; // nodo a cui il vertice corrente nella lista delle adiacenze sta puntando
	int wt; // peso dell'arco
	link next; // eventuale prossimo nodo
};

struct _dag {
	int V; // numero vertici
	int E; // numero archi
	link* adj_list; // lista delle adiacenze
	ST tab; // tabella di simboli
	link z; // nodo sentinella
	
};

/* alloca (dinamico) e ritorna un nuovo nodo */
static link new_node(int v, int wt, link next){
	
	link node = (link) malloc(sizeof(link));
	node->v = v;
	node->wt = wt;
	node->next = next;
	
	return node;
}

/* alloca e ritorna un arco */
static Edge edge_create(int v, int w, int wt){
	
	Edge edge;
	edge.v = v;
	edge.w = w;
	edge.wt = wt;
	
	return edge;
	
}

static Edge* edges_dup(Edge* edges, int n){
	
	Edge* my_edges = (Edge*) malloc(sizeof(Edge) * n);
	
	for (int i = 0; i < n; i++) {
		my_edges[i].v = edges[i].v;
		my_edges[i].w = edges[i].w;
		my_edges[i].wt = edges[i].wt;
	}
	
	return my_edges;
	
}

/* Duplica un grafo */
static DAG dup_dag(DAG dag){
	
	DAG graph = DAG_init(dag->V);
	
	graph->V = dag->V;
	graph->tab = ST_dup(dag->tab);
	
	
	Edge* edge = (Edge*) malloc(sizeof(Edge) * dag->E);
	DAG_extract_edges(dag, edge);
	
	/* Inizializzazione lista delle adiacenze con nodi sentinella */
	for (int i = 0; i < dag->E; i++)
	DAG_insert_edge(graph, edge[i]);
	
	
	return graph;
	
}

static int is_candidate_ok(DAG dag, int candidate){
	for (int i = 0; i < dag->V; i++)
	for (link temp = dag->adj_list[i]; temp != dag->z; temp = temp->next)
	if (temp->v == candidate)
		return 1;
	return 0;
}

/* Rimuove un nodo da una lista */
static link list_remove_node(link head, int key, link z){
	
	link x;
	link p;
	
	if (head == NULL)
		return NULL;
	
	for (x = head, p = NULL; x != z; p = x, x = x->next){
		if (x->v == key){
			
			if (x == head)
				head = x->next;
			else
				p->next = x->next;
			
			free(x);
			break;
		}
	}
	
	return head;
	
}

/* Controlla se un grafo ha un vertice sink */
static int has_sink(DAG dag, int* vertex){
	
	for (int i = 0; i < dag->V; i++){
		if (dag->adj_list[i] == dag->z && is_candidate_ok(dag, i)){
			*vertex = i;
			return 1;
		}
	}
	
	*vertex = -1;
	return 0;
	
	
}

static void remove_edge(DAG dag, Edge e) {
	
	int v = e.v;
	int w = e.w;
	link x;
	
	if (dag->adj_list[v]->v == w) {
		dag->adj_list[v] = dag->adj_list[v]->next;
		dag->E --;
	}
	else
		for (x = dag->adj_list[v]; x != dag->z; x = x->next)
	if (x->next->v == w) {
		x->next = x->next->next;
		dag->E--;
	}
}


/* Rimuove un verice di tipo sink */
static void remove_sink(DAG dag, int sink){
	
	for (int i = 0; i < dag->V; i++){
		for (link temp = dag->adj_list[i]; temp != dag->z; temp = temp->next){
			if (temp->v == sink){
				dag->adj_list[i] = list_remove_node(dag->adj_list[i], sink, dag->z);
				dag->E = dag->E - 1;
				break;
			}
		}
	}
	
}

/* Funzione ricorsiva per verificare se un grafo è un dag */
static void is_dag_rec(DAG dag, int* is_dag){
	
	
	/* se il grafo non ha archi, per com'è strutturata la ricorsione, è aciclico */
	if (!dag->E){
		*is_dag = 1;
		return;
	}
	
	int to_remove = 0;
	
	/* Se non ha "pozzi" non è aciclico */
	if (!has_sink(dag, &to_remove)){
		*is_dag = 0;
		return;
	}
	
	/* Se sono arrivato qui è perchè ho trovato un "pozzo", devo quindi rimuovere gli archi che vi conducono */
	remove_sink(dag, to_remove);
	
	/* Ricorro */
	is_dag_rec(dag, is_dag);
	
}

/* Wrapper, sfutta la funzione ricorsiva is_dag_rec per verificare se un dato grafo è un dag */
static int is_DAG(DAG dag){
	
	DAG temp = dup_dag(dag);
	
	int is_dag = 0;
	
	is_dag_rec(temp, &is_dag);
	
	DAG_free(dag);
	
	return is_dag;
	
}

/* Passato un insieme di archi ed il loro numero, ne calcola il peso complessivo */
static int get_weight(Edge* edge, int n){
	
	int w = 0;
	
	for (int i = 0; i < n; i++)
	w += edge[i].wt;
	
	return w;
	
	
}

static int is_edges_valid(DAG dag, Edge* edges, int edge_size){
	
	DAG my_dag = dup_dag(dag);
	
	for (int i = 0; i < edge_size; i++)
	remove_edge(my_dag, edges[i]);
	
	if (is_DAG(my_dag)){
		return 1;
	} else {
		return 0;
	}
}

static Edge* powerset(DAG dag, int set_size, int* card){
	
	int pow_set_size = pow(2, set_size);
	
	Edge* edges = malloc(sizeof(Edge) * dag->E);
	DAG_extract_edges(dag, edges);
	
	Edge* set = (Edge*) malloc(sizeof(Edge) * set_size);
	Edge* sol = (Edge*) malloc(sizeof(Edge) * set_size);
	
	int counter = 0;
	
	
	int min_card = 1000;
	int max_weight = 0;
	
	for (int i = 0; i < pow_set_size; i++) {
		
		/* Riempiamo via via l'insieme SINGOLO degli archi */
		for (int j = 0; j < set_size; j++) {
			if (i & (1 << j)){
				set[counter].v = edges[j].v;
				set[counter].w = edges[j].w;
				set[counter].wt = edges[j].wt;
				counter ++;
			}
		}
		
		if (i && is_edges_valid(dag, set, counter)){
			
			/* Controllo se la cardinalità sella soluzione ottenuta è accettabile */
			if (counter <= min_card){
				min_card = counter;
				if (get_weight(set, counter) > max_weight){
					
					max_weight = get_weight(set, counter);
					sol = edges_dup(set, counter);
					*card = counter;
				}
			}
		}
		
		counter = 0;
	}
	
	return sol;
	
}

/* Cerca la distanza massima nell'array dei nodi già visitati */
static int get_max_distance(int* dist, int* is_visited, int v){
	
	/* Necessario controllare nell'array la distanza maggiore */
	int max = -1;
	int max_index = 0;
	
	for (int i = 0; i < v; i++){
		if (!is_visited[i] && dist[i] >= max){
			max = dist[i];
			max_index = i;
		}
	}
	
	return max_index;
}

/* Controlla se esiste l'arco specificato */
static int is_node_present(DAG dag, int v, int w){
	
	link temp = dag->adj_list[v];
	
	for (; temp != dag->z; temp = temp->next)
	if (temp->v == w)
		return 1;
	
	return 0;
}

/* Restituisce, se presente, il peso del nodo w (arco v-w) */
static int get_w_size(DAG dag, int v, int w){
	
	link temp = dag->adj_list[v];
	
	for (; temp != dag->z; temp = temp->next)
	if (temp->v == w)
		return temp->v;
	
	return -1;
}

static void reverse_dijkstra(DAG dag, int starting_point){
	
	/* Vettore con le distanze finali e vettore per il controllo se ho già visitato un nodo */
	int dist[dag->V];
	int is_visited[dag->V];
	
	/* Inizializzazione dei due vettori */
	for (int i = 0; i < dag->V; i++) {
		dist[i] = -1; //inverso dei + infinto che si usano per la ricerca del cammino minimo
		is_visited[i] = 0;
	}
	
	/* Distanza dal nodo sorgente */
	dist[starting_point] = 0;
	
	/* Ciclo per ogni veritce */
	for (int i = 0; i < dag->V - 1; i++) {
		
		/* Ottengo l'indice dell'elemento a distanza massima */
		int w = get_max_distance(dist, is_visited, dag->V);
		
		is_visited[w] = 1;
		
		
		/*
		 Se trovo nella lista di adiacenze relative al verice trattato un valore:
		 - controllo se non l'ho ancora visitato
		 - controllo se la distanza passando dall'arco che sto analizzando è maggiore, in caso affermativo aggiorno
		 */
		
		for (int v = 0; v < dag->V; v++)
			if (!is_visited[v] && is_node_present(dag, w, v) && dist[w] != -1 && (dist[w] + get_w_size(dag, w, v) > dist[v]))
				dist[v] = dist[w] + get_w_size(dag, w, v);
		
	}
	
	/* Stampa dei risultati */
	for (int i = 0; i < dag->V; i++){
		if(dist[i] == -1)
			printf("%c (N/A)\n", ST_search_by_index(dag->tab, i).node_name);
		else
			printf("%c [%d]\n", ST_search_by_index(dag->tab, i).node_name, dist[i]);
	}
	printf("\n");
	
	
}


/*_______________________DAG_______________________*/


/* Allocazione del dag */
DAG DAG_init(int v){
	
	/* Allocazione dinamica */
	DAG graph = (DAG) malloc(sizeof(DAG));
	
	/* Inserimento del numeri di vertici */
	graph->V = v;
	
	/* Inizializzazione numero archi */
	graph->E = 0;
	
	/* Inizializzazione nodo sentinella */
	graph->z = new_node(-1, -1, NULL);
	
	/* Allocazione lista delle adiacenze */
	graph->adj_list = malloc(sizeof(link) * graph->V);
	
	/* Inizializzazione lista delle adiacenze con nodi sentinella */
	for (int i = 0; i < graph->V; i++)
	graph->adj_list[i] = graph->z;
	
	/* Inizializzazione tabella di simboli */
	graph->tab = ST_init(graph->V);
	
	return graph;
	
}

/* Dealloca un DAG */
void DAG_free(DAG dag){
	
	/* Deallocazione lista delle adiacenze */
	for (int i = 0; i < dag->V; i++){
		link temp = dag->adj_list[i];
		link next;
		for (; temp != dag->z;){
			next = temp;
			temp = temp->next;
			free(next);
		}
	}
	
	/* Deallocazione degli altri elementi */
	ST_free(dag->tab);
	free(dag->adj_list);
	free(dag->z);
	free(dag);
	
}

void DAG_insert_edge(DAG dag, Edge e){
	
	/* Inserimento arco - caso gradi orientati (non inserimento speculare su adh_list[w]) */
	dag->adj_list[e.v] = new_node(e.w, e.wt, dag->adj_list[e.v]);
	
	/* Incremento */
	dag->E = dag->E + 1;
	
}

/* "Estrae" gli archi di un grafo, inserendoli nell'array di archi a*/
void DAG_extract_edges(DAG dag, Edge* a){
	
	int E = 0;
	
	link temp;
	
	for (int i = 0; i < dag->V; i++) {
		temp = dag->adj_list[i];
		for (; temp != dag->z; temp = temp->next)
		if (i < dag->V)
			a[E++] = edge_create(i , temp->v, temp->wt);
	}
	
}

/* Carica un DAG letto dal file passato */
DAG DAG_load(FILE* fin){
	
	if (fin == NULL){
		printf("Non è stato possibile leggere il file, puntatore corrotto\n");
		return NULL;
	}
	
	/* Lettura del numero di vertici e relativa istanziazione del dag */
	int v;
	fscanf(fin, "%d\n", &v);
	DAG dag = DAG_init(v);
	
	/* Lettura nome dei vertici ed inserimento nella tabella di simboli */
	Nodo node;
	for (int i = 0; i < v; i++) {
		fscanf(fin, "%c\n", &node.node_name);
		ST_insert(dag->tab, node);
	}
	
	/* Infine - lettura degli archi */
	Edge edge;
	Nodo node_2;
	int wt;
	for (; fscanf(fin, "%c %c %d\n", &node.node_name, &node_2.node_name, &wt) == 3 ;){
		edge.wt = wt;
		edge.v = ST_search(dag->tab, node);
		edge.w = ST_search(dag->tab, node_2);
		DAG_insert_edge(dag, edge);
	}
	
	return dag;
	
}

/* Stampa un DAG */
void DAG_store(DAG dag, FILE* fout){
	
	/* Allocazione spazio per array di archi */
	Edge* a = (Edge*) malloc(sizeof(Edge) * dag->E);
	
	if (a == NULL) {
		printf("Errore allocazione memoria\n");
		return;
	}
	
	/* Riempimento array */
	DAG_extract_edges(dag, a);
	
	
	fprintf(fout, "Numero vertici %d\n", dag->V);
	fprintf(fout, "Vertici:\n");
	
	Nodo n_1, n_2;
	for (int i = 0; i < dag->V; i++) {
		n_1 = ST_search_by_index(dag->tab, i);
		fprintf(fout, "%c\n", n_1.node_name);
	}
	printf("\n");
	
	printf("Archi:\n");
	for (int i = 0; i < dag->E; i++) {
		n_1 = ST_search_by_index(dag->tab, a[i].v);
		n_2 = ST_search_by_index(dag->tab, a[i].w);
		fprintf(fout, "%c %c -> %d\n", n_1.node_name, n_2.node_name, a[i].wt);
	}
	
}

Edge* DAG_get_pow_edges(DAG dag, int* edges_number){
	
	int card;
	Edge* edges = powerset(dag, dag->E, &card);
	
	*edges_number = card;
	
	return edges;
}

/* Wrapper di is_dag per permettere ad un client esterno di accedevi */
int DAG_is_dag(DAG dag){
	return is_DAG(dag);
}

void DAG_remove_edge(DAG dag, int id_1, int id_2) {
	remove_edge(dag, edge_create(id_1, id_2, 0));
}

void DAG_remove_edges(DAG dag, Edge* edges, int edges_number){
	
	for (int i = 0; i < edges_number; i++) {
		remove_edge(dag, edges[i]);
	}
	
}

/* Stampa un insieme di archi */
void DAG_print_edges(DAG dag, Edge* edges, int size){
	
	for (int i = 0; i < size; i++)
	printf("[%c %c - %d] ", ST_search_by_index(dag->tab, edges[i].v).node_name, ST_search_by_index(dag->tab, edges[i].w).node_name, edges[i].wt);
	
	printf("\n");
	
}

void DAG_max_wheight_path(DAG dag){
	
	for(int i = 0; i < dag->V; i++){
		printf("Distanza [%c] : \n", ST_search_by_index(dag->tab, i).node_name);
		reverse_dijkstra(dag, i);
	}
	
	printf("\n");
	
}
