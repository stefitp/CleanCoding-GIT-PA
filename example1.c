#include <stdio.h>
#include <stdlib.h>

// structura pentru un nod din lista de adiacenta
typedef struct node {
    int destinatie;
    struct node* next;
} node;

// structura pentru graf
typedef struct graf {
    int nr_noduri;
    int* vizitat;
    node** liste_adiacenta;
} graf;

// structura pentru stiva
typedef struct stiva {
    int varf;
    int capacitate;
    int* elemente;
} stiva;

// creeaza un nod nou pentru lista de adiacenta
node* creeaza_nod(int destinatie) {
    node* nod_nou = (node*)malloc(sizeof(node));
    nod_nou->destinatie = destinatie;
    nod_nou->next = NULL;
    return nod_nou;
}

// creeaza un graf cu nr_noduri noduri
graf* creeaza_graf(int nr_noduri) {
    graf* g = (graf*)malloc(sizeof(graf));
    g->nr_noduri = nr_noduri;
    g->liste_adiacenta = (node**)malloc(nr_noduri * sizeof(node*));
    g->vizitat = (int*)calloc(nr_noduri, sizeof(int));

    // initializeaza listele de adiacenta cu null
    for (int i = 0; i < nr_noduri; i++) {
        g->liste_adiacenta[i] = NULL;
    }
    return g;
}

// adauga o muchie neorientata intre sursa si destinatie
void adauga_muchie(graf* g, int sursa, int destinatie) {
    // adauga destinatie in lista de adiacenta a sursei
    node* nod_nou = creeaza_nod(destinatie);
    nod_nou->next = g->liste_adiacenta[sursa];
    g->liste_adiacenta[sursa] = nod_nou;

    // adauga sursa in lista de adiacenta a destinatiei (graf neorientat)
    nod_nou = creeaza_nod(sursa);
    nod_nou->next = g->liste_adiacenta[destinatie];
    g->liste_adiacenta[destinatie] = nod_nou;
}

// creeaza o stiva cu capacitate data
stiva* creeaza_stiva(int capacitate) {
    stiva* s = (stiva*)malloc(sizeof(stiva));
    s->varf = -1;
    s->capacitate = capacitate;
    s->elemente = (int*)malloc(capacitate * sizeof(int));
    return s;
}

// adauga un element in stiva
void push(stiva* s, int valoare) {
    if (s->varf < s->capacitate - 1) {
        s->elemente[++s->varf] = valoare;
    }
}

// parcurgere dfs pentru a marca nodurile accesibile
void dfs(graf* g, stiva* s, int nod_start) {
    g->vizitat[nod_start] = 1;
    push(s, nod_start);

    // parcurge vecinii nodului curent
    node* aux = g->liste_adiacenta[nod_start];
    while (aux != NULL) {
        int vecin = aux->destinatie;
        if (!g->vizitat[vecin]) {
            dfs(g, s, vecin);
        }
        aux = aux->next;
    }
}

// reseteaza vectorul de noduri vizitate
void reseteaza_vizite(graf* g) {
    for (int i = 0; i < g->nr_noduri; i++) {
        g->vizitat[i] = 0;
    }
}

// citeste si adauga muchii in graf
void citeste_muchii(graf* g, int nr_muchii) {
    printf("introdu %d muchii (sursa destinatie):\n", nr_muchii);
    for (int i = 0; i < nr_muchii; i++) {
        int sursa, destinatie;
        scanf("%d %d", &sursa, &destinatie);
        adauga_muchie(g, sursa, destinatie);
    }
}

// verifica daca exista drum intre nodul start si nodul destinatie
int exista_drum(graf* g, int start, int destinatie) {
    stiva* s = creeaza_stiva(g->nr_noduri);
    reseteaza_vizite(g);
    dfs(g, s, start);

    int rezultat = g->vizitat[destinatie];
    
    // elibereaza memoria stivei
    free(s->elemente);
    free(s);
    return rezultat;
}

// elibereaza memoria ocupata de graf
void elibereaza_graf(graf* g) {
    for (int i = 0; i < g->nr_noduri; i++) {
        node* aux = g->liste_adiacenta[i];
        while (aux != NULL) {
            node* temp = aux;
            aux = aux->next;
            free(temp);
        }
    }
    free(g->liste_adiacenta);
    free(g->vizitat);
    free(g);
}

int main() {
    int nr_noduri, nr_muchii;
    
    // citeste numarul de noduri
    printf("cate noduri are graful? ");
    scanf("%d", &nr_noduri);

    // citeste numarul de muchii
    printf("cate muchii are graful? ");
    scanf("%d", &nr_muchii);

    // creeaza graful
    graf* g = creeaza_graf(nr_noduri);

    // citeste muchiile
    citeste_muchii(g, nr_muchii);

    // verifica drum intre doua noduri
    int nod_start, nod_destinatie;
    printf("introdu nodurile pentru verificare (start destinatie): ");
    scanf("%d %d", &nod_start, &nod_destinatie);

    // afiseaza rezultatul
    if (exista_drum(g, nod_start, nod_destinatie)) {
        printf("exista drum intre %d si %d\n", nod_start, nod_destinatie);
    } else {
        printf("nu exista drum intre %d si %d\n", nod_start, nod_destinatie);
    }

    // elibereaza memoria
    elibereaza_graf(g);
    return 0;
}
