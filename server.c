#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "GameData.h"
#include "GameInstance.h"
#include "UserData.h"


#define DIM_BUFFER 1024
#define DIM_CMD 100
#define DIM_PARAM 100

#define SINGLE_SERVER_TESTING
//#define PRINT_COMMAND_RICEAVED


// lista delle rooms da caricare (posso avere al massimo max_elem rooms tra cui scegliere)
struct Room rooms[MAX_ELEM];
struct GameInstance instances[MAX_ELEM];
struct User users[MAX_ELEM];

struct GameInstance* get_instance_by_socket(int sd) {
    struct GameInstance* instance = NULL;
    for(int i = 0; i < MAX_ELEM; i++) {
        if (instances[i].currentState != NOT_IN_USE && instances[i].clientSocket == sd) {
            instance = &instances[i];
            break;
        }
    }
    return instance;
}

struct User* get_user_by_socket(int sd) {
    struct User* instance = NULL;
    for(int i = 0; i < MAX_ELEM; i++) {
        if (users[i].state != UNREGISTERED && users[i].socketId == sd) {
            instance = &users[i];
            break;
        }
    }
    return instance;
}

struct User* get_user_by_name(char* name) {
    struct User* instance = NULL;
    for(int i = 0; i < MAX_ELEM; i++) {
        if (users[i].state != UNREGISTERED && strcmp(users[i].name, name) == 0) {
            instance = &users[i];
            break;
        }
    }
    return instance;
}


void send_msg(int sd, const char* format, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, format);

    // Formattazione della stringa
    vsnprintf(buffer, sizeof(buffer), format, args);

#ifdef SINGLE_SERVER_TESTING
    fprintf(stderr, "%s", buffer);
    return;
#endif

    // TODO - invia il messaggio al client

    va_end(args);
}

