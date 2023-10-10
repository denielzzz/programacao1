#include <stdio.h>
#include "libpilha.h"
#define MAX 100

/* insere os pareteses, chaves e colchetes abertos na pilha, 
 * caso encontre um fechamento, verifica se o topo da pilha
 * é o correspondente, caso não seja, retorna 0, caso seja 
 * em todos os casos, retorna 1 */
int verifica_expressao(pilha_t *pilha, char *expressao)
{
    int i = 0;
    int dado;

    while(expressao[i] != '\0')
    {
        switch(expressao[i])
        {
            case '(':
                push(pilha, 1);
                break;
            case '[':
                push(pilha, 2);
                break;
            case '{':
                push(pilha, 3);
                break;
            case ')':
                if(!pop(pilha, &dado) || dado != 1)
                    return 0;
                break;
            case ']':
                if(!pop(pilha, &dado) || dado != 2)
                    return 0;
                break;
            case '}':
                if(!pop(pilha, &dado) || dado != 3)
                    return 0;
                break;
            default:
                break;
        }
        i++;
    }
    if(pilha_vazia(pilha))
        return 1;
    
    return 0;
}

int main()
{
    pilha_t *pilha;
    char expressao[MAX];

    pilha = pilha_cria();
    fgets(expressao, MAX, stdin);
    
    if(verifica_expressao(pilha, expressao))
        printf("CORRETA\n");
    else
        printf("INCORRETA\n");

    pilha_destroi(&pilha);
    
    return 0;
}