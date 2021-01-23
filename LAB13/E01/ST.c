#include "ST.h"

struct symboltable {
	Nodo* node_array;
	int max_size;
	int size;
};

ST ST_init(int max_n){
	
	ST st = (ST) malloc(sizeof(ST));
	
	if (st == NULL) {
		printf("Errore allocazione memoria [ADT ST]\n");
		return NULL;
	}
	
	st->node_array = (Nodo*) calloc(max_n, sizeof(Nodo));
	
	if (st->node_array == NULL) {
		printf("Errore allocazione memoria [ADT ST]\n");
		free(st);
		return NULL;
	}
	
	st->max_size = max_n;
	st->size = 0;
	
	return st;
}

void ST_free(ST st){
	
	if (st == NULL)
		return;
	
	free(st->node_array);
	free(st);
	
}

int ST_count(ST st){
	return st->size;
}

void ST_insert(ST st, Nodo node){
	
	if (node.node_name == 0){
		printf("Erorre, tentato inserimento nodo nullo\n");
		return;
	}
		
	st->node_array[st->size].node_name = node.node_name;
	st->size ++;
	
}

int ST_search(ST st, Nodo node){
	
	for (int i = 0; i < st->size; i++)
		if (st->node_array != NULL && st->node_array[i].node_name == node.node_name)
			return i;
		
	return -1;
	
}

Nodo ST_search_by_index(ST st, int index){
	
	Nodo empty_node;
	empty_node.node_name = 0;
	
	if (index >= st->max_size || index < 0)
		return empty_node;
	
	return st->node_array[index];
	
}

ST ST_dup(ST st){
	
	ST my_st = (ST) malloc(sizeof(ST));
	
	if (my_st == NULL) {
		printf("Errore allocazione memoria [ADT ST]\n");
		return NULL;
	}
	
	my_st->node_array = (Nodo*) calloc(st->max_size, sizeof(Nodo));
	
	if (my_st->node_array == NULL) {
		printf("Errore allocazione memoria [ADT ST]\n");
		free(my_st);
		return NULL;
	}
	
	my_st->max_size = st->max_size;
	my_st->size = st->size;
	
	my_st->node_array = (Nodo*) malloc(sizeof(Nodo) * st->size);
	
	for (int i = 0; i < st->size; i++) 
		my_st->node_array[i].node_name = st->node_array[i].node_name;
	
	
	return my_st;
}

void ST_delete(ST st, int key){

	Nodo* node_array = (Nodo*) calloc(st->size, sizeof(Nodo));
	
	int counter = 0;
	for (int i = 0; i < st->size; i++){
		if (i != key){
			node_array[counter].node_name = st->node_array[i].node_name;
			counter ++;
		}
	}
	
	st->node_array = node_array;
	st->size --;

	
}
