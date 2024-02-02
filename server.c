#include <stdio.h>
#include <string.h>

int max_elem = 10;

struct Enigma {
    char* text;
    char* answer;
};

enum Status {
    FREE,           // senza enigma
    BLOCKED,        // bloccato da enigma
    BLOCKED_TOKEN,  // enigma e token
    TOKEN           // solo token
};

struct Object {
    char* name;
    char* desc;
    enum Status state;
    struct Enigma enigma;
};

struct Location {
    char* name;
    char* desc;
    int objects_index[max_elem];
};

struct Room {
    char* name;
    char* desc;
    struct Location locations[max_elem];
    struct Object objects[max_elem];
    char* hint; // suggerimento legato alla room
};

// lista delle rooms da caricare (posso avere al massimo max_elem rooms tra cui scegliere)
struct Room rooms[max_elem];

struct Enigma generate_enigma(string text, string answer) {
    struct Enigma eng;
    eng.text = text;
    eng.answer = answer;
    return eng;
};

struct Object generate_object(string name, string desc, enum Status state, struct Enigma eng) {
    struct Object obj;
    obj.name = name;
    obj.desc = desc;
    obj.state = state;
    obj.enigma = eng;
    return obj;
};

struct Location generate_location(string name, string desc, int objects_index[]) {
    struct Location loc;
    loc.name = name;
    loc.desc = desc;
    loc.objects_index = objects_index;
    return loc;
};

void initialization_rooms() {

    struct Enigma enigma_chiave = generate_enigma("Nella notte oscura e nel giorno chiaro,\n Un uccello nero con voce rara.\n 
                                                  \'Nevermore\', grida con aria mesta,\n Il suo richiamo sfida la nostra testa.\n", 
                                                  "Il Corvo\n"
    );
    struct Enigma enigma_busta = generate_enigma("Nel silenzio della notte, in una stanza segreta,\n Battiti misteriosi, un segreto da svelare.\n
                                                  Sotto il pavimento, il colpevole si confonde,\n ma il suo peccato non può nascondere.\n", 
                                                 "Il Cuore Rivelatore\n"
    );
    struct Enigma enigma_scatola = generate_enigma("Tra i muri di pietra, nell\'oscurità profonda,\n Un uomo impazzito, la sua mente sprofonda\n.
                                                    Ogni battito dell\'orologio, un passo verso il suo destino,\n Nella sua mente tormentata, nessuna via di redenzione.\n", 
                                                   "La Caduta della Casa degli Usher\n"
    );


    struct Object chiave = generate_object("chiave", 
                                            "una piccola chiave arrugginita: forse serviva ad aprire qualcosa di molto piccolo?\n",
                                            BLOCKED_TOKEN, 
                                            enigma_chiave
    );
    struct Object foglio = generate_object("foglio", 
                                            "sembra essere una lettera scritta a mano, in bella grafia, sembrerebbe scritta da una donna.\n",
                                            FREE, 
                                            null
    );
    struct Object busta = generate_object("busta", 
                                           "la ceralacca sembra spaccata, quindi è stata aperta. Al suo interno pare esserci una lettera anche se poco leggibile.\n",
                                           BLOCKED, 
                                           enigma_busta
    );
    struct Object lettere = generate_object("lettere", 
                                             "un pacco di lettere raccolte insieme da un filo di raso rosso scuro con un grande fiocco. Le buste sono molto ingiallite, 
                                             segno del tempo passato e sembrano essere firmate tutte dallo stesso mittente, una donna.\n",
                                             TOKEN, 
                                             null
    );
    struct Object ceralacca = generate_object("ceralacca", 
                                              "sembrerebbe un kit per bloccare le lettere con la ceralacca: ci sono vari stick colorati di cera e diversi sgilli, 
                                              tutti con la stessa lettera ma decorati diversamente.\n",
                                              FREE, 
                                              null
    );
    struct Object scatolina = generate_object("scatolina", 
                                               "una piccola scatolina porta-gioie rivestita di velluto nero. Sul davanti sembra esserci una piccola serratura, forse serve una chiave per aprirla?\n",
                                               BLOCKED_TOKEN_TOKEN, 
                                               enigma_scatola
    );


    struct Location camino = generate_location("camino", 
                                               "un tipico camino in pietra, è molto ampio e sembra non essere usato da molto tempo. Al suo interno c\'e\' tanta cenere e alcuni pezzi di legno non del tutto bruciati.
                                               Tra la cenere possiamo intravedere una **busta** e un **foglio** da lettere parzialmente bruciati e, in un angolo, una piccolissima **chiave**.\n",
                                               {0, 1, 2}
    );
    struct Location scrivania = generate_location("scrivania", 
                                                  "vecchia scrivania in legno massiccio, non ha cassetti e la superficie pare molto rovinata con grandi graffi profondi e macchie di inchiostro nero. Sopra troviamo un ++cofanetto++ 
                                                  in legno e metallo finemente decorato, decisamente un altro stile rispetto al resto della stanza e una raccolta di **lettere** chiuse con un fiocco.\n",
                                                  {3}
    );
    struct Location cofanetto = generate_location("cofanetto", 
                                                  "l\'interno è foderato di un tessuto molto lucido, simile alla seta. E\' diviso di 2 scompartimenti: il primo contiene vari strumenti per la **ceralacca** mentre nel secondo troviamo
                                                  una **scatolina** di vellutto.\n",
                                                  {4, 5}
    );


    struct Room r1;
    r1.name = "Baita di legno";
    r1.desc = "L\'interno della baita e\' caratterizzato da un\'unica stanza. Al centro, un grande ++camino++.
               In un angolo una vecchia ++scrivania++ in legno. La porta d\'uscita si trova dall\'altra parte della stanza ma è bloccata
               e servono 3 token per aprirla\n";
    r1.locations = {camino, scrivania, cofanetto};
    r1.objects = {chiave, foglio, busta, lettere, ceralacca, scatolina};
    r1.hint = "cerca dentro al camino";

    rooms[0] = r1;
}

int main() {
    // inizializzazione strutture
    initialization_rooms();

    // il server si mette in attesa di un client

    // gestione comando + gestione fine partita

    // chiusura corretta 
}