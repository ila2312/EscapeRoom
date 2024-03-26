#ifndef USER_DATA_H
#define USER_DATA_H

#include <string.h>

enum UserState {
	UNREGISTERED,	//user not in use, when a new user is allocated in memory will start with UNREGISTERED
	OFFLINE,
	ONLINE,
};


struct User
{
	char* name;
	char* password;
	enum UserState state;

	//socket if they are connected
	int socketId;
};


struct User register_new_user(char* name, char* password) {
	struct User* user = malloc(sizeof(struct User));
	user->name = malloc(strlen(name) + 1);
	user->password = malloc(strlen(password) + 1);
	strcpy(user->name, name);
	strcpy(user->password, password);
	user->socketId = -1;
	user->state = OFFLINE;
	return *user;
}

void clean_up_user(struct User* user) {
	free(user->name);
	free(user->password);
	free(user);
}

//restituisce -1 se utente non è offline, 1 se nome o password sono sbagliate, 0 altrimenti
int login_user(struct User* user, char* name, char* password) {
	if (user->state != OFFLINE) {
		return -1;
	}

	if (strcmp(user->name, name) == 0 && strcmp(user->password, password) == 0) {
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
		user->state = OFFLINE;
		return 0;
	}

	return 1;
}

int is_user_online(struct User* user) {
	return user != NULL && user->state == ONLINE;
}


#endif
