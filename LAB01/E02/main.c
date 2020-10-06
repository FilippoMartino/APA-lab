//
//  main.c
//  E02
//
//  Created by Filippo Martino on 02/10/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_DICTIONARY_REPLACEMENTS 30
#define MAX_STRING_SIZE 1024
#define RECODED_FILENAME "ricodificato.txt"
#define SPACE_CHAR " "
#define DIVIDER "$"
#define OUT 0
#define IN 1

typedef struct {
    char definizione[MAX_STRING_SIZE];
    char sostituzione[MAX_STRING_SIZE];
} Dictionary;

void fill_dictionary(FILE*, Dictionary[], int);
void build_recoded_file(FILE*, Dictionary[], int);
void recode_word(char[], char[], Dictionary[], int);
void insert_word_in_matrix(char [][MAX_STRING_SIZE], char [], int);
void clear_string(char[]);

unsigned count_words(char*);

int get_dictionary_size(FILE*);


int main(int argc, const char * argv[]) {
    
    /* Per questo esercizio mi servono due file, me li facico passare da linea di comando */
    
    if (argc < 3){
        printf("ERROR: PARAM %s, FILE_SORGENTE FILE_DIZIONARIO\n", argv[0]);
        return -1;
    }
    
    /*
     Allocazione dinamica per i nomi dei file
     */
    char* source_filename = strdup(argv[1]);
    char* dictionary_filaname = strdup(argv[2]);
    
    int dictionary_size;
    
    /* Apro il file di codifica e gestisco eventuali errori */
    FILE* fp = fopen(dictionary_filaname, "r");
    if (fp == NULL) {
        printf("Error opening the %s file\n", dictionary_filaname);
        return -1;
    }
    
    /* Aquisisco la grandezza del dizionario e gestisco eventuali errori */
    dictionary_size = get_dictionary_size(fp);
    if (dictionary_size < 0){
        return -1;
    }
    
    /* Dichiaro la struttura dizionario adesso che so quanto la voglio grande */
    Dictionary my_dictionary[dictionary_size];
    
    /* Riemipio la struttura */
    fill_dictionary(fp, my_dictionary, dictionary_size);
    
    /* Chiudo il file di codifica */
    fclose(fp);
    
    /* Apro il file con il testo da ricordificare e gestisco eventuali errori */
    fp = fopen(source_filename, "r");
    if (fp == NULL) {
        printf("Error opening the %s file\n", source_filename);
        return -1;
    }
    
    /* Chiamo la funzione che creerà il file codificato */
    build_recoded_file(fp, my_dictionary, dictionary_size);
    
    /* Chiudo il file che ho appena codificato */
    fclose(fp);
    
    /* Libero lo spazio occupato dalle variabili allocate dinamicamente */
    free(source_filename);
    free(dictionary_filaname);
    
    return 0;
}

/*
 Acquisisco la grandezza del file dizionario, ovvero il numero di sostituzioni indicate, nel
 caso non rispetti la grandezza specificata o la fscanf non trovi un intero
 nella prima riga, avviso l'utente e restituisco un valore di errore che verrà gestito
 da chi chiama la funzione
 */
int get_dictionary_size(FILE* fp){
    
    int size;
    fscanf(fp, "%d", &size);
    
    
    if (size == 0){
        printf("Errore durante la lettura del numero di definizioni nel dizionario\n");
        return -1;
    }
    
    
    if(size > MAX_DICTIONARY_REPLACEMENTS){
        printf("Errore, il numero di sostituzioni supera la grandezza massima consentita\n");
        return -1;
    }
    
    return size;
    
}

/* Si limita a "compilare" il dizionario */
void fill_dictionary(FILE* fp, Dictionary my_dictionary[], int dictionary_size){
    
    for (int i = 0; i < dictionary_size; i++)
    fscanf(fp, "%s %s", my_dictionary[i].sostituzione, my_dictionary[i].definizione);
    
}

