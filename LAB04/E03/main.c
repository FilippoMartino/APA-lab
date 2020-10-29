#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
void fragment_regex(char*, char*, char*, char*, char*);


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
	if (strstr(regexp, "[") && strstr(regexp, "]") && !strstr(regexp, "[^")) {
		return single_char(src, regexp);
	}
	
	/* Gestisco regex con metacarattere [^...]*/
	if (strstr(regexp, "[^") && strstr(regexp, "]")) {
		return single_not_char(src, regexp);
	}
	
	if (strstr(regexp, "\\a")) {
		return single_not_upper_char(src, regexp);
	}
	
	if (strstr(regexp, "\\A")) {
		return single_upper_char(src, regexp);
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
	
	char before[MAX_REGEX_SIZE];
	char after[MAX_REGEX_SIZE];

	
	memmove(before, regexp, metachar);
	memmove(after, regexp + metachar + 1, strlen(regexp) - strlen(before));
	
	
	
	char *p_1 = strstr(src, before);
	if (p_1 == NULL) return NULL;
	
	char temp = src[strlen(src) - strlen(p_1) + 1];
	src[strlen(src) - strlen(p_1) + 1] = '.';
	
	
	char *p_2 = strstr(src, after);
	src[strlen(src) - strlen(p_1) + 1] = temp;
	
	if (p_2 == NULL) return NULL;
	
	return p_1;
	
	
}

char *single_char(char *src, char *regexp){
	
	/* Caso [] senza caratteri specificati */
	char *empty_case = strstr(regexp, "[");
	if (!strcmp(empty_case, "[]"))
		return NULL;
	
	char before_meta[MAX_REGEX_SIZE];
	char meta[MAX_REGEX_SIZE];
	char after_meta[MAX_REGEX_SIZE];
	
	fragment_regex(before_meta, meta, after_meta, regexp, "[]");
	
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

char *single_not_char(char *src, char *regexp){
	
	/* Caso [^] vuoto da solo */
	if (strlen(regexp) == 3 && strstr(regexp, "[^]")) {
		return src;
	}
	
	char before_meta[MAX_REGEX_SIZE];
	char meta[MAX_REGEX_SIZE];
	char after_meta[MAX_REGEX_SIZE];
	
	fragment_regex(before_meta, meta, after_meta, regexp, "[^]");
	
	char my_regex[MAX_STRING_SIZE];
	
	for (int i = 0; i < strlen(meta); i++) {
		sprintf(my_regex, "%s%c%s", before_meta, meta[i], after_meta);
		char* p = strstr(src, my_regex);
		if (p == NULL){
			sprintf(my_regex, "%s%c%s", before_meta, '.', after_meta);
			/* Togliendo il carattere che non deve esserci, lo sostisco con il . e passo alla funzione di ricerca generica */
			return single_char_generic(src, my_regex);
		}
	}
	
	/* Se siamo qui è perchè non abbiamo trovato ricorrenze, ritorno NULL */
	return NULL;
}

char *single_not_upper_char(char *src, char *regexp){
	
	int pos = 0;
	
	if (regexp[0] == '\\') {
		regexp = regexp + 1;
		regexp[0] = '.';
		pos = 0;
	} else {
		
		char *p = strstr(regexp, "\\a");
		unsigned long start_point = strlen(regexp) - strlen(p);
		
		memmove(regexp + start_point, p + 2, strlen(p));
		regexp[start_point] = '.';
		pos = (int) start_point;
	}
	
	char* p = single_char_generic(src, regexp);

	if (!isupper(p[pos])) return p;

	return NULL;
}

char *single_upper_char(char *src, char *regexp){
	
	int pos = 0;
	
	if (regexp[0] == '\\') {
		regexp = regexp + 1;
		regexp[0] = '.';
		pos = 0;
	} else {
		
		char *p = strstr(regexp, "\\A");
		unsigned long start_point = strlen(regexp) - strlen(p);
		
		memmove(regexp + start_point, p + 2, strlen(p));
		regexp[start_point] = '.';
		pos = (int) start_point;
	}
	
	char* p = single_char_generic(src, regexp);

	if (isupper(p[pos])) return p;

	return NULL;
}

void clear_buffer(char * buffer){
	for (int i = 0; i < strlen(buffer); i++) {
		buffer[i] = 0;
	}
}

void fragment_regex(char* before, char* meta, char* after, char* regex, char* regex_type){
	
	clear_buffer(before);
	clear_buffer(meta);
	clear_buffer(after);
	
	int is_meta_writed = 0;
	int is_meta_being_writed = 0;
	int is_before_null = 0;
	int is_after_null = 0;
	
	if (regex[0] == '[')
		is_before_null = 1;
	
	if (regex[strlen(regex) - 1] == ']')
		is_after_null = 1;
	
	for (int i = 0; i < strlen(regex); i++) {
		
		if(regex[i] != '[' && !is_meta_being_writed && !is_meta_writed && !is_before_null)
			sprintf(before, "%s%c", before, regex[i]);
		
		if (regex[i] == '[')
			is_meta_being_writed = 1;
		
		if (is_meta_being_writed && regex[i] != '[' && regex[i] != ']') {
			sprintf(meta, "%s%c", meta, regex[i]);
		}
		
		if (regex[i] == ']'){
			is_meta_being_writed = 0;
			is_meta_writed = 1;
		}
		
		if (!is_meta_being_writed && is_meta_writed && regex[i] != ']' && !is_after_null)
			sprintf(after, "%s%c", after, regex[i]);
		
	}
	
	/* Nel caso in cui il tipo sia [^] devo togliere il primo carattere di meta '^' appunto*/
	if (!strcmp(regex_type, "[^]")) meta = meta + 1;
	
}
