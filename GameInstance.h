#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "GameData.h"

#define INV_SIZE 4

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

	int itemIds[INV_SIZE]
	// -1 se non ha item, altrimenti indice dell'item.

	//TODO - timer.
};

struct GameInstance generate_game(struct Room selected, int sd) {
	struct GameInstance instance;
	instance.roomSelected = selected;
	instance.clientSocket = sd;
	instance.currentState = STARTED;
	for(int i = 0; i < INV_SIZE; i++) {
		instance.itemIds[i] = -1;
	}
	//TODO - timer.
	return instance;
}


int is_game_instance_aviable(struct GameInstance instance) {
	return instance.currentState == NOT_IN_USE;
}

int has_item(struct GameInstance instance, int itemId) {
	if (itemId < 0) {
		return -1;
	}

	for(int i = 0; i < INV_SIZE; i++) {
		if (instance.itemIds[i] == itemId) {
			return 0;
		}
	}

	return itemId;
}

int add_item(struct GameInstance instance, int itemId) {
	for(int i = 0; i < INV_SIZE; i++) {
		if (instance.itemIds[i] == -1) {
			instance.itemIds[i] = itemId;
			return 0;
		}
	}
	return -1;
}

int drop_item(struct GameInstance instance, int itemId) {
	int res = has_item(instance, itemId);
	if (res != 0) {
		return res;
	}

	for(int i = 0; i < INV_SIZE; i++) {
		if (instance.itemIds[i] == itemId) {
			instance.itemIds[i] = -1;
			return 0;
		}
	}
}



#endif
