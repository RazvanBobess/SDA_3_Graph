/*BOBES RAZVAN - 315CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LGraph.h"

#define IN "tema3.in"
#define OUT "tema3.out"

int main() {

    FILE *out;
    FILE *in;

    in = fopen(IN, "r");
    out = fopen(OUT, "w");

    if (!out) {
        printf("Eroare la deschiderea fisierului de iesire!\n");
        return 1;
    }

    if (!in) {
        printf("Eroare la deschiderea fisierului de intrare!\n");
        fclose(out);
        return 1;
    }

    TGraph *drumuri = NULL;          //graful de drumuri
    char **vizitat = NULL;           // vector care reprezinta o matrice de locatii vizitates
    char oras_start[21];            // orasul de start
    int nr_drumuri_maxim;
    int nr_drumuri;

    fscanf(in, "%s %d %d", oras_start, &nr_drumuri_maxim, &nr_drumuri);

    vizitat = (char **)malloc(nr_drumuri * 2 * sizeof(char *));    // alocare memorie pentru vectorul de orase (din care porneste fiecare drum)

    // voi aloca memorie pentru fiecare drum in parte

    char **sursa_oras = (char **)malloc(nr_drumuri * sizeof(char *));
    char **destinatie_oras = (char **)malloc(nr_drumuri * sizeof(char *));
    int *costuri = (int *)malloc(nr_drumuri * sizeof(int));

    if (!vizitat) {
        fclose(in);
        fclose(out);
        return 1;
    }

    drumuri = CitireElementeGraph(vizitat, &nr_drumuri, in, sursa_oras, destinatie_oras, costuri);

    SelectareDrumMinim(drumuri, out, vizitat, oras_start, nr_drumuri, sursa_oras, destinatie_oras, costuri);

    int i;

    for (i = 0; i < nr_drumuri; i++) {
        if (sursa_oras[i]) {
            free(sursa_oras[i]);
        }
        if (destinatie_oras[i]) {
            free(destinatie_oras[i]);
        }
    }

    free(sursa_oras);
    free(destinatie_oras);
    free(costuri);

    for (i = 0; i < drumuri->orase; i++) {
        if (vizitat[i]) {
            free(vizitat[i]);
        }
    }
    free(vizitat);

    DistrugeGraph(&drumuri);

    fclose(out);
    return 0;
}