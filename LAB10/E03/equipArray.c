#include "equipArray.h"

/* ADT di prima classe collezione di oggetti di equipaggiamento */
typedef struct equipArray_s *equipArray_t;

struct equipArray_s {
	int* equipArray;
	int size;
};

/* creatore e disruttore */
equipArray_t equipArray_init(void){
	
	equipArray_t my_equipArray_t = (equipArray_t) malloc(sizeof(equipArray_t));
	
	if (my_equipArray_t == NULL) {
		printf("Errore durante l'allocazione dinamica della memoria\n");
		return NULL;
	}
	
	my_equipArray_t->size = 0;
	
	return my_equipArray_t;
	
}
void equipArray_free(equipArray_t equipArray){
	
	if (equipArray == NULL)
		return;
	
	
	free(equipArray->equipArray);
	free(equipArray);
	
}

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray){
	return equipArray->size;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
	
	if (fp == NULL) {
		printf("File non valido\n");
		return;
	}
	
	if (!equipArray->size) {
		printf("Nessun equipaggiamento\n");
		return;
	} else {
		
		for (int i = 0; i < equipArray->size; i++){
			
			int index = equipArray->equipArray[i];
			
			inv_t* my_inv = invArray_getByIndex(invArray, equipArray->equipArray[i]);
			
			inv_print(fp, my_inv);
		}
	}
	
	
}
/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray){
	
	if (equipArray->size == EQUIP_SLOT) {
		printf("Questo personaggio ha tutti gli slot occupati\n");
		return;
	}
	
	equipArray->size = equipArray->size + 1;
	equipArray->equipArray = realloc(equipArray->equipArray, sizeof(int) * equipArray->size);
	
	char name[LEN];
	printf("Scegliere nome equipaggiamento da aggiungere: ");
	scanf("%s", name);
	
	int index = invArray_searchByName(invArray, name);
	
	if (index != -1)
		equipArray->equipArray[equipArray->size - 1] = index;
	else
		printf("Non è stato trovato un oggetto dell'inventario corrispondente al nome specificato\n");
	
	return;
	
}
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
	return equipArray->equipArray[index];
}

