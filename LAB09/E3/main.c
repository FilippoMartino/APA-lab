#include "personaggi.h"
#include "items.h"

const char* commands_array[COMMAND_NUMBER] = {
	"load_p",
	"load_e",
	"add_p",
	"del",
	"add_e",
	"cal",
	"fine"};

typedef enum {
	load_player,
	load_equipment,
	add_player,
	delete_player,
	add_eq_to_player,
	get_stat,
	end
} _comando;

_comando get_command(void);

int main(int argc, const char * argv[]) {
	
	Player* my_players = NULL;
	Equipment* my_equipments = NULL;
	
	for (int command = get_command(); command != end; command = get_command()) {
		switch (command) {
			case load_player: new_players(&my_players);
				break;
			case load_equipment: add_equipments(&my_equipments);
				break;
			case add_player: add_player_by_console(&my_players);
				break;
			case delete_player: delete_player_by_id(&my_players);
				break;
			case add_eq_to_player: equip_player(&my_equipments, &my_players);
				break;
			case get_stat: get_player_stat(my_players);
				break;
			case end:
				break;
		}
	}
	
	/* Libero lo spazio allocato per le due liste */
	free_players(my_players);
	free_equipment(my_equipments);
	
	
	return 0;
}


/**
 Gestisce l'input utente
 
 @returns @b _comando una tipologia di comando
 @returns @b -1 se l'utente ha inserito un output sbagliato
 */
_comando get_command(){
	
	char comando_s[MAX_STRING];
	printf("Inserire comando (h per una lista dei possibili comandi): ");
	scanf("%s", comando_s);
	
	if (!strcmp(comando_s, "h")) {
		printf("############################################################\n");
		printf("Lista comandi possibili:\n");
		printf("%s: caricare una lista di personaggi da un file\n", commands_array[0]);
		printf("%s: caricare lista equipaggiamenti da un file\n", commands_array[1]);
		printf("%s: aggiungere un personaggio (da tastiera)\n", commands_array[2]);
		printf("%s: cancellazione di un personaggio\n", commands_array[3]);
		printf("%s: aggiunta equipaggiamento a personaggio\n", commands_array[4]);
		printf("%s: calcolo statistiche di un personaggio\n", commands_array[5]);
		printf("%s: Terminazione del programma\n", commands_array[6]);
		printf("############################################################\n");
		return get_command();
		
	}
	
	for (int i = 0; i < COMMAND_NUMBER; i++){
		if (!strcmp(comando_s, commands_array[i])){
			return i;
		}
	}
	
	printf("Hai inserito un comando non consentito\n");
	return -1;
	
}
