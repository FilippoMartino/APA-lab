#include "invArray.h"

typedef struct invArray_s *invArray_t;

struct invArray_s{
	inv_t* invp;
	int size;
};

/* creatore e disruttore */
invArray_t invArray_init(void){
	
	invArray_t my_array = (invArray_t) malloc(sizeof(invArray_t));
	
	if (my_array == NULL) {
		printf("Erorre durante l'allocazione della memoria\n");
		return NULL;
	}
	
	my_array->size = 0;
	
	return my_array;
	
}

void invArray_free(invArray_t invArray){
	
	for (int i = 0; i < invArray->size; i++) {
		free(invArray[i].invp);
	}
	
	free(invArray);
	
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp, invArray_t invArray){
	
	if (fp == NULL) {
		printf("Error: file not valid\n");
		return;
	}
	
	/* Nel caso l'array non fosse stato allocato */
	if (invArray == NULL)
		invArray = invArray_init();
	
	int size = 0;
	
	fscanf(fp, "%d", &size);
	
	if (!size) {
		printf("File size == 0\n");
		return;
	}
	
	invArray->size = size;
	
	invArray->invp = (inv_t*) malloc(sizeof(inv_t) * size);
	
	for (int i = 0; i < size; i++)
		inv_read(fp, &invArray->invp[i]);
	
}

void invArray_print(FILE *fp, invArray_t invArray){
	
	if (fp == NULL){
		printf("Impossibile stampare, puntatore a file non valido\n");
		return;
	}
	
	for (int i = 0; i < invArray->size ; i++)
		inv_print(fp, &invArray->invp[i]);
	
}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
	
	if (fp == NULL){
		printf("Impossibile stampare, puntatore a file non valido\n");
		return;
	}
	
	inv_print(fp, &invArray->invp[index]);
	
}

/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t* invArray_getByIndex(invArray_t invArray, int index){
	return &invArray->invp[index];
}

/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name){
	
	for (int i = 0; i < invArray->size; i++)
		if (!strcmp(invArray->invp[i].nome, name))
			return i;
		
	printf("Nessun elemento trovato\n");
	return -1;
	
}

