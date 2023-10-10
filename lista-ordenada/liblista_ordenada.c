#include <stdio.h>
#include <stdlib.h>
#include "liblista_ordenada.h"

/*
 * Cria uma Lista vazia, à retorna em caso de sucesso,
 * retorna NULL em caso de falha.
 */
lista_t *lista_cria()
{
    lista_t *novaLista;


    if(!(novaLista = malloc(sizeof(lista_t))))
        return NULL;

    novaLista->ini = NULL;

    return novaLista;
}

/*
 * Destroi a Lista e a aponta para NULL
 */
void lista_destroi(lista_t **l)
{   
   nodo_t *aux;

    while((*l)->ini != NULL)
    {
        aux = (*l)->ini;
        (*l)->ini = (*l)->ini->prox;
        free(aux->elemento);
        aux->elemento = NULL;
        free(aux);
        aux = NULL;
    }

    free(*l);
    *l = NULL;
}

/*
 * Adiciona um elemento em ordem de acordo com o valor elemento->chave na Lista.
 * Retorna 1 em caso de sucesso e 0 caso contrario.
 */
int lista_insere_ordenado(lista_t *l, elemento_t *elemento)
{
    nodo_t *novoElemento;

    /* inicializam as variáveis e retornam 0 em caso de erro de alocação*/    
    if(!(novoElemento = malloc(sizeof(nodo_t))))
        return 0;
    if(!(novoElemento->elemento = malloc(sizeof(elemento_t))))
        return 0;

    novoElemento->elemento->chave = elemento->chave;
    
    /* insere na primeira posição da lista*/
    if(l->ini == NULL || (elemento->chave < l->ini->elemento->chave)) 
    {
        novoElemento->prox = l->ini;
        l->ini = novoElemento;

        return 1;
    }

    nodo_t *aux;

    aux = l->ini;
    while((aux->prox != NULL) && (aux->prox->elemento->chave < elemento->chave))
        aux = aux->prox;

    novoElemento->prox = aux->prox;
    aux->prox = novoElemento;

    return 1;
}

/*
 * Retira o elemento da Lista e a mantem em ordem.
 * Retorna 1 em caso de sucesso e 0 caso elemento nao esteja na Lista.
 */
int lista_remove_ordenado(lista_t *l, elemento_t *elemento)
{
    if(l->ini == NULL)
        return 0;

    nodo_t *aux;
    aux = l->ini;

    if (l->ini->elemento->chave == elemento->chave)
    {
        l->ini = l->ini->prox;
        free(aux->elemento);
        free(aux);
        aux = NULL;

        return 1;
    }
    
    while(aux->prox->elemento->chave != elemento->chave && aux->prox->prox != NULL)
        aux = aux->prox;    
    
    if(aux->prox->elemento->chave != elemento->chave)
        return 0;

    nodo_t *aux2;

    aux2 = aux->prox;
    aux->prox = aux->prox->prox;
    
    free(aux2->elemento);
    aux2->elemento = NULL;
    free(aux2);
    aux2 = NULL;

    return 1;
}