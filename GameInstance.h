#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "GameData.h"
#include <time.h>

#define INV_SIZE 4
#define MAX_TIME (60 * 5)
//time in seconds for max duration

enum GameState {
	NOT_IN_USE,
	STARTED,
	//TIMEOUT,
	ENDED
};

struct GameInstance
{
	struct Room roomSelected;
	enum GameState currentState;
	int clientSocket;

	int itemIds[INV_SIZE];
	// -1 se non ha item, altrimenti indice dell'item.

	time_t starting_time
};

// genera un istanza del gioco
struct GameInstance generate_game(struct Room selected, int sd) {
	struct GameInstance instance;
	instance.roomSelected = selected;
	instance.clientSocket = sd;
	instance.currentState = STARTED;
	instance.starting_time = time(NULL);
	//in C99 we can  use time() instead in C89 we should use Clock() which is less accurate

	// -1 = spazio libero, altrimenti prende l'indice dell'item
	// viene fatto perche altrimenti l'array viene inizializzato tutto a 0
	for(int i = 0; i < INV_SIZE; i++) {
		instance.itemIds[i] = -1;
	}
	//TODO - timer.
	return instance;
}

// controlla se l'instanza di gioco è libera
int is_game_instance_avaiable(struct GameInstance instance) {
	return instance.currentState == NOT_IN_USE;
}

int has_item(struct GameInstance instance, int itemId) {
	// controllo sull'id passato, id < 0 non ha senso
	// ritorna -1 se l'id passato è errato
	if (itemId < 0) {
		return -1;
	}

	// ritorna 0 se l'item è presente nell'inventario
	for(int i = 0; i < INV_SIZE; i++) {
		if (instance.itemIds[i] == itemId) {
			return 0;
		}
	}

	// ritorna 1 se l'item non è nell'inventario
	return 1;
}

int add_item(struct GameInstance* instance, int itemId) {
	for(int i = 0; i < INV_SIZE; i++) {
		if (instance->itemIds[i] == -1) {
			instance->itemIds[i] = itemId;
			return 0;
		}
	}
	return -1;
}

int drop_item(struct GameInstance* instance, int itemId) {
	int res = has_item((*instance), itemId);
	// controlla il risultato della has_item che dice se l'item è presente o meno
	if (res != 0) {
		return res;
	}

	// ritorna 0 se il drop è andato a buon fine
	for(int i = 0; i < INV_SIZE; i++) {
		if (instance->itemIds[i] == itemId) {
			instance->itemIds[i] = -1;
			return 0;
		}
	}

	// si arriva qua quando non è possibile droppare l'item perche questo non è presente
	// teoricamente non ci si arriva mai
	return 1;
}


int has_timer_ended(struct GameInstance instance) {
	time_t current_time = time(NULL);
	double timeDiff = difftime(instance.starting_time, current_time);

	return timeDiff >= MAX_TIME;
}


#endif
