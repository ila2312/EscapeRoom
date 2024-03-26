#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <string.h>
#include <stdlib.h>

#define MAX_ELEM 10

struct Enigma {
    char* text;
    char* answer;
};

enum Status {
    FREE,           // senza enigma
    BLOCKED,        // bloccato da enigma -> la risoluzione permette di guadagnare tempo extra
    BLOCKED_TOKEN,  // enigma e token -> la ricoluzione permette di raccogliere un token
    TOKEN           // solo token
};

struct Object {
    char* name;
    char* desc;
    enum Status state;
    struct Enigma* enigma;
};

struct Location {
    char* name;
    char* desc;
    int objects_index[MAX_ELEM];
};

struct Room {
    char* name;
    char* desc;
    char* hint; // suggerimento legato alla room
    struct Location locations[MAX_ELEM];
    struct Object objects[MAX_ELEM];
    int locationSize;
    int objectsSize;
};


// genera una struct di tipo Enigma passandogli i vari campi
struct Enigma* generate_enigma(char* text, char* answer) {
    struct Enigma* eng = (struct Enigma*) malloc(sizeof(struct Enigma));
    eng->text = text;
    eng->answer = answer;
    return eng;
};

// genera una struct di tipo Object passandogli i vari campi
struct Object generate_object(char* name, char* desc, enum Status state, struct Enigma* eng) {
    struct Object obj;
    obj.name = name;
    obj.desc = desc;
    obj.state = state;
    obj.enigma = eng;
    return obj;
};

// genera una struct di tipo Location passandogli i vari campi
struct Location generate_location(char* name, char* desc, int objects_index[]) {
    struct Location loc;
    loc.name = name;
    loc.desc = desc;
    for (int i=0; i<MAX_ELEM; i++) {
        loc.objects_index[i] = objects_index[i];
    }
    return loc;
};

int min(int valueA, int valueB) {
    if (valueA > valueB) {
        return valueB;
    }
    return valueA;
}

int get_object_index(struct Room room, char* objName) {
    for(int i = 0; i < min(MAX_ELEM, room.objectsSize); i++) {
        if (room.objects[i].name == NULL) {
            break;
        } else if (strcmp(room.objects[i].name, objName) == 0) {
            return i;
        }
    }
    return -1;
}

int get_location_index(struct Room room, char* locName) {
    for(int i = 0; i < min(MAX_ELEM, room.locationSize); i++) {
        if (room.locations[i].name == NULL) {
            break;
        } else if (strcmp(room.locations[i].name, locName) == 0) {
            return i;
        }
    }
    return -1;
}

void clean_up_room(struct Room room) {
    for(int i = 0; i < min(MAX_ELEM, room.objectsSize); i++) {
        if (room.objects[i].enigma != NULL) {
            free(room.objects[i].enigma);
        }
    }
}

#endif