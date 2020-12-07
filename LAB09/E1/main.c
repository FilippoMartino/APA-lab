#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int start;
	int end;
	int d;
} Att;

typedef struct {
	Att* v;
	int v_lenght;
	int total_d;
} Att_table;

typedef struct _att_list Att_list;

struct _att_list {
	Att_table* table;
	Att_list* next;
};

Att_list* init_att(Att*, int);
Att_list* scroll_att_list(Att_list*);
Att_list* dup_att_list(Att_list*);

Att_table* init_att_table(Att*, int);
Att_table* dup_att_table(Att_table*);
Att_table* get_max(Att_list*);

Att* dup_att(Att*, int);

int fill_att(char*, Att**);
int is_compatible(Att, Att);

void attSel(int, Att*);
void build_att_list(Att_list*, Att_table*);
void print_table(Att_table*);

int main(int argc, const char * argv[]) {
	
	Att* my_att;
	
	if (argc < 2) {
		printf("Error: PARAM %s FILENAME\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	int att_number = fill_att((char *) argv[1], &my_att);
	
//	for (int i = 0; i < att_number; i++) {
//		printf("Start: %d\nEnd: %d\nDiff: %d\n\n", my_att[i].start, my_att[i].end, my_att[i].d);
//	}
	
	attSel(att_number, my_att);
	
	return 0;
}

Att_list* init_att(Att* v, int v_lenght){
	
	Att_table* att_table = (Att_table*) malloc(sizeof(Att_table));
	
	if (att_table == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	att_table->v_lenght = v_lenght;
	
	int tot_d = 0;
	for (int i = 0; i < v_lenght; i++) {
		tot_d += v[i].d;
	}
	
	att_table->total_d = tot_d;
	att_table->v = dup_att(v, v_lenght);
	
	Att_list* dup = (Att_list*) malloc(sizeof(Att_list));
	
	if (dup == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	dup->table = att_table;
	dup->next = NULL;
	
	return dup;
	
}

Att_list* scroll_att_list(Att_list* att_list){
	
	/* Caso lista vuota */
	if (att_list == NULL)
		return NULL;
	
	/* Caso lista con un solo nodo */
	if (att_list->next == NULL)
		return att_list;
	
	Att_list* current = att_list;
	
	/* Scorro fino all'ultimo nodo */
	for (; current->next != NULL; current = current->next);
	
	return current;
	
}

Att_list* dup_att_list(Att_list* att_list){
	
	Att_list* new_node = (Att_list*) malloc(sizeof(Att_list));
	
	if (new_node == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	new_node->table = dup_att_table(att_list->table);
	new_node->next = NULL;
	
	return new_node;
	
}

Att_table* init_att_table(Att* v, int v_lenght){
	
	Att_table* dup = (Att_table*) malloc(sizeof(Att_table));
	
	if (dup == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	int total_d = 0;
	dup->v_lenght = v_lenght;
	dup->v = dup_att(v, v_lenght);
	
	for (int i = 0; i < v_lenght; i++) total_d += v[i].d;
	
	dup->total_d = total_d;
	
	return dup;
	
}

Att_table* dup_att_table(Att_table* att_table){
	
	Att_table* duplicate = (Att_table*) malloc(sizeof(Att_table));
	
	if (duplicate == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	duplicate->v = dup_att(att_table->v, att_table->v_lenght);
	duplicate->v_lenght = att_table->v_lenght;
	duplicate->total_d = att_table->total_d;
	
	return duplicate;
	
}

Att* dup_att(Att* att, int att_number){
	
	if (att_number == 0) {
		printf("Errore: Numero attese uguale a 0\n");
		return NULL;
	}
	
	Att* dup = (Att*) malloc(sizeof(Att) * att_number);
	
	for (int i = 0; i < att_number ; i++) {
		dup[i].start = att[i].start;
		dup[i].end = att[i].end;
		dup[i].d = att[i].d;
	}
	
	return dup;
	
}

int fill_att(char* file_name, Att** my_att){
	
	FILE* fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		printf("Error opening file %s\n", file_name);
		return 0;
	}
	
	int att_number;
	fscanf(fp, "%d\n", &att_number);
	
	Att* att = (Att*) malloc(sizeof(Att) * att_number);
	
	for (int i = 0; i < att_number; i++) {
		fscanf(fp, "%d %d\n", &att[i].start, &att[i].end);
		att[i].d = att[i].end - att[i].start;
	}
	
	*my_att = att;
	
	fclose(fp);
	
	return att_number;
	
}

int is_compatible(Att att_1, Att att_2){
	
	if (att_1.end <= att_2.start)
		return 1;
	else
		return 0;
	
}

void attSel(int N, Att *v){
	
	Att_table* my_table = NULL;
	int max = 0;
	
	for (int i = 0; i < N; i++) {
		
		Att_list* my_att_list = init_att(&v[i], 1);
		Att_table* my_att_table = init_att_table(v, N);
		build_att_list(my_att_list, my_att_table);
		Att_table* max_table = get_max(my_att_list);
		
		if (max_table->total_d > max) {
			max = max_table->total_d;
			my_table = max_table;
		}
	}
	
	printf("Uno dei possibili insiemi di attività che massimizza la somma: ");
	print_table(my_table);
	
}

void build_att_list(Att_list* head, Att_table* info_table){
	
	Att_list* current = scroll_att_list(head);
	
	for (int i = 0; i < info_table->v_lenght; i++) {
		
		Att_list* next = dup_att_list(current);
		
		if (is_compatible(current->table->v[current->table->v_lenght - 1], info_table->v[i])) {
			
			current->table->v_lenght = current->table->v_lenght + 1;
			current->table->total_d = current->table->total_d + info_table->v[i].d;
			
			current->table->v = realloc(current->table->v, sizeof(Att) * current->table->v_lenght);
			current->table->v[current->table->v_lenght - 1] = *dup_att(&info_table->v[i], 1);
			
			/*
			 Ho lasciato il seguente codice perchè ho pensato che potesse rendere più agevole
			 la correzzione, se i commenti vengono infatti rimossi e si esegue, verranno stampate
			 tutte le combinazioni "legali" che possono essere generate con i dati letti dal file.
			 Tutte le righe sono nodi "tabella" della lista che successivamente consulterò per estrarre
			 il valore migliore
			 */
			
//			printf("current: ");
//			for (int j = 0; j < current->table->v_lenght; j++)
//				printf("(%d,%d) ", current->table->v[j].start, current->table->v[j].end);
//
//			printf("per una somma delle durate pari a: %d\n", current->table->total_d);
			
			build_att_list(current, info_table);
			
			current->next = next;
			current = current->next;
			
		}
		
	}
	
}

Att_table* get_max(Att_list* head){
	
	int max_d = 0;
	Att_table* max_table = NULL;
	
	for (; head != NULL; head = head->next)
	if (head->table->total_d > max_d){
		max_d = head->table->total_d;
		max_table = dup_att_table(head->table);
	}
			
		
	if (!max_d)
		return NULL;
	else
		return max_table;
	
	
}

void print_table(Att_table* table){
	
	for (int j = 0; j < table->v_lenght; j++)
		printf("(%d,%d) ", table->v[j].start, table->v[j].end);
	
	printf("per una somma delle durate pari a: %d\n", table->total_d);
}
