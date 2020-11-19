#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_graph(int**, int);
int is_vertex_cover(int**, int*, int, int, int);
int contains(int*, int*, int);
void get_vertex_cover(int, int*, int, int, int**);
int* translate_sol(int*, int*, int);

int** get_graph(char*, int*, int*);

int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		printf("Error: PARAM %s FILENAME\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* Numero nodi */
	int node_number;
	/* Numero archi */
	int links_number;
	
	int** graph = get_graph((char*) argv[1], &node_number, &links_number);
	
	int* sol = (int*) malloc(sizeof(int) * node_number);
	
	get_vertex_cover(0, sol, node_number, links_number, graph);
	
	free(sol);
	
	free_graph(graph, links_number);
	
	
	
	return 0;
}

void free_graph(int** graph, int links_numbers){
	for (int i = 0; i < links_numbers; i++) {
		free(graph[i]);
	}
	free(graph);
}

int** get_graph(char* file_name, int* node_number, int* links_number){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Errore all'apertura del file\n");
		exit(EXIT_FAILURE);
	}
	
	fscanf(fp, "%d %d", node_number, links_number);
	int** graph = (int**) malloc(sizeof(int*) * *links_number);
	
	for (int i = 0; i < *links_number; i++) {
		graph[i] = (int*) malloc(sizeof(int) * 2);
		fscanf(fp, "%d %d", &graph[i][0], &graph[i][1]);
	}
	
	fclose(fp);
	
	return graph;
	
}

int is_vertex_cover(int** graph, int* candidate, int candidate_numbers, int links_number, int node_number){
	
	int is_vertex = 1;
	
	for (int i = 0; i < node_number; i++)
	if (!contains(candidate, graph[i], candidate_numbers))
		is_vertex = 0;
	
	return is_vertex;
	
}

int contains(int* candidate, int* link, int candidate_numbers){
	
	for (int i = 0; i < candidate_numbers; i++)
	if (candidate[i] == link[0] || candidate[i] == link[1])
		return 1;
	
	return 0;
	
}

void get_vertex_cover(int pos, int* sol, int node_number, int link_number, int** graph){
	
	if (pos >= node_number) {
		int res_lenght = 0;
		int* res = translate_sol(sol, &res_lenght, pos);
		if (is_vertex_cover(graph, res, res_lenght, link_number, node_number)) {
			printf("{");
			for (int i = 0; i < res_lenght; i++){
				if (i == res_lenght - 1)
					printf("%d}\n", res[i]);
				else
					printf("%d, ", res[i]);
			}
		}
		
		return;
	}
	
	sol[pos] = 0;
	get_vertex_cover(pos + 1, sol, node_number, link_number, graph);
	sol[pos] = 1;
	get_vertex_cover(pos + 1, sol, node_number, link_number, graph);
	
	return;
	
}

int* translate_sol(int* sol, int* res_lenght, int pos){
	
	int counter = 0;
	
	for (int i = 0; i < pos; i++) {
		if (sol[i]) counter ++;
	}

	int* res = (int*) malloc(sizeof(int) * counter);
	
	counter = 0;
	
	for (int i = 0; i < pos; i++){
		if (sol[i]){
			res[counter] = i;
			counter ++;
		}
	}
	*res_lenght = counter;
	
	
	
	return res;
	
}
