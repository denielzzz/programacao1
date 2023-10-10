/* 
 * Programa para testar a libpilha.
 * feito por Marcos Castilho em 05/04/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "libpilha.h"

/* 
 * Cria e retorna uma nova pilha.
 * Retorna NULL em caso de erro de alocação.
*/
pilha_t *pilha_cria ()
{
    pilha_t *pilha;

    if(!(pilha = malloc(sizeof(pilha_t))))
        return NULL;

    pilha->tamanho = 0;
    pilha->topo = NULL;

    return pilha;
}

/* Desaloca toda memoria da pilha e faz pilha receber NULL. */
void pilha_destroi (pilha_t **pilha)
{
    nodo_t *aux;

    while(!pilha_vazia(*pilha))
    {
        aux = (*pilha)->topo;
        (*pilha)->topo = (*pilha)->topo->prox;
        free(aux);
        
        (*pilha)->tamanho--;
    }

    free(*pilha);
    (*pilha) = NULL;
}

/* 
 * Insere dado na pilha (politica LIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int push (pilha_t *pilha, int dado)
{
    nodo_t *aux;

    if(!(aux = malloc(sizeof(nodo_t))))
        return 0;
   
    aux->dado = dado;
    aux->prox = pilha->topo;
    pilha->topo = aux;
   
    pilha->tamanho++;

    return 1;
}
/* 
 * Remove o topo (politica LIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da pilha estar vazia.
*/
int pop (pilha_t *pilha, int *dado)
{
    if(pilha_vazia(pilha))
        return 0;

    nodo_t *aux;

    *dado = pilha->topo->dado;
    aux = pilha->topo;
    pilha->topo = pilha->topo->prox;
    free(aux);
    aux = NULL;
   
    pilha->tamanho--;
    
    return 1;
}
 
/* Similar ao pop, mas retorna o elemento dado sem remove-lo. */
int pilha_topo (pilha_t *pilha, int *dado)
{
    if(pilha_vazia(pilha))
        return 0;

    *dado = pilha->topo->dado;
    
    return 1;
} 

/* Retorna o numero de elementos da pilha, que pode ser 0. */
int pilha_tamanho (pilha_t *pilha)
{
    return pilha->tamanho;
}

/* Retorna 1 se pilha vazia, 0 em caso contrario. */ 
int pilha_vazia (pilha_t *pilha)
{
    return !pilha->tamanho;
}
