#include <stdio.h>
#include <stdlib.h>
#include "liblista_ordenada.h"

int main(void)
{
    lista_t *l;
    elemento_t *elemento;

    elemento = malloc(sizeof(elemento_t));

    if(!(l = lista_cria()))
    {
        printf("Erro ao criar lista\n");
        return 0;
    }
    printf("Lista criada com sucesso\n");

    elemento->chave = 5;
    if(lista_insere_ordenado(l, elemento))
        printf("%d\n", l->ini->elemento->chave);
    else
        printf("Erro ao inserir elemento\n");

    elemento->chave = 3;
    if(lista_insere_ordenado(l, elemento))
    {
        printf("%d ", l->ini->elemento->chave);
        printf("%d\n", l->ini->prox->elemento->chave);
    }
    else
        printf("Erro ao inserir elemento\n");


    elemento->chave = 2;
    if(lista_insere_ordenado(l, elemento))
    {
        printf("%d ", l->ini->elemento->chave);
        printf("%d ", l->ini->prox->elemento->chave);
        printf("%d\n", l->ini->prox->prox->elemento->chave);
    }
    else
        printf("Erro ao inserir elemento\n");

    elemento->chave = 1;
    if(lista_insere_ordenado(l, elemento))
    {
        printf("%d ", l->ini->elemento->chave);
        printf("%d ", l->ini->prox->elemento->chave);
        printf("%d ", l->ini->prox->prox->elemento->chave);
        printf("%d\n", l->ini->prox->prox->prox->elemento->chave);
    }
    else
        printf("Erro ao inserir elemento\n");

    elemento->chave = 4;
    if(lista_insere_ordenado(l, elemento))
    {
        printf("%d ", l->ini->elemento->chave);
        printf("%d ", l->ini->prox->elemento->chave);
        printf("%d ", l->ini->prox->prox->elemento->chave);
        printf("%d ", l->ini->prox->prox->prox->elemento->chave);
        printf("%d\n", l->ini->prox->prox->prox->prox->elemento->chave);
    }
    else
        printf("Erro ao inserir elemento\n");

    elemento->chave = 5;
    if(lista_remove_ordenado(l, elemento))
    {
    printf("%d ", l->ini->elemento->chave);
    printf("%d ", l->ini->prox->elemento->chave);
    printf("%d ", l->ini->prox->prox->elemento->chave);
    printf("%d\n", l->ini->prox->prox->prox->elemento->chave);
    }
    else
        printf("Elemento nao esta na lista\n");

    elemento->chave = 1;
    if(lista_remove_ordenado(l, elemento))
    {
    printf("%d ", l->ini->elemento->chave);
    printf("%d ", l->ini->prox->elemento->chave);
    printf("%d\n", l->ini->prox->prox->elemento->chave);
    }
    else
        printf("Elemento nao esta na lista\n");

    elemento->chave = 4;
    if(lista_remove_ordenado(l, elemento))
    {
    printf("%d ", l->ini->elemento->chave);
    printf("%d\n", l->ini->prox->elemento->chave);
    }
    else
        printf("Elemento nao esta na lista\n");


    elemento->chave = 2;
    if(lista_remove_ordenado(l, elemento))
        printf("%d\n", l->ini->elemento->chave);
    else
        printf("Elemento nao esta na lista\n");


    elemento->chave = 3;
    if(lista_remove_ordenado(l, elemento))
        printf("Lista vazia\n");
    else
        printf("Elemento nao esta na lista\n");

    lista_destroi(&l);
    free(elemento);

    return 0;
}