// inizializzazione della struct Room con la creazione dei vari elementi che la compongono
void initialization_rooms() {
    struct Enigma enigma_chiave = generate_enigma("Nella notte oscura e nel giorno chiaro,\n Un uccello nero con voce rara.\n \
                                                   \'Nevermore\', grida con aria mesta,\n Il suo richiamo sfida la nostra testa.\n",
                                                  "Il Corvo\n");


    struct Enigma enigma_busta = generate_enigma("Nel silenzio della notte, in una stanza segreta,\n Battiti misteriosi, un segreto da svelare.\n \
                                                  Sotto il pavimento, il colpevole si confonde,\n ma il suo peccato non può nascondere.\n",
                                                 "Il Cuore Rivelatore\n");



    struct Enigma enigma_scatola = generate_enigma("Tra i muri di pietra, nell\'oscurità profonda,\n Un uomo impazzito, la sua mente sprofonda\n. \
                                                    Ogni battito dell\'orologio, un passo verso il suo destino,\n Nella sua mente tormentata, nessuna via di redenzione.\n\
                                                    A) Il Gatto Nero  B) Il Ritratto Ovale  C)La Caduta della Casa degli Usher\n",
                                                   "C\n");


    struct Object chiave = generate_object("chiave",
                                            "una piccola chiave arrugginita: forse serviva ad aprire qualcosa di molto piccolo?\n",
                                            BLOCKED_TOKEN,
                                            &enigma_chiave);


    struct Object foglio = generate_object("foglio",
                                            "sembra essere una lettera scritta a mano, in bella grafia, sembrerebbe scritta da una donna.\n",
                                            FREE,
                                            ((struct Enigma*) NULL));

    struct Object busta = generate_object("busta",
                                           "la ceralacca sembra spaccata, quindi è stata aperta. Al suo interno pare esserci una lettera anche se poco leggibile.\n",
                                           BLOCKED,
                                           &enigma_busta);

    struct Object lettere = generate_object("lettere",
                                             "un pacco di lettere raccolte insieme da un filo di raso rosso scuro con un grande fiocco. Le buste sono molto ingiallite, \
                                             segno del tempo passato e sembrano essere firmate tutte dallo stesso mittente, una donna.\n",
                                             TOKEN,
                                             ((struct Enigma*) NULL));

    struct Object ceralacca = generate_object("ceralacca",
                                              "sembrerebbe un kit per bloccare le lettere con la ceralacca: ci sono vari stick colorati di cera e diversi sgilli, \
                                              tutti con la stessa lettera ma decorati diversamente.\n",
                                              FREE,
                                              ((struct Enigma*) NULL));
    struct Object scatolina = generate_object("scatolina",
                                               "una piccola scatolina porta-gioie rivestita di velluto nero. Sul davanti sembra esserci una piccola serratura, forse serve una chiave per aprirla?\n",
                                               BLOCKED_TOKEN,
                                               &enigma_scatola);

    int array_camino[] = {0, 1, 2};
    struct Location camino = generate_location("camino",
                                               "un tipico camino in pietra, è molto ampio e sembra non essere usato da molto tempo. Al suo interno c\'e\' tanta cenere e alcuni pezzi di legno non del tutto bruciati.\
                                               Tra la cenere possiamo intravedere una **busta** e un **foglio** da lettere parzialmente bruciati e, in un angolo, una piccolissima **chiave**.\n",
                                               array_camino);
    int array_scrivania[] = {3};
    struct Location scrivania = generate_location("scrivania",
                                                  "vecchia scrivania in legno massiccio, non ha cassetti e la superficie pare molto rovinata con grandi graffi profondi e macchie di inchiostro nero. Sopra troviamo un ++cofanetto++ \
                                                  in legno e metallo finemente decorato, decisamente un altro stile rispetto al resto della stanza e una raccolta di **lettere** chiuse con un fiocco.\n",
                                                  array_scrivania);
    int array_cofanetto[] = {4, 5};
    struct Location cofanetto = generate_location("cofanetto",
                                                  "l\'interno è foderato di un tessuto molto lucido, simile alla seta. E\' diviso di 2 scompartimenti: il primo contiene vari strumenti per la **ceralacca** mentre nel secondo troviamo \
                                                  una **scatolina** di vellutto.\n",
                                                  array_cofanetto);


    struct Room r1;
    r1.name = "Baita";
    r1.desc = "L\'interno della baita e\' caratterizzato da un\'unica stanza. Al centro, un grande ++camino++. \
               In un angolo una vecchia ++scrivania++ in legno. La porta d\'uscita si trova dall\'altra parte della stanza ma è bloccata \
               e servono 3 token per aprirla\n";

// eventualmente da modificare !!
    r1.locations[0] = camino;
    r1.locations[1] = scrivania;
    r1.locations[2] = cofanetto;
    r1.objects[0] = chiave;
    r1.objects[1] = foglio;
    r1.objects[2] = busta;
    r1.objects[3] = lettere;
    r1.objects[4] = ceralacca;
    r1.objects[5] = scatolina;
    r1.hint = "cerca dentro al camino";

    rooms[0] = r1;
    fprintf(stderr, "Init completata! \n");
}

void gestione_login(char* name, char* password, int sd) {
    printf("sono dentro login \n");

    if (name == NULL || password == NULL || strlen(name) == 0 || strlen(password) == 0) {
        send_msg(sd, "Invalid username or password\n");
        return;
    }

    struct User* connectedUser = get_user_by_name(name);
    if (connectedUser == NULL) {
        send_msg(sd, "il nome utente %s non corrisponde a nessun utente| prima registrare il nuovo utente con il comando singup \n", name);
        return;
    }

    int res = login_user(connectedUser, name, password);
    if (res < 0) {
        send_msg(sd,"User %s is not OFFLINE \n", connectedUser->name);
        return;
    } else if (res > 0) {
        send_msg(sd,"Invalid Password per utente %s \n", connectedUser->name);
        return;
    }

    send_msg(sd,"Utente %s Loggato! \n", connectedUser->name);
    connectedUser->socketId = sd;

}

void gestione_signup(char* name, char* password, int sd) {
    printf("sono dentro signup \n");

    if (name == NULL || password == NULL || strlen(name) == 0 || strlen(password) == 0) {
        send_msg(sd, "Invalid username or password\n");
        return;
    }

    struct User* connectedUser = get_user_by_socket(sd);
    if (connectedUser != NULL) {
        send_msg(sd, "User is already connected with user name: %s \n", connectedUser->name);
        return;
    }

    for(int i = 0; i < MAX_ELEM; i++) {
        if (users[i].state == UNREGISTERED) {
            users[i] = register_new_user(name, password);
            //users[i].socketId = sd;
            send_msg(sd,"New user registered with name: %s \n", name);
            return;
        }
    }

    send_msg(sd, "Unable to create a new username! please try later! \n");

}

