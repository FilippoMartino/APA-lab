#ifndef personaggi_h
#define personaggi_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_SIZE 7
#define MAX_STRING 50
#define EQ_SLOT_NUMBER 8
#define COMMAND_NUMBER 7

typedef struct {
	int hp;
	int mp;
	int atk;
	int def;
	int mag;
	int spr;
} Stat;

typedef struct {
	char name[MAX_STRING];
	char class[MAX_STRING];
	Stat eq_stat;
} Item;

typedef struct {
	char id[ID_SIZE];
	char player_name[MAX_STRING];
	char class[MAX_STRING];
	Item player_inv[EQ_SLOT_NUMBER];
	int item_number;
	Stat player_stat;
} Bio;

typedef struct _player Player;

struct _player {
	Bio* bio;
	Player* next;
};

Player* load_player_list(char*);
Player* new_player(char[ID_SIZE], char[MAX_STRING], char[MAX_STRING], Stat);

int get_file_lenght(char*);

void add_player_by_console(Player**);
void new_players(Player**);
void delete_player_by_id(Player**);

void get_player_stat(Player*);
void print_player(Bio);
void print_players(Player*);
void free_players(Player*);

void init_stat(Stat*);
Stat sum_stat(Stat, Stat);

#endif /* personaggi_h */
