#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SONG_LENGHT 255

typedef struct {
	int songs_number;
	char** songs;
} Friend;

typedef struct {
	int songs_number;
	char** songs;
} Playlist;

void free_bidimensional_buffer(int, char**);
void free_friends(int, Friend*);
void free_playlists(int, Playlist*);
Friend* read_songs(int, int*, FILE*);
int get_playlist_number(int, Friend*);
void build_playlists(Playlist*, Friend*, int, int*, int, char**);

int main(int argc, const char * argv[]) {
	
	int friends_number;
	int total_song_number;
	int playlists_number;
	
	/* Controllo su parametri passati in input */
	if (argc < 2){
		printf("ERROR PARAM: USAGE %s FILENAME\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	FILE* fp = fopen(argv[1], "r");
	
	fscanf(fp, "%d", &friends_number);
	
	/* Utilizzo la funzione read_songs per leggere tutte le canzoni dal file puntato da fp e salvarle in un array bidimensionale */
	Friend* friends = read_songs(friends_number, &total_song_number, fp);
	
	fclose(fp);
	
	playlists_number = get_playlist_number(friends_number, friends);
	
	Playlist* playlists = malloc(sizeof(Playlist) * playlists_number);
	
	for (int i = 0; i < playlists_number; i++) playlists[i].songs = malloc(sizeof(char**) * friends_number);
	
	for (int i = 0; i < playlists_number; i++)
	for (int j = 0; j < friends_number; j++)
	playlists[i].songs[j] = malloc(sizeof(char*) * MAX_SONG_LENGHT);
	
	int* temp;
	*temp = 0;
	
	char** current_playlist = malloc(sizeof(char**) * friends_number);
	
	for (int i = 0; i < friends_number; i++) {
		current_playlist[i] = malloc(sizeof(char*) * MAX_SONG_LENGHT);
	}
	
	build_playlists(playlists, friends, 0, temp, friends_number, current_playlist);
	
	printf("lista possibili playlist\n");
	for (int i = 0; i < playlists_number; i ++) {
		printf("\n######################\n");
		for (int j = 0; j < friends_number; j++) {
			printf("%s\n", playlists[i].songs[j]);
		}
		printf("######################\n");
	}
	
	
	free_friends(friends_number, friends);
	free_playlists(playlists_number, playlists);
	
	return 0;
}

/**
 Dealloca le singole stirghe di un buffer bidimensionale, nonchè lo spazio occupato dallo stesso buffer
 */
void free_bidimensional_buffer(int n, char** buffer){
	for (int i = 0; i < n; i++) free(buffer[i]);
	free(buffer);
}
/**
 Dealloca un array di tipo Friend
 */
void free_friends(int n, Friend* f){
	
	for (int i = 0; i < n; i ++) free_bidimensional_buffer(f[i].songs_number - 1, f[i].songs);
	free(f);
	
}

/**
 Dealloca un array di tipo Playlist
 */
void free_playlists(int n, Playlist* p){
	for (int i = 0; i < n; i++) free_bidimensional_buffer(p[i].songs_number - 1, p[i].songs);
}

/**
 Legge le canzoni da un file formattato come da consegna, le salva dinamicamente e restituisce il puntatore all'area di memoria corrispondente
 Il numero totale di cazoni trovate va passato in modo dinamico in modo che possa essere modificato
 */
Friend* read_songs(int friends_number, int *total_song_number, FILE *fp){
	
	Friend* friends = malloc(sizeof(Friend*));
	
	/* Controllo sull'allocazione */
	if (friends == NULL) {
		printf("Errore allocazione memoria\n");
		return NULL;
	}
	
	int single_friend_song_number;
	
	/* inizializzo il numero totale di canzioni, in modo che la funzione chiamante non debba preoccuparsi di farlo */
	*total_song_number = 0;
	
	/* Ciclo nei blocchi degli amici */
	for (int i = 0; i < friends_number; i++) {
		
		/* Di volta in volta prendo il numero di canzoni che ogni amico ha scelto */
		fscanf(fp, "%d", &single_friend_song_number);
		
		/* Incremento */
		*total_song_number = *total_song_number + single_friend_song_number;
		
		if (friends == NULL) {
			printf("Errore allocazione memoria\n");
			return NULL;
		}
		
		friends[i].songs_number = single_friend_song_number;
		
		char** songs = malloc(sizeof(char**) * single_friend_song_number);
		
		if (songs == NULL) {
			printf("Errore allocazione memoria\n");
			return NULL;
		}
		
		for (int j = 0; j < single_friend_song_number; j++) {
			
			songs[j] = malloc(sizeof(char*) * MAX_SONG_LENGHT);
			
			if (songs[j] == NULL) {
				printf("Errore allocazione memoria\n");
				return NULL;
			}
			
			fscanf(fp, "%s", songs[j]);
			
		}
		
		friends[i].songs = malloc(sizeof(char**) * single_friend_song_number);
		friends[i].songs = songs;
		
	}
	
	return friends;
	
}

/**
 Restituisce il numero possibile di playlist possibile
 */
int get_playlist_number(int friend_number, Friend* friends){
	
	int playlist_number = 1;
	
	for (int i = 0; i < friend_number; i ++) playlist_number *= friends[i].songs_number;
	
	return playlist_number;
	
}

void build_playlists(Playlist* playlist, Friend* friend, int current_friend, int* current_playlist, int friends_number, char** single_playlist){
	
	if (current_friend == friends_number) {
		char** temp = malloc(sizeof(char**) * friends_number);
		
		for (int i = 0; i < friends_number; i++) {
			temp[i] = malloc(sizeof(char*) * MAX_SONG_LENGHT);
			temp[i] = strdup(single_playlist[i]);
		}
		
		playlist[*current_playlist].songs = temp;
		*current_playlist = *current_playlist + 1;
		return;
	}
	
	for (int i = 0; i < friend[current_friend].songs_number; i++) {
		single_playlist[current_friend] = strdup(friend[current_friend].songs[i]);
		current_friend ++;
		build_playlists(playlist, friend, current_friend, current_playlist, friends_number, single_playlist);
		current_friend --;
		if (current_friend == friends_number) return;
	}
	
	
	
	
	
	

}
