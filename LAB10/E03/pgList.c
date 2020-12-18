#include "pgList.h"

typedef struct pgListNode* link;

struct pgListNode {
	pg_t* pgp;
	link next;
};

struct pgList_s {
	link head;
	int N;
};

static link new_node(pg_t pg, link next){
	
	link node = (link) malloc(sizeof(link));
	node->pgp = (pg_t *) malloc(sizeof(pg_t));
	
	strcpy(node->pgp->cod, pg.cod);
	strcpy(node->pgp->nome, pg.nome);
	strcpy(node->pgp->classe, pg.classe);
	node->pgp->b_stat = pg.b_stat;
	node->pgp->eq_stat = pg.eq_stat;
	node->pgp->equip = pg.equip;
	node->next = next;
	
	return node;
	
}

/* creatore e distruttore */
pgList_t pgList_init(void){
	
	pgList_t stack = (pgList_t) malloc(sizeof(stack));
	stack->head = NULL;
	stack->N = 0;
	
	return stack;
	
}
void pgList_free(pgList_t pgList){
	
	for (; pgList->head != NULL;) {
		link temp = pgList->head;
		pgList->head = pgList->head->next;
		pg_clean(temp->pgp);
		free(temp);
		
	}
	
}

/* lettura e scrittura su file */
void pgList_read(FILE *fp, pgList_t pgList){
	
	if (pgList == NULL)
		pgList = pgList_init();
	
	int pg_number = get_file_lenght(fp);

	pg_t pg;
	
	for (int i = 0; i < pg_number; i++) {
		
		pg_read(fp, &pg);
		pgList->head = new_node(pg, pgList->head);
		pgList->N = pgList->N + 1;
	}
	
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
	
	link current = pgList->head;
	
	for (int i = 0; i < pgList->N; i++) {
		pg_print(fp, current->pgp, invArray);
		current = current->next;
	}
	
 }

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg){
	pgList->head = new_node(pg, pgList->head);
	pgList->N = pgList->N + 1;
}

/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod){
	
	link current = pgList->head;
	link temp = NULL;
	
	for (; current != NULL; temp = current, current = current->next) {
		if (!strcmp(current->pgp->cod, cod)) {
			
			/* Nel caso mi trovassi nel primo nodo */
			if (pgList->head == current)
				pgList->head = current->next;
			else
				temp->next = current->next;
			
			pg_clean(current->pgp);
			free(current->pgp);
			free(current);
			
			pgList->N = pgList->N - 1;
			return;
		}
	}
	
	printf("Non è stato trovato nessun personaggio con il codice corrispondente\n");
	
}
/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
	
	link current = pgList->head;
	
	for (int i = 0; i < pgList->N; i++, current = current->next)
		if (!strcmp(current->pgp->cod, cod))
			return current->pgp;
	
	printf("Nessun giocatore trovato con il codice specificato\n");
	return NULL;
	
}

int get_file_lenght(FILE* fp){
	
	int lines = 0;
	char ch;
	char last = '\n';
	
	for(;!feof(fp);) {
		ch = fgetc(fp);
		if(ch == '\n' && last != '\n' && ch != EOF)
			lines++;
		last = ch;
	}
	
	
	
	rewind(fp);
	
	return lines;
}