void gestione_help(int sd) {
    printf("sono dentro help");
}

void gestione_start(char* room, int sd) {
    printf("sono dentro start con room %s \n", room);

    struct Room selectedRoom;
    for(int i = 0; i < MAX_ELEM; i++) {
        if (rooms[i].name == NULL) {
            //siamo arrivati alla fine di quelli inizializzati e non l'abbiamo trovato
            send_msg(sd, "ERRORE room non valida");
            return;
        } else if (strcmp(room, rooms[i].name) == 0) {
            //abbiamo trovato la room selezionata!
            selectedRoom = rooms[i];
            break;
        }
    }

    send_msg(sd, "Room selezionata: %s \n", selectedRoom.name);

    for(int i = 0; i < MAX_ELEM; i++) {
        if (is_game_instance_avaiable(instances[i])) {
            instances[i] = generate_game(selectedRoom, sd);
            send_msg(sd, "Room selezionata: %s ed iniziata! \n", selectedRoom.name);
            return;
        }
    }

    send_msg(sd, "ERRORE non ci sono posizioni disponibili, torna più tardi! \n");

}

void gestione_take(char* objName, int sd) {
    printf("sono dentro take \n");

    struct GameInstance* instance = get_instance_by_socket(sd);
    if (instance->currentState != STARTED) {
        send_msg(sd, "Error! no room selected for this socket %d \n", sd);
        return;
    }

    int itemIndex = get_object_index(instance->roomSelected, objName);
    if (itemIndex < 0) {
        send_msg(sd, "Error! invalid item: %s \n", objName);
        return;
    }

    if (has_item(*instance, itemIndex) == 0) {
        send_msg(sd, "Hai già preso questo item %s \n", objName);
        return;
    }

    if (add_item(instance, itemIndex) != 0) {
        send_msg(sd, "Error! can't add item: %s \n", objName);
        //no space ?
    } else {
        send_msg(sd, "Preso item: %s \n", objName);
    }

}

void gestione_look(char* objNameOrLocation, int sd) {
    fprintf(stderr, "sono dentro look \n");

    struct GameInstance* instance = get_instance_by_socket(sd);
    if (instance->currentState != STARTED) {
        send_msg(sd, "Error! no room selected for this socket %d \n", sd);
        return;
    }

    int index = get_object_index(instance->roomSelected, objNameOrLocation);
    if (index >= 0) {
        //look a object
        send_msg(sd, "Object %s: \n%s \n", objNameOrLocation, instance->roomSelected.objects[index].desc);
        return;
    }

    index = get_location_index(instance->roomSelected, objNameOrLocation);
    if (index >= 0) {
        //look a object
        send_msg(sd, "Location %s: \n%s \n", objNameOrLocation, instance->roomSelected.locations[index].desc);
        return;
    }

    send_msg(sd, "Invalid Object or Location selected! \n");


}

void gestione_drop(char* objName, int sd) {
    printf("sono dentro drop \n");

    struct GameInstance* instance = get_instance_by_socket(sd);
    if (instance->currentState != STARTED) {
        send_msg(sd, "Error! no room selected for this socket %d \n", sd);
        return;
    }

    int itemIndex = get_object_index(instance->roomSelected, objName);
    if (itemIndex < 0) {
        send_msg(sd, "Error! invalid item: %s \n", objName);
        return;
    }

    if (drop_item(instance, itemIndex) != 0) {
        send_msg(sd, "Error! non puoi lasciare questo item: %s \n", objName);
        //no space ?
    } else {
        send_msg(sd, "Item lasciato: %s \n", objName);
    }
}

void gestione_use(char* obj1, char* obj2, int sd) {
    printf("sono dentro use");
}

