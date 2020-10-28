#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 100
#define MAX_REGEX_SIZE 20

char *cercaRegexp(char *, char *);
/* Caso stringa di caratteri senza metacarattere */
char *generic_sintrg(char*, char*);
/* caso metacarattere -> '.' */
char *single_char_generic(char *, char *);
/* caso metacarattere -> '[]' */
char *single_char(char *, char *);
/* caso metacarattere -> '[^ ]' */
char *single_not_char(char *, char *);
/* caso metacarattere -> '\a' */
char *single_not_upper_char(char *, char *);
/* caso metacarattere -> '\A' */
char *single_upper_char(char *, char *);

void clear_buffer(char *);



int main(int argc, const char * argv[]) {
	
	char src[MAX_STRING_SIZE];
	char regex[MAX_REGEX_SIZE];
	
	printf("Inserire striga sorgente [Input alfanumerico]: ");
	scanf("%s", src);
	printf("Inserire espressione regolare: ");
	scanf("%s", regex);
	
	char *p = cercaRegexp(src, regex);
	
	if (p != NULL)
		printf("Ricorrenza trovata [%s]: %s\n", src, p);
	else
		printf("Non sono presenti ricorrenze\n");
	
	return 0;
}



char *cercaRegexp(char *src, char *regexp){
	
	
	
	/* Gestisco il caso di regex con metacarattere '.' */
	if (strstr(regexp, ".") != NULL)
		return single_char_generic(src, regexp);
	
	/* Gestisco regex con metacarattere '[...]' */
	if (strstr(regexp, "[") && strstr(regexp, "]")) {
		return single_char(src, regexp);
	}
	
	
	return generic_sintrg(src, regexp);
	
}


char *generic_sintrg(char *src, char *regexp){
	return strstr(src, regexp);
}


/**
 Nelle regex il '.' inserito all'interno di una stringa implica la ricerca in un testo
 di tutte le parole contenenti tutti i caratteri precedenti al punto e tutti quelli dopo,
 il . può corrispondere ad un qualsiasi carattere, esempi:
 - 'm.a' -> mia, bimba, impantanata
 */
char *single_char_generic(char *src, char *regexp){
	
	/* Caso in cui si abbia solo '.' come regex*/
	if (regexp[0] == '.' && strlen(regexp) == 1)
		return src;
	
	/* Caso .[***] */
	if (regexp[0] == '.'){
		
		char *p = strstr(src, regexp + 1);
		
		if (p != NULL)
			return src + (strlen(src) - strlen(p) - 1);
		else
			return NULL;
	}
	
	/* Caso [***]. */
	if (regexp[strlen(regexp) - 1] == '.'){
		
		/* Elimino il punto */
		regexp[strlen(regexp) - 1] = '\0';
		char *p = strstr(src, regexp);
		
		if (p != NULL)
			return p;
		else
			return NULL;
	}
	
	/* Caso [***].[***] */
	
	/* Se sono arrivato qui è perchè ho solo più questo caso da gestire */
	
	/* Devo capire a quale posizione ho il . */
	int metachar = 0;
	for (int i = 0; i < strlen(regexp); i++)
	if (regexp[i] == '.')
		metachar = i;
	
	
	char *tok = strtok(regexp, ".");
	char *tok_1 = strdup(tok);
	char *temp = strdup(src);
	char *p = strstr(temp, tok);
	p[1] = '1';
	
	tok = strtok(NULL, ".");
	
	if (p != NULL && strstr(temp, tok) != NULL)
		return strstr(src, tok_1);
	else
		return NULL;
	
	
}

char *single_char(char *src, char *regexp){
	
	/* Caso [] senza caratteri specificati */
	char *empty_case = strstr(regexp, "[");
	if (!strcmp(empty_case, "[]"))
		return NULL;
	
	char before_meta[MAX_REGEX_SIZE];
	char meta[MAX_REGEX_SIZE];
	char after_meta[MAX_REGEX_SIZE];
	
	clear_buffer(before_meta);
	clear_buffer(meta);
	clear_buffer(after_meta);
	
	int is_meta_writed = 0;
	int is_meta_being_writed = 0;
	int is_before_null = 0;
	int is_after_null = 0;
	
	if (regexp[0] == '[')
		is_before_null = 1;
	
	if (regexp[strlen(regexp) - 1] == ']')
		is_after_null = 1;
	
	for (int i = 0; i < strlen(regexp); i++) {
	
		if(regexp[i] != '[' && !is_meta_being_writed && !is_meta_writed && !is_before_null)
			sprintf(before_meta, "%s%c", before_meta, regexp[i]);
		
		if (regexp[i] == '[')
			is_meta_being_writed = 1;
		
		if (is_meta_being_writed && regexp[i] != '[' && regexp[i] != ']') {
			sprintf(meta, "%s%c", meta, regexp[i]);
		}
		
		if (regexp[i] == ']'){
			is_meta_being_writed = 0;
			is_meta_writed = 1;
		}
		
		if (!is_meta_being_writed && is_meta_writed && regexp[i] != ']' && !is_after_null)
			sprintf(after_meta, "%s%c", after_meta, regexp[i]);
		
	}
	
	char my_regex[MAX_STRING_SIZE];
	
	for (int i = 0; i < strlen(meta); i++) {
		sprintf(my_regex, "%s%c%s", before_meta, meta[i], after_meta);
		char* p = strstr(src, my_regex);
		if (p != NULL)
			return p;
	}
	
	/* So sono arrivato qui è perchè non ho trovato ricorrenze della regex nella stringa */
	return NULL;
}

//char *single_not_char(char *src, char *regexp);

//char *single_not_upper_char(char *src, char *regexp);

//char *single_upper_char(char *src, char *regexp);

void clear_buffer(char * buffer){
	for (int i = 0; i < strlen(buffer); i++) {
		buffer[i] = 0;
	}
}
