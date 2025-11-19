/*BOBES RAZVAN - 315CB*/

// Cerinta 1

typedef struct SinaDest {  
    int tronsoane;                  // number of tronsoane
    char *name;                     // numele destinatiei
    long int *uzura;                // grad de uzura
    struct SinaDest *urm;           // urmatorul element din lista de adiacenta
} TCelDest, *TSinDest;

typedef struct {
    int n_routes;                   // numarul de rute
    TSinDest *x;
} TGL;

// Cerinta 2

typedef struct GraphDrum {
    char *destinatie;               // numele destinatiei
    int cost;                       // costul drumului
    struct GraphDrum *urm;          // urmatorul element din lista de adiacenta
} TCelGraph, *TSinGraph;

typedef struct {
    int orase;                      // numarul de orase
    int n;                          // numarul de orase sursa distincte
    TSinGraph *x;                   // vector de pointeri la liste de adiacenta   
} TGraph;


TGL *AlocGraphRute(int n);
void DistrgTGL(TGL **ag);
int HashChar(char *s);
TGL *CitGrafRute(char **vizitat, int *years, long int *uzura_minima, int *route, FILE *input);
void AfisareTGL(TGL *ag, FILE *output);

// Cerinta 2 :
TGraph *AlocareGraph(int n);
void DistrugeGraph(TGraph **aL);
TGraph *CitireElementeGraph(char **vizitat, int *nr_drumuri, FILE *input, char **sursa_oras, char **destinatie_oras, int *costuri_drumuri);
void AfisareGraph(TGraph *aL, FILE *output, char **vizitat);
void SelectareDrumMinim(TGraph *aL, FILE *output, char **vizitat, char *start, int nr_drumuri, char **sursa_oras, char **destinatie_oras, int *costuri_drumuri);