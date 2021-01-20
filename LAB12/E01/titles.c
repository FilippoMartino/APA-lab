#include "titles.h"

#define S 2

/* Definizione singolo titolo */
struct _title {
	char* code;
	int transaction_number;
	Transaction* transaction_array;
	Q_BST qutes_bst;
};

/* Definizione lista di titoli */
struct _title_list {
	Title title;
	TL next;
};


/* Fondere in uno solo */
void bound_titles(Title* target, Title title);

/* Nuovo nodo in lista */
static TL TL_new_node(Title title, TL next);

/* Inizializzazione di un titolo */
Title title_init(void){
	
	Title title = (Title) malloc(sizeof(Title));
	
	title->code = (char*) malloc(sizeof(char) * (MAX_CODE_SIZE + 1));
	title->transaction_array = NULL;
	title->transaction_number = 0;
	title->qutes_bst = Q_BST_init();
	
	return title;
	
}

/* Legge un titolo dato il puntatore ad un file aperto in lettura */
Title title_read(FILE* fin){
	
	/* Inizializzazione titolo*/
	Title title = title_init();
	
	/* Leggo il codice in una variabile temporanea */
	char code[MAX_CODE_SIZE];
	
	/* Lettura del codice e del numero di transizioni */
	fscanf(fin, "%s %d", code, &title->transaction_number);
	
	/* Copia del codice e allocazione array transizioni */
	title->code = strdup(code);
	if (!title->transaction_number)
		return title;
	
	title->transaction_array = (Transaction*) malloc(sizeof(Transaction) * title->transaction_number);
	
	/* Lettura singole transizioni */
	for (int i = 0; i < title->transaction_number; i++) {
		title->transaction_array[i].date = read_date(fin);
		fscanf(fin, "%d %d", &title->transaction_array[i].value, &title->transaction_array[i].quantity);
	}
	
	title->qutes_bst = Q_BST_load(title->transaction_array, title->transaction_number);
	
	return title;
	
}

/* Semplice funzione di stampa */
void title_show(Title title){
	
	printf("Titolo %s - Transizioni [%d]:\n", title->code, title->transaction_number);
	for (int i = 0; i < title->transaction_number; i++){
		printf("- ");
		date_show(title->transaction_array[i].date);
		printf(" value: %d, quantity: %d\n", title->transaction_array[i].value, title->transaction_array[i].quantity);
	}
	printf("Quotazioni:\n");
	Q_BST_visit(title->qutes_bst, INORDER);
	
}

/* Confronta due titoli */
int title_cmp(Title t_1, Title t_2){
	
	int title_1 = 0;
	int title_2 = 0;
	
	/* Questa stringa di controllo impone di ignorare tutti i caratteri finchè non si arriva ad un numero */
	sscanf(t_1->code, "%*[^0123456789]%d", &title_1);
	sscanf(t_2->code, "%*[^0123456789]%d", &title_2);
	
	if (title_1 > title_2)
		return -1;
	else if (title_2 > title_1)
		return 1;
	else
		return 0;
	
}

void title_print_quote_by_date(Title title, Date date){
	
	QUOTE my_quote = Q_BST_get_quote_in_date(title->qutes_bst, date);
	
	if (my_quote.value == QUOTE_set_default().value)
		printf("Non è stata trovata una quotazione per la data fornita\n");
	else
		QUOTE_show(my_quote);
	
}

void title_print_max_min_quote_between_date(Title title, Date date_1, Date date_2){
	
	QUOTE max_quote = Q_BST_get_max_quote_between(title->qutes_bst, date_1, date_2);
	
	printf("Quotazione massima: ");
	if (max_quote.value == -1)
		printf("Nessuna quotazione massima trovata nell'intervallo specificato\n");
	else
		QUOTE_show(max_quote);
	
	QUOTE min_quote = Q_BST_get_min_quote_between(title->qutes_bst, date_1, date_2);
	
	printf("Quotazione minima: ");
	if (min_quote.value == -1)
		printf("Nessuna quotazione minima trovata nell'intervallo specificato\n");
	else
		QUOTE_show(min_quote);
	
}

void title_print_max_min_quote(Title title){
	
	Date date_1 = date_get_min();
	Date date_2 = date_get_max();
	
	QUOTE max_quote = Q_BST_get_max_quote_between(title->qutes_bst, date_1, date_2);
	
	printf("Quotazione massima: ");
	if (max_quote.value == -1)
		printf("Nessuna quotazione massima trovata nell'intervallo specificato\n");
	else
		QUOTE_show(max_quote);
	
	QUOTE min_quote = Q_BST_get_min_quote_between(title->qutes_bst, date_1, date_2);
	
	printf("Quotazione minima: ");
	if (min_quote.value == -1)
		printf("Nessuna quotazione minima trovata nell'intervallo specificato\n");
	else
		QUOTE_show(min_quote);
}

void title_free(Title title){
	
	Q_BST_free(title->qutes_bst);
	free(title->code);
	free(title->transaction_array);
	free(title);
	
}

