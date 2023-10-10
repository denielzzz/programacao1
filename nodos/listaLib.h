#include <stdio.h>
#include <stdlib.h>
#define TIPO int


struct nodo
{
    TIPO dado;
    struct nodo *proximo;
};

struct cabeca
{
    int tam;
    struct nodo *comeco;
};

struct cabeca *inicializa_cabeca();

struct nodo *le_elemento();

int lista_vazia(struct cabeca *cabeca);

void insere_elemento(struct cabeca *cabeca, struct nodo *x);

int remove_elemento(struct cabeca *cabeca, TIPO x);

void imprime_elementos(struct cabeca *cabeca);