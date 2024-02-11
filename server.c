#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "GameData.h"
#include "GameInstance.h"


#define DIM_BUFFER 1024
#define DIM_CMD 5 // i comandi sono al massino di 5 lettere
#define DIM_PARAM 100

#define SINGLE_SERVER_TESTING


// lista delle rooms da caricare (posso avere al massimo max_elem rooms tra cui scegliere)
struct Room rooms[MAX_ELEM];
struct GameInstance instances[MAX_ELEM];



void send_msg(int sd, const char* format, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, format);

    // Formattazione della stringa
    vsnprintf(buffer, sizeof(buffer), format, args);

#ifdef SINGLE_SERVER_TESTING
    fprintf(stdout, "%s", buffer);
    return;
#endif

    // TODO - invia il messaggio al client

    va_end(args);
}

// inizializzazione della struct Room con la creazione dei vari elementi che la compongono
void initialization_rooms() {
     fprintf(stderr, "inizio init\n");
    struct Enigma enigma_chiave = generate_enigma("Nella notte oscura e nel giorno chiaro,\n Un uccello nero con voce rara.\n \
                                                   \'Nevermore\', grida con aria mesta,\n Il suo richiamo sfida la nostra testa.\n",
                                                  "Il Corvo\n");
     fprintf(stderr, "primo enigma\n");

    struct Enigma enigma_busta = generate_enigma("Nel silenzio della notte, in una stanza segreta,\n Battiti misteriosi, un segreto da svelare.\n \
                                                  Sotto il pavimento, il colpevole si confonde,\n ma il suo peccato non può nascondere.\n",
                                                 "Il Cuore Rivelatore\n");

     fprintf(stderr, "secondo enigma\n");

    struct Enigma enigma_scatola = generate_enigma("Tra i muri di pietra, nell\'oscurità profonda,\n Un uomo impazzito, la sua mente sprofonda\n. \
                                                    Ogni battito dell\'orologio, un passo verso il suo destino,\n Nella sua mente tormentata, nessuna via di redenzione.\n\
                                                    A) Il Gatto Nero  B) Il Ritratto Ovale  C)La Caduta della Casa degli Usher\n",
                                                   "C\n");
     fprintf(stderr, "terzo enigma\n");

    struct Object chiave = generate_object("chiave",
                                            "una piccola chiave arrugginita: forse serviva ad aprire qualcosa di molto piccolo?\n",
                                            BLOCKED_TOKEN,
                                            &enigma_chiave);
    fprintf(stderr, "primo obj\n");

    struct Object foglio = generate_object("foglio",
                                            "sembra essere una lettera scritta a mano, in bella grafia, sembrerebbe scritta da una donna.\n",
                                            FREE,
                                            ((struct Enigma*) NULL));
    fprintf(stderr, "secondo obj\n");
    struct Object busta = generate_object("busta",
                                           "la ceralacca sembra spaccata, quindi è stata aperta. Al suo interno pare esserci una lettera anche se poco leggibile.\n",
                                           BLOCKED,
                                           &enigma_busta);
    fprintf(stderr, "terzo obj\n");
    struct Object lettere = generate_object("lettere",
                                             "un pacco di lettere raccolte insieme da un filo di raso rosso scuro con un grande fiocco. Le buste sono molto ingiallite, \
                                             segno del tempo passato e sembrano essere firmate tutte dallo stesso mittente, una donna.\n",
                                             TOKEN,
                                             ((struct Enigma*) NULL));
    fprintf(stderr, "quarto obj\n");
    struct Object ceralacca = generate_object("ceralacca",
                                              "sembrerebbe un kit per bloccare le lettere con la ceralacca: ci sono vari stick colorati di cera e diversi sgilli, \
                                              tutti con la stessa lettera ma decorati diversamente.\n",
                                              FREE,
                                              ((struct Enigma*) NULL));      fprintf(stderr, "quinto obj\n");
    struct Object scatolina = generate_object("scatolina",
                                               "una piccola scatolina porta-gioie rivestita di velluto nero. Sul davanti sembra esserci una piccola serratura, forse serve una chiave per aprirla?\n",
                                               BLOCKED_TOKEN,
                                               &enigma_scatola);         fprintf(stderr, "sesto obj\n");

    int array_camino[] = {0, 1, 2};
    struct Location camino = generate_location("camino",
                                               "un tipico camino in pietra, è molto ampio e sembra non essere usato da molto tempo. Al suo interno c\'e\' tanta cenere e alcuni pezzi di legno non del tutto bruciati.\
                                               Tra la cenere possiamo intravedere una **busta** e un **foglio** da lettere parzialmente bruciati e, in un angolo, una piccolissima **chiave**.\n",
                                               array_camino);        fprintf(stderr, "primo loc\n");
    int array_scrivania[] = {3};
    struct Location scrivania = generate_location("scrivania",
                                                  "vecchia scrivania in legno massiccio, non ha cassetti e la superficie pare molto rovinata con grandi graffi profondi e macchie di inchiostro nero. Sopra troviamo un ++cofanetto++ \
                                                  in legno e metallo finemente decorato, decisamente un altro stile rispetto al resto della stanza e una raccolta di **lettere** chiuse con un fiocco.\n",
                                                  array_scrivania);      fprintf(stderr, "seconod loc\n");
    int array_cofanetto[] = {4, 5};
    struct Location cofanetto = generate_location("cofanetto",
                                                  "l\'interno è foderato di un tessuto molto lucido, simile alla seta. E\' diviso di 2 scompartimenti: il primo contiene vari strumenti per la **ceralacca** mentre nel secondo troviamo \
                                                  una **scatolina** di vellutto.\n",
                                                  array_cofanetto);          fprintf(stderr, "terzo loc\n");


    struct Room r1;
    r1.name = "Baita";
    r1.desc = "L\'interno della baita e\' caratterizzato da un\'unica stanza. Al centro, un grande ++camino++. \
               In un angolo una vecchia ++scrivania++ in legno. La porta d\'uscita si trova dall\'altra parte della stanza ma è bloccata \
               e servono 3 token per aprirla\n";
    fprintf(stderr, "creazione room \n");

// eventualmente da modificare !!
    r1.locations = malloc(sizeof(struct Location) * 3);
    r1.locations[0] = camino;
    r1.locations[1] = scrivania;
    r1.locations[2] = cofanetto;
    r1.objects = malloc(sizeof(struct Object) * 6);
    r1.objects[0] = chiave;
    r1.objects[1] = foglio;
    r1.objects[2] = busta;
    r1.objects[3] = lettere;
    r1.objects[4] = ceralacca;
    r1.objects[5] = scatolina;
    r1.hint = "cerca dentro al camino";

    rooms[0] = r1;
}

