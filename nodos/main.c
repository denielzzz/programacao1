#include <stdio.h>
#include "listaLib.h"

int main()
{
    struct cabeca *cabeca;
    struct nodo *x;
    int saida;
    
    cabeca = inicializa_cabeca();
    
    if(!cabeca)
    {
        printf("erro de alocacao\n");
        return 1;
    }

    do
    {
        printf("digite um numero\n");
        x = le_elemento();
        if(!x)
        {
            printf("erro de alocacao\n");
            return 1;
        }

        printf("digite 1 para inserir e 2 para tirar\n");
        scanf("%d", &saida);
        if(saida == 1)
            insere_elemento(cabeca, x);
        else
            if(saida != 2 || !remove_elemento(cabeca, x->dado))
            {
                printf("comando inválido\n");
                saida = 0;
                continue;
            }    

        printf("lista de elementos: ");
        imprime_elementos(cabeca);

        printf("\ndigite 3 para sair ou qualquer numero para continuar\n");
        scanf("%d", &saida);
    } while(saida != 3);
    
    return 0;
}