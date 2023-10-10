#include <stdio.h>
#include <stdlib.h>
#include "libConfeitaria.h"
#define MAX_BOLO 20
#define MAX_TORTA 10
#define DIA 10

frigobar *criaFrigobar(frigobar *frig)
{
    frig = calloc(1, sizeof(frigobar));

    for(int i = 0; i < MAX_BOLO; i++)
    {
        frig->prat_cima[i] = calloc(1, sizeof(bolo));

        if(i < MAX_TORTA)
            frig->prat_baixo[i] = calloc(1, sizeof(torta));   
    }

    return frig;
}

void alocaProdutos(frigobar *f)
{
    for(int i = 0; i < MAX_BOLO; i++)
        {
            f->prat_cima[i]->sabor = 'c';
            f->prat_cima[i]->dat.dia = DIA;

            if(i < MAX_TORTA)
            {
                f->prat_baixo[i]->dat.dia = DIA;   
                f->prat_baixo[i]->validade = DIA + 5;
            }
        }
}

void destroiFrigobar(frigobar *f)
{
    for(int i = 0; i < MAX_BOLO; i++)
    {
        free(f->prat_cima[i]);

        if(i < MAX_TORTA)
            free(f->prat_baixo[i]);
    }    
    
    free(f);
    f = NULL;
}