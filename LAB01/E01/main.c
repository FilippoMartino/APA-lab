//
//  main.c
//  E01
//
//  Created by Filippo Martino on 30/09/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DAYS_NUMBER 20
#define MAX_TEAMS_NUMBER 20




/* Defnizione della struttura per gestione punteggio squadra */
typedef struct {
    int row_index;
    int score;
}Teams;

/* Il nome del file su cui effettuare operazioni lo prendo da linea di comando */
int main(int argc, const char * argv[]) {
    
    /* Controllo su input utente */
    if (argc < 2){
        printf("ERROR: USAGE %s FILE_NAME\n", argv[0]);
        return -1;
    }
    
    char* file_name = strdup(argv[1]);
    int team_number, day_number, score, max_score = 0, vinner_id = 0s;
    FILE* fp = fopen(file_name, "r");
    
    /* Controllo sull'apertura del file */
    if (fp == NULL){
        printf("Errore nell'aperutra del file\n");
        return -1;
    }
    
    /*
     Leggo le dimensioni della martice delle squadre e le inserisco via via nella
     struttura, mi servira quindi solo il numero di righe (per la dichiarazione
     della strutttura), per dichiarare la grandezza
     del vettore, il numero di colonne lo salvo in una variabile, mi servirà per
     il ciclo di incremento punteggi
    */
    
    fscanf(fp, "%d %d", &team_number, &day_number);
    
    /* Dichiarazione della struttura */
    Teams teams[team_number];
    
    /* Riempio la struttura */
    for (int i = 0; i < team_number; i++) {
        teams[i].row_index = i + 1;
        /* Devo inizializzare il punteggio di ogni squadra prima di inserirne */
        teams[i].score = 0;
        for (int j = 0; j < day_number; j++) {
            fscanf(fp, "%d", &score);
            teams[i].score += score;
        }
    }
    
    /* Stampo la squadra vincitrice */
    for (int i = 0; i < team_number; i++) {
        if (teams[i].score > max_score){
            max_score = teams[i].score;
            vinner_id = teams[i].row_index;
        }
    }
    
    printf("La squadra vincitrice è la %d, con %d punti\n", vinner_id, max_score);
    
    /* Libero le variabili allocate dinamicamente */
    free(file_name);
    fclose(fp);
    
    
    return 0;
}
