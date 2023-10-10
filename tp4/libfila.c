#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
fila_t *fila_cria()
{
    fila_t *novaFila;

    if(!(novaFila = malloc(sizeof(fila_t))))
        return NULL;
    
    novaFila->cabeca = NULL;
    novaFila->cauda = NULL;
    novaFila->tamanho = 0;

    return novaFila;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi(fila_t **fila)
{
    nodo_t *aux;
    
    while(fila_tamanho(*fila))
    {    
        aux = (*fila)->cabeca;
        (*fila)->cabeca = (*fila)->cabeca->prox;
        free(aux);
        aux = NULL;
        (*fila)->tamanho--;
    }

    free(*fila);
}

/* 
 * Insere dado no final da fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue(fila_t *fila, int dado)
{
    nodo_t *novoNodo;

    if(!(novoNodo = malloc(sizeof(nodo_t))))
        return 0;

    novoNodo->dado = dado;
    novoNodo->prox = NULL;
    
    if(fila_vazia(fila))
    {
        fila->cabeca = novoNodo;
        fila->cauda = novoNodo;
        fila->tamanho++;
        return 1;
    }

    fila->cauda->prox = novoNodo;
    fila->cauda = novoNodo; 
    fila->tamanho++;

    return 1;
}

/* 
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro dado. Nao confundir com o retorno da funcao.
 * A funcao retorna 1 em caso de sucesso e 0 no caso da fila estar vazia.
*/
int dequeue(fila_t *fila, int *dado)
{
    if(fila_vazia(fila))        
        return 0;

    nodo_t *aux;

    *dado = fila->cabeca->dado;
    aux = fila->cabeca;
    fila->cabeca = fila->cabeca->prox;
    free(aux);
    aux = NULL;
    fila->tamanho--;

    if(fila_vazia(fila))
        fila->cauda = NULL;    

    return 1;
}
 
/* Retorna o numero de elementos da fila, que pode ser 0. */
int fila_tamanho(fila_t *fila)
{
    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia(fila_t *fila)
{
    return !fila->tamanho;
}
