/*BOBES RAZVAN - 315CB*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LGraph.h"
#include <limits.h>

#define IN "tema3.in"

// Rezolvarea cerintei 2 :

// Alocarea unei zone de memorie pentru graph-ul de drummuri 

TGraph *AlocareGraph(int n) {
    TGraph *g = (TGraph *)malloc(sizeof(TGraph));
    if (!g) return NULL;

    g->n = n;
    g->x = (TSinGraph *)malloc(sizeof(TSinGraph) * n);
    if (!g->x) {
        free(g);
        return NULL;
    }
	g->orase = 0;
    return g;
}

// Functie de distrugere a Graph-ului de drumuri

void DistrugeGraph(TGraph **aL) {
    if (!aL) return ;

    int i;
    TSinGraph p, aux;
    for (i = 0; i < (*aL)->n; i++) {
        p = (*aL)->x[i];
        while (p) {
            aux = p;
            p = p->urm;
            free(aux->destinatie);
            free(aux);
        }
    }
    free((*aL)->x);
    free((*aL));
}

// Introducem fiecare drum in lista de adiacenta (Graph-ul de drumuri) + cost drum

TGraph *CitireElementeGraph(char **vizitat, int *nr_drumuri, FILE *input, char **sursa_oras, char **destinatie_oras, int *costuri_drumuri) {
    TGraph *aL = NULL;
    char name_src[101];
    char name_dest[101];
    int drum;
    int cost;
    drum = (*nr_drumuri);

    if (!drum) {
        fclose(input);
        return NULL;
    }

    int i, cont1, cont2;                                             //cont - contor pentru numarul de locatii vizitate
    cont1 = 0;

    int contor_parcurgere;
    contor_parcurgere = 0;

    aL = AlocareGraph(drum);

    TSinGraph *p, aux;
    for (i = 0; i < drum; i++) {
        if (fscanf(input, "%s %s %d", name_src, name_dest, &cost) != 3) {
            fclose(input);
            return NULL;
        }

        int len_dest = strlen(name_dest);
        int len_src = strlen(name_src);

        sursa_oras[contor_parcurgere] = (char *)malloc((len_src + 1) * sizeof(char));
        destinatie_oras[contor_parcurgere] = (char *)malloc((len_dest + 1) * sizeof(char));

        strncpy(sursa_oras[contor_parcurgere], name_src, len_src + 1);
        strncpy(destinatie_oras[contor_parcurgere], name_dest, len_dest + 1);

        sursa_oras[contor_parcurgere][len_src] = '\0';
        destinatie_oras[contor_parcurgere][len_dest] = '\0';

        costuri_drumuri[contor_parcurgere] = cost;

        int idx_src, idx_dest;
        idx_src = -1;
        idx_dest = -1;
        int j;

        // verific daca orasul sursa exista in vectorul de orase vizitate
        // daca nu il gasesc, il voi adauga in lista de orase vizitate

        if (!cont1) {
            vizitat[cont1] = (char *)malloc((strlen(name_src) + 1) * sizeof(char));
            strcpy(vizitat[cont1], name_src);
            idx_src = cont1;
            cont1++;
        } else {
            for (j = 0; j < cont1; j++) {
                if (vizitat[j] && strcmp(vizitat[j], name_src) == 0) {
                    idx_src = j;
                }
            }
            if (idx_src == -1) {
                vizitat[cont1] = (char *)malloc((strlen(name_src) + 1) * sizeof(char));
                strcpy(vizitat[cont1], name_src);
                idx_src = cont1;
                cont1++;
            }
        }

        p = aL->x + idx_src;
        while (*p && strcmp((*p)->destinatie, name_dest) != 0) {
            p = &(*p)->urm;
        }

        aux = (TCelGraph *)calloc(sizeof(TCelGraph),1);
        if (!aux) {
            DistrugeGraph(&aL);
            fclose(input);
            return NULL;
        }
        aux->urm = *p;
        *p = aux;
        aux->destinatie = (char *)malloc((len_dest + 1) * sizeof(char));
        strcpy(aux->destinatie, name_dest);
        aux->cost = cost;

        contor_parcurgere++;
    }

	// Am salvat contorul de drumuri intr-un alt auxiliar, va reprezenta numarul de orase sursa, distincte
	cont2 = cont1;
	if (cont2 < drum * 2) {
		for (i = cont2; i < drum; i++) {
			free(aL->x[i]);
		}
		aL->n = cont2;								// actualizez numarul de drumuri, in cazul in care am alocat mai mult decat aveam nevoie
	}

	for (i = 0; i < cont2; i++) {
		aux = aL->x[i];
		for ( ; aux != NULL; aux = aux->urm) {
			int j;
			for (j = 0; j < cont1; j++) {
				if (strcmp(vizitat[j], aux->destinatie) == 0) {
					break;
				}
			}
			if (j == cont1) {
				int len;
				len = strlen(aux->destinatie);
				vizitat[cont1] = (char *)malloc((len + 1) * sizeof(char));
				strcpy(vizitat[cont1], aux->destinatie);
				cont1++;
			}
		}
	}

	// cont1 va reprezenta numarul de orase vizitate

	if (cont1 < 2 * drum) {
		for (i = cont1; i < 2 * drum; i++) {
			free(vizitat[i]);
		}
		vizitat = (char **)realloc(vizitat, cont1 * sizeof(char *));		// realoc zona de memorie pentru vectorul de orase vizitate
	}

	aL->orase = cont1;
    fclose(input);
    return aL;
}

// Functie auxiliara de afisare a Graph-ului de drumuri; pentru verificare

void AfisareGraph(TGraph *aL, FILE *output, char **vizitat) {
    if (!aL) return;

    int i;
    TSinGraph p;
    for (i = 0; i < aL->n; i++) {
        p = aL->x[i];
        if (!p) {
            fprintf(output, "%d: - \n", i);
        } else {
            fprintf(output, "%s -> ", vizitat[i]);
        }
        for (; p != NULL; p = p->urm) {
            fprintf(output, "%s %d ", p->destinatie, p->cost);
        }
        fprintf(output, "\n");
    }
}

void SelectareDrumMinim(TGraph *aL, FILE *out, char **vizitat, char *start, int nr_drumuri, char **sursa_oras, char **destinatie_oras, int *costuri_drumuri) {
	// char *start - orasul de start -- poate sa fie un oras din lista de destinatii sou un oras din vectorul vizitat
	// pentru fiecare index din vectorul vizitat am coresponzator un oras / mai multe orase aflate la acelasi index din lista de destinatii
	int i;
		// vector de costuri minime
	int d_cost[nr_drumuri];
		// vector de parinti, o sa retina numele fiecarui oras parinte (de unde porneste un drum), va retine maxim nr_drumuri_maxim orase de start
	char **parinte = (char **)malloc(nr_drumuri * sizeof(char *));
		// vector coada, verifca daca am vizitat un oras sau nu
	int Q[nr_drumuri];

    int coada_drumuri_vizitate[nr_drumuri];

	int idx_start = -1;
		// caut indexul orasului de start ; pot sa verific daca orasul de start se afla in prima parte a vectorului de orase (este un oras sursa sau un oras destinatie)
	for (i = 0; i < aL->orase; i++) {
		if (strcmp(vizitat[i], start) == 0) {
			idx_start = i;
			break;
		}
	}

    for (i = 0; i < nr_drumuri; i++) {
        coada_drumuri_vizitate[i] = 0;
    }

    // Am initializat vectorii de costuri si de parinti

    for (i = 0; i < nr_drumuri; i++) {
        d_cost[i] = INT_MAX;
        Q[i] = 0;
        parinte[i] = NULL;
    }

    // Daca indexul sursa este mai mare decat numarul de orase sursa, atunci orasul de start este un oras destinatie
    // Daca este un oras destinatie, atunci trebuie sa-l caut prin aL si sa gasesc valoarea minima prin care putem sa ajungem la el

    int min_start = INT_MAX;

    for (i = 0; i < aL->n; i++) {
        TSinGraph p = aL->x[i];
        for (; p != NULL; p = p->urm) {
            if (strcmp(p->destinatie, start) == 0) {
                if (d_cost[i] < min_start) {
                    min_start = d_cost[i];
                }
            }
        }
    }

    d_cost[idx_start] = 0;

    int j;
    for (j = 0; j < nr_drumuri; j++) {
        int u = -1;
        int min = INT_MAX;
        for (i = 0; i < nr_drumuri; i++) {
            if (Q[i] == 0 && d_cost[i] < min) {
                min = d_cost[i];
                u = i;
            }
        }
        if (u == -1) {
            break;
        }
        // u > aL->n inseamna ca am gasit un oras in lista de destinatii
        // trebuie sa caut un oras sursa care porneste catre orasul gasit, si are o valoare minima a costuluis

        Q[u] = 1;
        TSinGraph p = aL->x[u];

        // Trebuie sa caut orasul sursa care porneste catre orasul gasit, si are o valoare minima a costului

        for ( ; p != NULL; p = p->urm) {
            int v;
            for (v = 0; v < nr_drumuri; v++) {
                if (strcmp(vizitat[v], p->destinatie) == 0) {
                    break;
                }
            }
            if (Q[v] == 0 && d_cost[v] > d_cost[u] + p->cost && d_cost[u] != INT_MAX) {
                d_cost[v] = d_cost[u] + p->cost;
                int len = strlen(vizitat[u]);
                parinte[v] = (char *)malloc((len + 1) * sizeof(char));
                parinte[v] = vizitat[u];
            }
        }
    }

    d_cost[idx_start] = min_start;

    int *vizitat2 = (int *)malloc(aL->orase * sizeof(int));

    for (i = 0; i < aL->orase; i++) {
        vizitat2[i] = 0;
    }

    // Vreau sa sar peste primele 3 elemente care se citesc din fisierul de input

    int drumuri_alese = 0;

    int k;
    k = 0;

    for (k = 0; k < nr_drumuri; k++) {
        for (i = 0; i < nr_drumuri; i++) {
            if (d_cost[i] != INT_MAX && Q[i] == 1 && strcmp(sursa_oras[k], parinte[i]) == 0 && strcmp(vizitat[i], destinatie_oras[k]) == 0 && vizitat2[i] == 0) {
                drumuri_alese++;
            }
        }
    }

    fprintf(out, "%d\n", drumuri_alese);

    for (k = 0; k < nr_drumuri; k++) {
        for (i = 0; i < nr_drumuri; i++) {
            if (d_cost[i] != INT_MAX && Q[i] == 1 && strcmp(sursa_oras[k], parinte[i]) == 0 && strcmp(vizitat[i], destinatie_oras[k]) == 0 && vizitat2[i] == 0) {
                fprintf(out, "%s %s\n", parinte[i], vizitat[i]);
                vizitat2[i] = 1;
            }
        }
    }

    free(parinte);
    free(vizitat2);
}