#include "Graph.h"

#define FILENAME "graph.txt"
#define NODE_NUMBER 3

typedef enum {
	uscita,
	print,
	sottografo,
	list_rapp
} Menu;

int adapt_file(char* filename);
int is_node_present(Nodo* array, int n, Nodo node);
void manage_sub_graph(M_Graph m_graph);
void manage_list_rapp(M_Graph m_graph);

int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		printf("ERROR! PARAM: %s FILE_NAME\n", argv[0]);
		return -1;
	}
	
	/* Adatto il file al modulo di lettura della libreria */
	int node_number = adapt_file((char*) argv[1]);
	
	/* Inizializzazione grafo con matrice delle adiacenze */
	M_Graph m_graph = MAT_GRAPH_init(node_number);
	
	/* Carico il grafo */
	FILE* fp = fopen(FILENAME, "r");
	m_graph = MAT_GRAPH_load(fp);
	fclose(fp);
	
	int comando = 1;
	
	for (;comando;) {
		
		/* Possibili comandi */
		printf("Lista comandi:\n");
		printf("1 -> stampare in ordine alfabetico\n");
		printf("2 -> verificare sottografo\n");
		printf("3 -> generare rappresentazione con lista di adiacenza\n");
		printf("0 -> terminare il programma\n-> ");
		scanf("%d", &comando);
		
		
		switch (comando) {
			case print:
				MAT_GRAPH_print_alph(m_graph, stdout);
				break;
			case sottografo:
				manage_sub_graph(m_graph);
				break;
			case list_rapp:
				manage_list_rapp(m_graph);
				break;
			case uscita:
				break;
			default:
				printf("Comando non valido\n");
				break;
		}
	}
	
	MAT_GRAPH_free(m_graph);
	
	return 0;
}

int adapt_file(char* filename){
	
	/* Devo prima salvarmi tutti i nodi in un array */
	FILE* fp = fopen(filename, "r");
	
	if (fp == NULL) {
		printf("Errore durante l'apertura del file %s\n", filename);
		return 0;
	}
	
	Nodo node_1, node_2;
	Nodo* node_array = (Nodo*) malloc(sizeof(Nodo));
	
	int node_number = 0;
	int wt = 0;
	
	for (; fscanf(fp, "%s %s %s %s %d", node_1.pc_name, node_1.sub_net, node_2.pc_name, node_2.sub_net, &wt) == 5; ) {
		
		if (!is_node_present(node_array, node_number, node_1)) {
			node_array = realloc(node_array, sizeof(Nodo) * (node_number + 1));
			node_array[node_number] = node_1;
			node_number ++;
		}
		
		if (!is_node_present(node_array, node_number, node_2)) {
			node_array = realloc(node_array, sizeof(Nodo) * (node_number + 1));
			node_array[node_number] = node_2;
			node_number ++;
		}
		
	}
	
	fclose(fp);
	
	/* Procedo a copiare ed adattare nel formato della libreria il file */
	FILE* to_read = fopen(filename, "r");
	FILE* to_print = fopen(FILENAME, "w");
	
	fprintf(to_print, "%d\n", node_number);
	
	for (int i = 0; i < node_number; i++)
		fprintf(to_print, "%s %s\n", node_array[i].pc_name, node_array[i].sub_net);
	
	for (;fscanf(to_read, "%s %s %s %s %d\n", node_1.pc_name, node_1.sub_net, node_2.pc_name, node_2.sub_net, &wt) == 5;) {
		fprintf(to_print, "%s %s %s %s %d\n", node_1.pc_name, node_1.sub_net, node_2.pc_name, node_2.sub_net, wt);
	}
	
	
	fclose(to_read);
	fclose(to_print);
	
	return node_number;
}

int is_node_present(Nodo* array, int n, Nodo node){
	
	for (int i = 0; i < n; i++)
		if (!strcmp(node.pc_name, array[i].pc_name) && !strcmp(node.sub_net, array[i].sub_net))
			return 1;
	
	return 0;
	
}

void manage_sub_graph(M_Graph m_graph){
	
	Nodo nodi[NODE_NUMBER];
	for (int i = 0; i < NODE_NUMBER ; i++) {
		printf("Inserire nome nodo [%d]: ", i + 1);
		scanf("%s", nodi[i].pc_name);
		printf("Inserire nome sottorete: ");
		scanf("%s", nodi[i].sub_net);
	}
	if (MAT_IS_SUBGRAPH(m_graph, nodi, NODE_NUMBER))
		printf("Sottografo presente\n");
	else
		printf("Sottografo non presente\n");
}

void manage_list_rapp(M_Graph m_graph){
	
	L_Graph l_graph = GET_LIST_GRAPH(m_graph);
	
	if (l_graph == NULL) {
		printf("Errori durante la conversione a rapp. a lista delle adiacenze\n");
		return;
	}
	
	fflush(stdin);
	
	char c;
	printf("Rappresentazione a lista di adiacenza gerata\nDesiderate stampare? Y/N: ");
	scanf("%c", &c);
	
	if (c == 'Y')
		LIST_GRAPH_store(l_graph, stdout);
	
	LIST_GRAPH_free(l_graph);
	
	return;
	
	
	
}
