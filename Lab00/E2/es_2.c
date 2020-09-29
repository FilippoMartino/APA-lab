#include <stdio.h>

//TODO implementare controllo lunghezza nome file < MAX_SIZE

#define MAX_FILENAME_SIZE 20
#define MAX_BUFFER_SIZE 1024
#define MAX_STRING_SIZE 20

int conta(char* , int);

int main(int argc, char const *argv[]){

    char file_name[MAX_FILENAME_SIZE];
    int row_number;
    char string[MAX_STRING_SIZE];
    /* Substring number */
    int n;
    int total_substring_number;
    
    printf("Nome del file (max 20 caratteri): ");
    scanf("%s", file_name);

    FILE* fp = fopen(file_name, "r");
    fscanf(fp, "%d", &row_number);
    //debug
    //printf("Row number %d", row_number);

    printf("\nInserire lunghezza sottostrigha da cercare: ");
    scanf("%d", n);

    for (int i = 0; i < row_number; i++) {
        fscanf(fp, "%s", string);
        total_substring_number += conta(string, n);
    }
    

    fclose(fp);
    return 0;
}

/*  Definire un sistema di avanzamento nella stringa che incrementi il numero di 
    sottostringhe trovate ogni volta che vengono individuate due vocali,
    iterare la procedura ricorsivamente fino alla fine della striga */
int conta(char S[20] , int n){
    // Prova aggiunta commento di nuovo
}