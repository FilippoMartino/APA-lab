//
//  main.c
//  E03
//
//  Created by Filippo Martino on 07/10/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_SIZE 20
#define MAX_SIZE 30
#define EXIT_STRING "fine"

/* Organizzo in una struttura tutte le informazioni necessarie per un'operazione */
typedef struct {
    char selettore[MAX_SIZE];
    int indice;
    char direzione[MAX_SIZE];
    int posizioni;
} Operazione;

void manipola_matrice(int, int, int [MAX_SIZE][MAX_SIZE], Operazione);
void ruota_vettore(int [], int, int, char [MAX_SIZE]);
void ruota_singola_cella(int[], int, char [MAX_SIZE]);
void stampa_matrice(int, int, int[MAX_SIZE][MAX_SIZE]);

Operazione get_operazione();

int main(int argc, const char * argv[]) {
    
    char nome_file[MAX_FILENAME_SIZE];
    FILE* fp;
    int numero_righe, numero_colonne;
    
    /*
     Per evitare errori al superamento dei 20 caratteri senza usare stringhe
     dinamiche l'unico modo che mi viene in mente è quello di obbligare la scanf
     a leggere solo 20 caratteri, anche se sono consapevole non vada bene perchè hard-coded
     */
    printf("Inserire nome file [MAX 20 caratteri]: ");
    scanf("%20s", nome_file);
    
    /* Provo ad aprire il file e gestisco eventuali errori */
    if((fp = fopen(nome_file, "r")) == NULL){
        printf("Errore all'apertura del file\n");
        return -1;
    }
    
    /* Provo a leggere la grandezza della matrice e gestisco eventuali errori */
    if (fscanf(fp, "%d %d", &numero_righe, &numero_colonne) < 2){
        printf("Errore nella lettura delle dimensioni della matrice\n");
        return -1;
    }
    
    /* Controllo dimensioni righe e colonne */
    if((numero_righe > MAX_SIZE) || (numero_colonne > MAX_SIZE)){
        printf("Le misure specificate nel file superano la dimensione massima consentita\n");
        return -1;
    }
    
    /* Inizializzo e riempio la matrice */
    int matrice[numero_righe][numero_colonne];
    for (int i = 0; i < numero_righe; i++) {
        for (int j = 0; j < numero_colonne; j++) {
            fscanf(fp, "%d", &matrice[i][j]);
        }
    }
    /* Chiudo il file */
    fclose(fp);
    
    /* Dichiaro ed inizializzo una nuova operazione mediante la funzione dedicata */
    Operazione operazione = get_operazione();
    
    /* Ciclo fino a inserimento stringa "fine" nel selettore */
    for (;strcmp(operazione.selettore, EXIT_STRING) != 0;) {
        /* Manipolo la matrice per l'operazione specificata */
        manipola_matrice(numero_righe,
                         numero_colonne,
                         matrice,
                         operazione);
        /* Stampo la matrice ottenuta */
        stampa_matrice(numero_righe, numero_colonne, matrice);
        /* Acquisisco una nuova operazione */
        operazione = get_operazione();
        
    }

    return 0;
}

/* restituisce una struttura operazione inizialilzzata */
Operazione get_operazione(){
    
    Operazione operazione;
    printf("Inserire l'operazione da realizzare, dati richiesti: <selettore> <indice> <direzione> <posizioni>\n");
    scanf("%s %d %s %d", operazione.selettore, &operazione.indice, operazione.direzione, &operazione.posizioni);
    /* Adatto l'indice al c */
    operazione.indice -= 1;
    
    return operazione;
}

/* Tutte le operazioni di manipolazione della matrice richieste */
void manipola_matrice(int righe, int colonne, int matrice[righe][colonne], Operazione operazione){
    
    /*
     Questo for mi serve per ottimizzare i clicli di spostamento: se ho un numero di clicli
     uguale al numero di righe della matrice, lo spostamento "torna al punto di partenza",
     quindi considero validi solo spostamenti inferiori al numero di righe della matrice,
     se sono superiori li riconduco con una semplice sottrazione
     */
    for (; operazione.posizioni > righe; operazione.posizioni -= righe);
    
    
    /* Gestisco i vari casi */
    
    if (strcmp(operazione.selettore, "riga") == 0){
        /*
         Sulle rige è semplice "riciclare il codice", ho già dei vettori,
         e le direzioni sono concordi a quelle utilizzate dalla funzione
         */
        ruota_vettore(matrice[operazione.indice],
                      colonne,
                      operazione.posizioni,
                      operazione.direzione);
        
    }else{
        /* vettore che mi servirà per ruotare la colonna*/
        int v[righe];
        /* Mi copio la colonna interessata nel vettore */
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                if(j == operazione.indice)
                    v[i] = matrice[i][j];
            }
        }
        
        /*
         Nel ruotare la colonna devo considerare una rotazione "vericale", che la
         mia funzione non prevede, devo quindi gestire i "su" come degli spostamenti
         di un vettore verticale trasposto verso sinistra, analogamente i "giu" come
         spostamenti verso destra
         */
        if(strcmp(operazione.direzione, "su") == 0){
            ruota_vettore(v,
                          righe,
                          operazione.posizioni,
                          "sinistra");
        }else{
            ruota_vettore(v,
                          righe,
                          operazione.posizioni,
                          "destra");
        }
        
        /* Adesso sostituisco nella matrice in maniera analoga a come ho copiato la colonna */
        for (int i = 0; i < righe; i++) {
            for (int j = 0; j < colonne; j++) {
                if(j == operazione.indice)
                    matrice[i][j] = v[i];
            }
        }
    }
}

/* Il vettore verrà ruotato (orizzontalmente) di n posizioni a dx o sx */
void ruota_vettore(int vettore[], int dimensioni_vettore, int posizioni, char direzione[MAX_SIZE]){
    
    /* Richiamo la funzione della singola rotazione n volte quante sono le posizioni richieste */
    for (int i = 0; i < posizioni; i++)
        ruota_singola_cella(vettore, dimensioni_vettore, direzione);

}

/* Spostamento del vettore passato di un unità a destra o a sinistra (incremento o decremento) */
void ruota_singola_cella(int vettore[], int dimenioni_vettore, char direzione[MAX_SIZE]){

    int i;
    /* Rotazione verso sinistra */
    if (strcmp(direzione, "sinistra") == 0) {
        
        int temp = vettore[0];
        for (i = 0; i < dimenioni_vettore - 1; i++)
            vettore[i] = vettore[i + 1];
        vettore[i] = temp;
        
    /* Rotazione verso destra*/
    } else {
        
        int v_copy[dimenioni_vettore];
        for (int i = 0; i < dimenioni_vettore; i ++)
            v_copy[i] = vettore[i];
        
        for (i = 0; i < (dimenioni_vettore - 1); i++)
            vettore[i + 1] = v_copy[i];
        
        vettore[0] = v_copy[dimenioni_vettore - 1];
    }
}

/* Ho usato questa funzione principalmente per il debug, ma ho deciso si lasciarla */
void stampa_matrice(int righe, int colonne, int matrice[righe][colonne]){
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("[%d] ", matrice[i][j]);
        }
        printf("\n");
    }
}
