#ifndef items_h
#define items_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaggi.h"

typedef struct _equipment Equipment;

struct _equipment {
	Item* item;
	Equipment* next;
};

Equipment* load_equipment_list(char*);
Equipment* new_equipment(char[MAX_STRING], char[MAX_STRING], Stat);

void add_equipments(Equipment**);
void equip_player(Equipment**, Player**);

void print_equipment(Item);
void print_equipment_list(Equipment*);
void free_equipment(Equipment*);

#endif /* items_h */