/*
 Funzione più importante, crea il file ricodificato appoggiandosi ad un altra funzione,
 che ricodifica le singole parole
 */
void build_recoded_file(FILE* fp, Dictionary my_dictionary[], int dictionary_size){
    
    /* Apro il file di ricodifica, in scrittura */
    FILE* recoded_file = fopen(RECODED_FILENAME, "w");
    /* Variabile in cui salvo di volta in volta la riga del file da codificare */
    char current_line[MAX_STRING_SIZE] = "";
    /* Variabile in cui inserire il numero di parole in una riga */
    int line_word_number;
    
    /* Ciclo finchè il file da ricordificare non finisce */
    for(;fgets(current_line, MAX_STRING_SIZE, fp) != NULL;){
        
        /* Cerco e rimuovo il carattere '/n' alla fine della riga */
        current_line[strcspn(current_line, "\n")] = 0;
        /* Diciaro le due variabili in cui andro ad inserire le versioni omonime ricodificate */
        char recoded_line[MAX_STRING_SIZE] = "";
        char recoded_word[MAX_STRING_SIZE] = "";
        
        /* Conto quante parole sono presenti sulla riga */
        line_word_number = count_words(current_line);
        /*
         Dichiaro un' array bidimensionale, mi serve per
         agevolarmi il passaggio delle signole parole alla
         funzione di decodifica
         */
        char words[line_word_number][MAX_STRING_SIZE];
        /*
         Uso nuovamente una funzione per riempire l'array appena inizializzato:
         avendo usato la strtok mi sono trovato obbligato, ogni volta che decidevo di
         utilizzarla (in questo caso ho diviso per i ' ' in modo da avere i puntatori
         di ogni "inizio parola") di "farla dall'inizio alla fine", perchè la strtok lavora
         sui puntatori, se richiamata in un altro contesto mischia insieme aree di memoria.
         Ecco spiegata la scelta della matrice: in questo modo posso usare prima la strtok sulla
         riga. Una volta letta tutta posso usare la gestione della memoria della funzione sulla
         singola parola, non posso dividere la riga in sottostringhe (parole) e passarle alla
         funzione di ricodifica della parola, altrimenti strtok gestisce aree di memoria errate.
         */
        insert_word_in_matrix(words, current_line, line_word_number);
        
        /* Ciclo per il numero di parole presenti nella riga */
        for (int i = 0; i < line_word_number; i++) {
            
            /* Funzione di codifica della singola parola */
            recode_word(words[i], recoded_word, my_dictionary, dictionary_size);
            /* Concateno poi di volta in volta le parole codificate */
            strcat(recoded_line, recoded_word);
            strcat(recoded_line, " ");
            
            /* Essendo la stringa un buffer non dinamico, devo pulirlo di volta in volta */
            clear_string(recoded_word);
            
            
        }
        /* Stesso discorso per il buffer in cui salvo la riga */
        clear_string(current_line);
        
        /* Stampo sul file la riga decodificata e il carattere '\n' */
        fprintf(recoded_file, "%s %c", recoded_line, '\n');
        /* Stampa a video (non richiesta), attivabile per debug o per fruizione output immediata */
        //        printf("Riga codificata: %s\n", recoded_line);
        
    }
    
    /* Una volta scritto, chiudo il file */
    fclose(recoded_file);
    
}

/*
 Codifica la singola parola:
 Controllo se esiste una stringa contenuta nel dizionario, se la ricerca è positiva
 procedo alla decodifica che consiste:
 - Individuazione del primo carattere trovato nella definizione del dizionario
 - Sostituzione con un TAG definito da una macro
 - Divisione della parola in token, in base alla posizione del tag
 - Concatenazione delle parti risultanti (prima token, sostituzione del dizionario,
 resto della parola [eventualmente]
 
 Gestisco i casi "particolari", ossia
 - Controllo se la definizione corrisponde all'intera parola e gestisco a parte
 - Controllo se la definizione non e presente, nel caso la parola codificata
 è uguale alla parola non codificata.
 */