void gestione_objs(int sd) {
    fprintf(stderr, "sono dentro objs \n");

    struct GameInstance* instance = get_instance_by_socket(sd);
    if (instance->currentState != STARTED) {
        send_msg(sd, "Error! no room selected for this socket %d \n", sd);
        return;
    }

    char listOfObjs[1024] = "\0";

    int hasitems = -1;
    for (int i = 0; i < INV_SIZE; i++) {
        if (instance->itemIds[i] != -1) {
            hasitems = 0;
            strcat(listOfObjs, instance->roomSelected.objects[instance->itemIds[i]].name);
            strcat(listOfObjs, "\n");
        }
    }

    if (hasitems == 0) {
        strcat(listOfObjs, "\0");
        send_msg(sd, "List of Items: \n%s \n", listOfObjs);
    } else {
        send_msg(sd, "you have no items \n");
    }


}

void gestione_hint(int sd) {
    printf("sono dentro hint \n");

    struct GameInstance* instance = get_instance_by_socket(sd);
    if (instance->currentState != STARTED) {
        send_msg(sd, "Error! no room selected for this socket %d \n", sd);
        return;
    }

    send_msg(sd, "Hint: %s \n", instance->roomSelected.hint);
}

void gestione_end(int sd) {
    printf("sono dentro end");
}

int gestione_utente(int sd) {
    struct User* connectedUser = get_user_by_socket(sd);
    if (connectedUser != NULL) {
        return 0;
    }
    return -1;
}

// in base al comando viene chiamata la funzione corrispondente per la gestione
void gestione_comandi(char* msg, int sd){
    char cmd[DIM_CMD] = "\0";
    char param1[DIM_PARAM] = "\0";
    char param2[DIM_PARAM] = "\0";
    sscanf(msg, "%s %s %s", cmd, param1, param2);

#ifdef PRINT_COMMAND_RICEAVED

    printf("comando ricevuto: %s \n", cmd);
    printf("param1: %s \n", param1);
    printf("param2: %s \n", param2);

#endif

    if(strcmp(cmd, "login")==0)
        gestione_login(param1, param2, sd);
    else if(strcmp(cmd, "signup")==0)
        gestione_signup(param1, param2, sd);
    else if(strcmp(cmd, "help")==0)
        gestione_help(sd);
    else {
        if (gestione_utente(sd) != 0) {
            send_msg(sd, "L'utente deve essere loggato per effettuare altri comandi! \n");
            return;
        }

        if(strcmp(cmd, "start")==0)
            gestione_start(param1, sd);
        else if(strcmp(cmd, "take")==0)
            gestione_take(param1, sd); //TODO - gestire Enigma/UseList e condizioni di vittoria della partita
        else if(strcmp(cmd, "look")==0)
            gestione_look(param1, sd);
        else if(strcmp(cmd, "drop")==0)
            gestione_drop(param1, sd);
        else if(strcmp(cmd, "use")==0) //TODO - gestire UseList
            gestione_use(param1, param2, sd);
        else if(strcmp(cmd, "objs")==0)
            gestione_objs(sd);
        else if(strcmp(cmd, "hint")==0)
            gestione_hint(sd);
        else if(strcmp(cmd, "end")==0) //TODO - gestire chiusura forzata della partita da parte del client
            gestione_end(sd);
        else
            gestione_help(sd);
    }

    // se il comando inviato non è nessuno dei precedenti vuol dire che è stato scritto male e quindi invio help
}





int main() {
    // inizializzazione strutture
    fprintf(stderr, "start \n");
    initialization_rooms();

    fprintf(stderr, "gestione comandi \n");
    // il server si mette in attesa di un client
    gestione_comandi("login fabio 1234", 12);
    gestione_comandi("signup fabio 1234", 12);
    gestione_comandi("login fabio 1234", 12);

    gestione_comandi("start Baita", 12);
    gestione_comandi("take foglio", 12);
    gestione_comandi("look camino", 12);
    gestione_comandi("look chiave", 12);
    gestione_comandi("take chiave", 12);
    gestione_comandi("objs", 12);
    gestione_comandi("hint", 12);

    // TODO - this should be done when receaving each command by each client!
    /*struct GameInstance*  instance = get_instance_by_socket(12);
    if (has_timer_ended(*instance)) {
        printf("Time is ended! \n");
    } else {
        printf("Time is NOT ended! \n");
    }*/

    // gestione comando + gestione fine partita
    printf("STOP! \n");
    // chiusura corretta

    // per controllare la memoria.
    //valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./server
}
