#ifndef GAME_DATA_H
#define GAME_DATA_H

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
    struct Location* locations;
    struct Object* objects;
};


// genera una struct di tipo Enigma passandogli i vari campi
struct Enigma generate_enigma(char* text, char* answer) {
    struct Enigma eng;
    eng.text = text;
    eng.answer = answer;
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

#endif