void gestione_login(char* name, char* password, int sd) {
    printf("sono dentro login");
}

void gestione_signup(char* name, char* password, int sd) {
    printf("sono dentro signup");
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
        if (instances[i].currentState == NOT_IN_USE) {
            instances[i] = generate_game(selectedRoom, sd);
            send_msg(sd, "Room selezionata: %s ed iniziata! \n", selectedRoom.name);
            return;
        }
    }

    send_msg(sd, "ERRORE non ci sono posizioni disponibili, torna più tardi! \n");

}

void gestione_take(char* obj, int sd) {
    printf("sono dentro take");
}

void gestione_look(char* obj, int sd) {
    printf("sono dentro look");
}

void gestione_drop(char* obj, int sd) {
    printf("sono dentro drop");
}

void gestione_use(char* obj1, char* obj2, int sd) {
    printf("sono dentro use");
}

void gestione_objs(int sd) {
    printf("sono dentro objs");
}

void gestione_hint(int sd) {
    printf("sono dentro hint");
}

void gestione_end(int sd) {
    printf("sono dentro end");
}

// in base al comando viene chiamata la funzione corrispondente per la gestione
void gestione_comandi(char* msg, int sd){
    char cmd[100];
    char param1[100];
    char param2[100];
    sscanf(msg, "%s %s %s", cmd, param1, param2);

    printf("comando ricevuto: %s \n", cmd);
    printf("param1: %s \n", param1);
    printf("param2: %s \n", param2);

    if(strcmp(cmd, "login")==0)
        gestione_login(param1, param2, sd);
    else if(strcmp(cmd, "signup")==0)
        gestione_signup(param1, param2, sd);

    else if(strcmp(cmd, "help")==0)
        gestione_help(sd);
    else if(strcmp(cmd, "start")==0)
        gestione_start(param1, sd);
    else if(strcmp(cmd, "take")==0)
        gestione_take(param1, sd);
    else if(strcmp(cmd, "look")==0)
        gestione_look(param1, sd);
    else if(strcmp(cmd, "drop")==0)
        gestione_drop(param1, sd);
    else if(strcmp(cmd, "use")==0)
        gestione_use(param1, param2, sd);
    else if(strcmp(cmd, "objs")==0)
        gestione_objs(sd);
    else if(strcmp(cmd, "hint")==0)
        gestione_hint(sd);
    else if(strcmp(cmd, "end")==0)
        gestione_end(sd);
    else
        gestione_help(sd);
    // se il comando inviato non è nessuno dei precedenti vuol dire che è stato scritto male e quindi invio help
}





int main() {
    // inizializzazione strutture
    fprintf(stderr, "start \n");
    initialization_rooms();

    fprintf(stderr, "gestione comandi \n");
    // il server si mette in attesa di un client
    gestione_comandi("start Baita", 12);
    // gestione comando + gestione fine partita

    // chiusura corretta
}
