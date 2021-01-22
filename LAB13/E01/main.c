#include "DAG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char * argv[]) {
	
	if (argc < 2){
		printf("ERROR! PARAM %s FILENAME\n", argv[0]);
		return -1;
	}
	
	FILE* fin = fopen((char*) argv[1], "r");
	
	DAG my_dag = DAG_load(fin);
	
	DAG_store(my_dag, stdout);
	
	if (!DAG_is_dag(my_dag)){
		Edge* to_remove = DAG_get_pow_edges(my_dag);
	} else {
		printf("é già un dag\n");
	}
	
	DAG_free(my_dag);
	
	fclose(fin);
	
	return 0;
}