void recode_word(char single_word[], char recoded_word[], Dictionary my_dictionary[], int dictionary_size){
    
    /* Punterà alla prima ricorrenza di una parola con la definizione */
    char* pointer;
    /* Punterà ai vari pezzi della parola divisa */
    char* tok;
    /* Variabile di controllo per gestione parole senza sottostringhe presenti nel dizionario */
    int decoded = 0;
    
    /*
     controllo nel ciclo se la sottostringa è presente o meno,
     eseguo le operazioni solo se la parola non è ancora stata decodificata
     */
    for (int i = 0; i < dictionary_size && decoded == 0; i++) {
        
        /* strstr cerca la prima ricorrenza della sottostriga nella stringa */
        pointer = strstr(single_word, my_dictionary[i].definizione);
        
        /* Se ho trovato una ricorrenza procedo alla codifica */
        if (pointer != NULL){
            
            /* Gestisco l'eventuale caso in cui la definizione e la parola siano uguali */
            if (strcmp(single_word, my_dictionary[i].definizione) != 0){
                
                strncpy(pointer, DIVIDER, 1);
                tok = strtok(single_word, DIVIDER);
                strcat(recoded_word, tok);
                strcat(recoded_word, my_dictionary[i].sostituzione);
                tok = strtok(NULL, DIVIDER);
                /*
                 Quando aggiungo eventuali pezzi dopo la codifica, devo far scorrere
                 il puntatore del numero corrispondente ai caratteri della definizione
                 che abbiamo appena sostituito, il -1 tiene conto del fatto che abbiamo
                 usato un carattere per il delimitatore dei token, che è de facto il primo
                 della definizione
                 */
                strcat(recoded_word, tok + (strlen(my_dictionary[i].definizione) - 1));
                decoded = 1;
                
            }else{
                
                strcpy(recoded_word, my_dictionary[i].sostituzione);
                decoded = 1;
                
            }
        }
    }
    
    /* Se non abbiamo decodificato niente, copiamo la parola così com'è */
    if (!decoded) strcpy(recoded_word, single_word);
}

/*
 Questa funzione si limita a dividere la riga @line, composta
 da @word_number parole, in singole parole ed inserirle
 in @matrix
 */
void insert_word_in_matrix(char matrix[][MAX_STRING_SIZE], char line[MAX_STRING_SIZE], int word_number){
    
    char* tok;
    tok = strtok(line, SPACE_CHAR);
    strcpy(matrix[0], tok);
    
    for (int i = 1; i < word_number; i++) {
        tok = strtok(NULL, SPACE_CHAR);
        strcpy(matrix[i], tok);
    }
}

/*
 Funzione generale di conteggio parole, uso un unsigned proprio per la generalità,
 posso riciclarla in molti programmi.
 */
unsigned count_words(char *str){
    
    int state = OUT;
    unsigned wc = 0;  // word count
    
    /* Scansiona tutti i caratteri, uno ad uno */
    for (; *str ;) {
        /* Se il carattere successivo non è la continuazione di una parola, setto lo stato ad OUT */
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = OUT;
        /*
         Se ho invece la continuazione della parola e lo stato era precedentemente ad OUT
         setto lo stato ad IN ed incremento; serve nel caso si sia appena conteggiata una parola
         */
        else if (state == OUT) {
            state = IN;
            ++wc;
        }
        
        /*
         Se non ho trovato un carattere che mi fa credere di essere arrivato a fine parola e
         lo stato prima non era OUT, procedo ad incrementare
         */
        ++str;
    }
    
    return wc;
}

/* "Pulisce" una stringa passata, insernendo 0 in tutte le sue celle */
void clear_string(char string[]){ for (int i = 0; i < strlen(string); i++) string[i] = 0; }
