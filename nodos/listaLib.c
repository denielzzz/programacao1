#include <stdio.h>
#include <stdlib.h>
#include "listaLib.h"

struct cabeca *inicializa_cabeca()
{
    struct cabeca *cabeca;

    cabeca = malloc(sizeof(struct cabeca));
    if(!cabeca)
        return NULL;

    cabeca->comeco = NULL;
    cabeca->tam = 0;
    
    return cabeca;
}

struct nodo *le_elemento()
{
    struct nodo *x;
    
    x = malloc(sizeof(struct nodo));
    if(!x)
        return NULL;

    scanf(" %d", &x->dado);
    x->proximo = NULL;

    return x;
}


int lista_vazia(struct cabeca *cabeca)
{
    if(cabeca != NULL)
        return !cabeca->tam;
    
    return 0;
}

void insere_elemento(struct cabeca *cabeca, struct nodo *x)
{
    if(!lista_vazia(cabeca))
        x->proximo = cabeca->comeco;    
    
    cabeca->comeco = x;
    cabeca->tam++;
}
            
int remove_elemento(struct cabeca *cabeca, TIPO x)
{
   if(lista_vazia(cabeca))
       return 0;
   
   struct nodo *aux;

   if(cabeca->comeco->dado == x)
   {
       aux = cabeca->comeco;
       cabeca->comeco = cabeca->comeco->proximo;
       cabeca->tam--;
       free(aux);        
       return 1;
   }
   
   aux = cabeca->comeco;

   while(aux->proximo != NULL && aux->proximo->dado != x)
       aux = aux->proximo;

   if(aux->proximo == NULL)
       return 0;

   struct nodo *aux2 = aux->proximo;        
   aux->proximo = aux->proximo->proximo;
   free(aux2);

   cabeca->tam--;

   return 1; 
}

void imprime_elementos(struct cabeca *cabeca)
{
    if(lista_vazia(cabeca))
        return;
    
    struct nodo *aux = cabeca->comeco; 
    for(int i = 0; i < cabeca->tam; i++)
    {
        printf("%d ", aux->dado);
        aux = aux->proximo;
    }
}
