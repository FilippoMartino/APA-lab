#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char nome[20];
	char cognome[20];
} Studente;

int main(int argc, const char * argv[]) {
	
	printf("Creo una classe di 20 studenti...\n");
	
	Studente *studenti = (Studente*) malloc(sizeof(Studente) * 20);
	
	for (int i = 0; i < 20; i++) studenti[i].id = i;
	
	printf("Stampo id studenti...\n");
	
	for (int i = 0; i < 20; i++) printf("Studente %d, id -> %d\n", i + 1, studenti[i].id);
	
	printf("Classe creata\n");
	
	printf("Creo adesso 5 classi...\n");
	
	Studente ** classi = (Studente**) malloc(sizeof(Studente*) * 5);
	
	for (int i = 0; i < 20; i++) classi[i] = (Studente*) malloc(sizeof(Studente) * 20);
	
	for (int i = 0; i < 5; i++)
	for (int j = 0; j < 20; j++)
	classi[i][j].id = j;
	
	for (int i = 0; i < 5; i++){
		printf("Classe %d\n", i + 1);
		for (int j = 0; j < 20; j++){
			printf("Studente %d, id -> %d\n", j + 1, classi[i][j].id);
		}
	}
	
	
	
	return 0;
}
