#include "titles.h"

#define MAX_FILE_NAME 20

typedef enum {
	R,
	S,
	SFD,
	SBD,
	SMM,
	BBST,
	END,
	DEFAULT,
} _comando;

const int command_number = 7;

const char* comandi[command_number] = {
	"R",
	"S",
	"SFD",
	"SBD",
	"SMM",
	"BBST",
	"END"
};

void print_info(void);
_comando get_comando(void);
FILE* get_file_by_user(void);
void search(TL tl);
void search_for_date(TL tl);
void search_between_date(TL tl);
void search_max_min_abs(TL tl);
void balance(TL tl);

int main(int argc, char* argv[]){
	
	if (argc < 2) {
		printf("Error! PARAM FILENAME%s\n", argv[0]);
		return -1;
	}
	
	FILE* fin = fopen(argv[1], "r");
	
	TL tl = TL_init();
	
	TL_store(&tl, fin);
	
	fclose(fin);
	
	print_info();
	
	_comando comando = get_comando();
	
	for (; comando != END ;){
		
		switch (comando) {
			case R:
				TL_store(&tl, get_file_by_user());
				printf("Lista aggiornata\n");
				break;
			case S:
				search(tl);
				break;
			case SFD:
				search_for_date(tl);
				break;
			case SBD:
				search_between_date(tl);
				break;
			case SMM:
				search_max_min_abs(tl);
				break;
			case BBST:
				balance(tl);
				break;
			default:
				printf("Comando errato\n");
				break;
		}
		
		comando = get_comando();
		
	}
	TL_free(tl);
	return 0;
}

_comando get_comando(void){
	
	char string[20];
	printf("Inserire comando: ");
	scanf("%s", string);
	
	for (int i = 0; i < command_number; i++)
	if (!strcmp(string, comandi[i]))
		return i;
	
	return DEFAULT;
	
	
}

void print_info(void){
	printf("%s: acquisizione del contenuto di un file contenente un insieme di transazioni\n", comandi[0]);
	printf("%s: ricerca di un titolo azionario (ricerca in lista)\n", comandi[1]);
	printf("%s: ricerca, dato un titolo precedentemente selezionato, della sua quotazione in una certa data\n", comandi[2]);
	printf("%s: ricerca, dato un titolo precedentemente selezionato, della sua quotazione minima e massima in un certo intervallo di date\n", comandi[3]);
	printf("%s: ricerca, dato un titolo precedentemente selezionato, della quotazione minima e massima lungo tutto il periodo registrato\n", comandi[4]);
	printf("%s: dato un titolo precedentemente selezionato, bilanciamento dell'albero di quotazioni\n", comandi[5]);
	printf("%s: uscita dal programma\n", comandi[6]);
}

FILE* get_file_by_user(void){
	char filename[MAX_FILE_NAME];
	
	printf("Insere nome del file: ");
	scanf("%s", filename);
	FILE* f = fopen(filename, "r");
	
	if (f == NULL) {
		printf("Errori durante l'apertura del file %s\n", filename);
		return NULL;
	}
	
	return f;
	
	
}

void search(TL tl){
	
	char code[MAX_CODE_SIZE];
	char c;
	
	printf("Inserire codice del titolo da cercare: ");
	scanf("%s", code);
	fflush(stdin);
	printf("Si desidera stampare il titolo? [Y/N]: ");
	scanf("%c", &c);
	
	Title title = TL_search_by_index(tl, code);
	
	if (title != NULL)
		printf("Titolo trovato\n");
	else
		return;
	
	if (c == 'Y')
		title_show(title);
}

void search_for_date(TL tl){
	
	char code[MAX_CODE_SIZE];
	
	printf("Inserire codice del titolo da cercare: ");
	scanf("%s", code);
	
	Title title = TL_search_by_index(tl, code);
	
	if (title == NULL)
		return;
		
	printf("Inserire la data [YYYY/MM/DD HH:MM]: ");
	Date date;
	
	scanf("%d/%d/%d %d:%d", &date.year, &date.month, &date.day, &date.hour, &date.minutes);
	
	QUOTE quote = Q_BST_get_quote_in_date(title_get_bst(title), date);
	
	printf("Massima quotazione per %s in data ", code);
	date_show(date);
	printf(": ");
	QUOTE_show(quote);
	printf("\n");
	
}

void search_between_date(TL tl){
	
	char code[MAX_CODE_SIZE];
	
	printf("Inserire codice del titolo da cercare: ");
	scanf("%s", code);
	
	Title title = TL_search_by_index(tl, code);
	
	if (title == NULL)
		return;
		
	printf("Inserire la prima data [YYYY/MM/DD HH:MM]: ");
	Date date_1;
	scanf("%d/%d/%d %d:%d", &date_1.year, &date_1.month, &date_1.day, &date_1.hour, &date_1.minutes);
	
	printf("Inserire la seconda data [YYYY/MM/DD HH:MM]: ");
	Date date_2;
	scanf("%d/%d/%d %d:%d", &date_2.year, &date_2.month, &date_2.day, &date_2.hour, &date_2.minutes);
	
	title_print_max_min_quote_between_date(title, date_1, date_2);
}

void search_max_min_abs(TL tl){
	
	char code[MAX_CODE_SIZE];
	
	printf("Inserire codice del titolo da cercare: ");
	scanf("%s", code);
	
	Title title = TL_search_by_index(tl, code);
	
	if (title == NULL)
		return;
	
	title_print_max_min_quote(title);
	
}

void balance(TL tl){
	
	char code[MAX_CODE_SIZE];
	
	printf("Inserire codice del titolo da cercare: ");
	scanf("%s", code);
	
	Title title = TL_search_by_index(tl, code);
	
	if (title == NULL)
		return;
	
	title_balance_bst(title);
	printf("BST del titolo bilanciato\n");
	
}