/* Inizializzazione lista */
TL TL_init(void){
	
	TL tl = (TL) malloc(sizeof(TL));
	
	if (tl == NULL){
		printf("Errore allocazione memoria [Titles.c]\n");
		return NULL;
	}
	
	tl->title = title_init();
	tl->next = NULL;
	
	return tl;
	
}

/* Istanzia un nuovo nodo con le specifiche indicate */
static TL TL_new_node(Title title, TL next){
	
	TL tl = TL_init();
	tl->title = title;
	tl->next = next;
	
	return tl;
	
}

/* Inserisce un titolo in lista, tiene conto dell'ordine */
void TL_insert(TL* tl, Title title){
	
	TL my_tl = *tl;
	TL first = *tl;
	
	if (my_tl == NULL){
		my_tl = TL_init();
		my_tl = TL_new_node(title, NULL);
		tl = &my_tl;
		return;
	}
	
	
	if (!TL_size(my_tl)){
		my_tl = TL_new_node(title, NULL);
		*tl = my_tl;
		return;
	}
	
	for (TL temp; my_tl->next != NULL; my_tl = my_tl->next) {
		
		if(!title_cmp(my_tl->title, title)){
			bound_titles(&my_tl->title, title);
			*tl = first;
			return;
		}
		
		if(title_cmp(my_tl->next->title, title) == -1){
			temp = my_tl->next;
			my_tl->next = TL_new_node(title, my_tl->next);
			my_tl->next->next = temp;
			*tl = first;
			return;
			
		} else if (!title_cmp(my_tl->next->title, title)){
			bound_titles(&my_tl->next->title, title);
			*tl = first;
			return;
		}
		
	}
	
	my_tl->next = TL_new_node(title, NULL);
	
	*tl = first;
	
}

Title TL_search_by_index(TL tl, char* code){
	
	Title title = title_init();
	
	/* Scorro la lista */
	for (; tl != NULL; tl = tl->next) {
		
		/* Se trovo il titolo che stavo cercando */
		if (!strcmp(tl->title->code, code)) {
			
			/* Copio */
			
			title->code = strdup(tl->title->code);
			title->transaction_number = tl->title->transaction_number;
			
			title->transaction_array = (Transaction*) malloc(sizeof(Transaction) * title->transaction_number);
			
			for (int i = 0; i < title->transaction_number; i++) {
				title->transaction_array[i].date = tl->title->transaction_array[i].date;
				title->transaction_array[i].quantity = tl->title->transaction_array[i].quantity;
				title->transaction_array[i].value = tl->title->transaction_array[i].value;
			}
			
			title->qutes_bst = tl->title->qutes_bst;
			
			/* Ritorno */
			
			return title;
		}
	}
	
	/* In caso contrario avviso l'utente, dealloco il titolo che avevo allocato e restituisco NULL */
	printf("Non sono presenti titoli con il codice specificato!\n");
	title_free(title);
	return NULL;
	
}

/* Aggiunge titoli ad una lista (vuota o piena che sia) leggendoli dal file passato */
void TL_store(TL* tl, FILE* fin){
	
	if (tl == NULL)
		*tl = TL_init();
	
	int title_number = 0;
	fscanf(fin, "%d", &title_number);
	
	Title title = NULL;
	
	for (int i = 0; i < title_number; i++) {
		title = title_read(fin);
		TL_insert(tl, title);
	}
	
}

/* Conta e ritorna il numero di titoli presenti in lista */
int TL_size(TL tl){
	
	int size = 0;
	
	if (tl->title->code[0] == '\0')
		return 0;
	
	for (; tl != NULL; tl = tl->next, size++);
	
	return size;
	
}

/* Stampa la lista ordinata */
void TL_show(TL tl){
	
	for (; tl != NULL; tl = tl->next){
		title_show(tl->title);
		printf("\n");
	}
	
}

/* Unisce due titoli uguali (a livello di transazioni) */
void bound_titles(Title* target, Title title){
	
	if (title_cmp(*target, title)) {
		printf("Errore: i titoli devono essere uguali\n");
		return;
	}
	
	int size = (*target)->transaction_number;
	int total_size = title->transaction_number + size;
	
	(*target)->transaction_array = (Transaction*) realloc((*target)->transaction_array, sizeof(Transaction) * total_size);
	
	for (int i = size; i < total_size; i++) {
		(*target)->transaction_array[i].date = title->transaction_array[i - size].date;
		(*target)->transaction_array[i].quantity = title->transaction_array[i - size].quantity;
		(*target)->transaction_array[i].value = title->transaction_array[i - size].value;
	}
	
	(*target)->transaction_number = total_size;
	
}

/* Libera lo spazio occupato da una lista di titoli */
void TL_free(TL tl){

	TL temp;
	
	for (; tl != NULL; tl = temp){
		temp = tl->next;
		title_free(tl->title);
		free(tl);
	}
	
}

/* Bilanciamento bst quotazioni */
void title_balance_bst(Title title){
	
	int max = Q_BST_get_max_path_len(title->qutes_bst);
	int min = Q_BST_get_min_path_len(title->qutes_bst);
	
	int rapp = max / min;
	
	if (rapp > S)
		Q_BST_balance(title->qutes_bst);
}

Q_BST title_get_bst(Title title){
	return title->qutes_bst;
}
