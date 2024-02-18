#ifndef USER_DATA_H
#define USER_DATA_H

#include <string.h>

enum UserState {
	UNREGISTERED,
	OFFLINE,
	ONLINE,
};


struct User
{
	char* name;
	char* password;
	UserState state;

	//socket if they are connected
	int socketId;
};


User register_new_user(char* name, char* password) {
	struct User user;
	user.name = name;
	user.password = password;
	user.socketId = -1;
	user.state = OFFLINE;
	return user;
}

//restituisce -1 se utente non è offline, 1 se nome o password sono sbagliate, 0 altrimenti
int login_user(struct User* user, char* name, char* password, int socket) {
	if (user->state != OFFLINE) {
		return -1;
	}

	if (strcmp(user->name, name) == 0 && strcmp(user->password, password) == 0) {
		user->socketId = socket;
		user->state = ONLINE;
		return 0;
	}

	return 1;
}

//restituisce -1 se utente non è ONLINE, 1 se nome è sbagliato, 0 altrimenti
int quit_user(struct User* user, char* name) {
	if (user->state != ONLINE) {
		return -1;
	}

	if (strcmp(user->name, name) == 0) {
		user->socketId = -1;
		user->state = OFFLINE;
		return 0;
	}

	return 1;
}


#endif
