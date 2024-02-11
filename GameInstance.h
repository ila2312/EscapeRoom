#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "GameData.h"

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

	//TODO - timer.
};

struct GameInstance generate_game(struct Room selected, int sd) {
	struct GameInstance instance;
	instance.roomSelected = selected;
	instance.clientSocket = sd;
	instance.currentState = STARTED;
	//TODO - timer.
	return instance;
}



#endif
