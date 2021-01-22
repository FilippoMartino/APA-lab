#include "DAG.h"

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
	
	int candidate = 0;
	int pass = 0;
	
	for (int i = 0; i < dag->V; i++)
	if (dag->adj_list[i] == dag->z)
		candidate = i;
	
	
	for (int i = 0; i < dag->V; i++)
	for (link temp = dag->adj_list[i]; temp != dag->z; temp = temp->next)
	if (temp->v == candidate)
		pass = 1;
	
	
	if (pass){
		*vertex = candidate;
		return 1;
	} else {
		*vertex = -1;
		return 0;
	}
	
}

static void  remove_edge(DAG dag, Edge e) {
	
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
			}
		}
	}
	
	dag->V = dag->V - 1;
	
	Edge* edges = (Edge*) malloc(sizeof(Edge) * dag->E);
	DAG_extract_edges(dag, edges);
	
	
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
	
	free(temp);
	
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
		remove_edge(my_dag, edges[i])
	
	
	//	printf("Is connected: %d\nIs DAG: %d\n", is_connected(my_dag), is_DAG(my_dag, 0));
	
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
		
		
		//		printf("Powerset generato: ");
		//		for (int i = 0; i < counter; i++)
		//			printf("[%c %c] ", ST_search_by_index(dag->tab, set[i].v).node_name, ST_search_by_index(dag->tab, set[i].w).node_name);
		//		printf("\n");
		
		if (i && is_edges_valid(dag, set, counter)){
			
			/* Controllo se la cardinalità sella soluzione ottenuta è accettabile */
			if (counter <= min_card){
				min_card = counter;
				if (get_weight(set, counter) > max_weight){
					
					printf("Archi accettabili come soluzione: ");
					for (int i = 0; i < counter; i++)
					printf("[%c %c %d] ", ST_search_by_index(dag->tab, set[i].v).node_name,
						   ST_search_by_index(dag->tab, set[i].w).node_name,
						   set[i].wt);
					printf("\n");
					
					
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

Edge* DAG_get_pow_edges(DAG dag){
	
	int card;
	Edge* edges = powerset(dag, dag->E, &card);
	
	return NULL;
}

DAG DAG_remove_edges(DAG dag, Edge* edges, int edge_size){
	
	DAG my_dag = DAG_init(dag->V);
	my_dag->E = 0;
	my_dag->tab = ST_dup(dag->tab);
	
	for (int i = 0; i < dag->E; i++) {
		
		link temp = dag->adj_list[i];
		for (;temp != dag->z; temp = temp->next) {
			
			int can_insert = 1;
			for (int j = 0; j < edge_size; j++)
			if (temp->v == edges[j].w && i == edges[j].v)
				can_insert = 0;
			
			if (can_insert){
				my_dag->adj_list[i] = new_node(temp->v, temp->wt, my_dag->adj_list[i]);
				my_dag->E = my_dag->E + 1;
			}
			
		}
	}
	
	return my_dag;
}

/* Wrapper di is_dag per permettere ad un client esterno di accedevi */
int DAG_is_dag(DAG dag){
	return is_DAG(dag);
}

void DAG_remove_edge(DAG dag, int id_1, int id_2) {
	removeE(G, edge_create(id1, id2, 0));
}
