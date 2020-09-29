#include <stdio.h>
#include <string.h>

//TODO implementare controllo lunghezza nome file < MAX_SIZE

#define MAX_FILENAME_SIZE 20
#define MAX_BUFFER_SIZE 1024
#define MAX_STRING_SIZE 20
#define VOWELS_NUMBER 2

/*  Definisco un array con le vocali  */
const char VOWELS[5] = {'a', 'e', 'i', 'o', 'u'};

int conta(char *, int);

int main(int argc, char const *argv[]) {

    char file_name[MAX_FILENAME_SIZE];
    int row_number;
    char string[MAX_STRING_SIZE];
    /* Substring number */
    int n;
    int total_substring_number;

    printf("Nome del file (max 20 caratteri): ");
    scanf("%s", file_name);


    FILE *fp = fopen(file_name, "r");
    fscanf(fp, "%d", &row_number);

    printf("\nInserire lunghezza sottostrigha da cercare: ");
    scanf("%d", &n);

    for (int i = 0; i < row_number; i++) {
        fscanf(fp, "%s", string);
        total_substring_number += conta(string, n);

        /* Devo svuotare la stringa */
        for(int j = 0; j < 20; j++)
            string[j] = 0;       

    }

    fclose(fp);

    printf("Il numero totale di sottostringhe nel file è: %d\n", total_substring_number);

    return 0;

}

int conta(char S[20], int n) {

    /*  
        Controllo rapido se la lunghezza della stringa è minore della sottostringa richiesta 
        mi conviene direttamente uscire
    */
    if (strlen(S) < n)
        return 0;

    int substring_number = 0;
    int spotted_vowel = 0;

    /*
        Divisione della parola in tutte le sue sottostringhe
        possibili secondo la formula:
        sottostringhe possibili = numero lettere - (dimensioni sottostringa + 1) 
    */
    int possible_substring = strlen(S) - n + 1;

    /* Dichiaro la matrice di cui ora conosco le dimensioni */
    char substrings[possible_substring][n];

    /* Inserimento delle sottostringhe nella matrice */
    for(int i = 0; i < possible_substring; i++){
        for(int j = i; j < n + i; j++){
            substrings[i][j - i] = S[j];
        }
        
    }

    /* Conta delle sottostringhe con n vocali  */

    for (int i = 0; i < possible_substring; i++){
        for (int j = 0; j < n; j++){
            
            if (substrings[i][j] == 'a' || substrings[i][j] == 'A')
                spotted_vowel ++;
            if (substrings[i][j] == 'e' || substrings[i][j] == 'E')
                spotted_vowel ++;
            if (substrings[i][j] == 'i' || substrings[i][j] == 'I')
                spotted_vowel ++;
            if (substrings[i][j] == 'o' || substrings[i][j] == 'O')
                spotted_vowel ++;
            if (substrings[i][j] == 'u' || substrings[i][j] == 'U')
                spotted_vowel ++;
            
            if (spotted_vowel == 2){
                substring_number ++;
                spotted_vowel = 0;
            }
        }
        spotted_vowel = 0;
    }

    return substring_number;

}