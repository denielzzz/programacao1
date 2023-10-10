#include <stdio.h>
#define MAX_BOLO 20
#define MAX_TORTA 10

/**/
typedef struct tipo_data
{
    int dia;

}data;

/**/
typedef struct tipo_bolo
{
    data dat;
    char sabor;

}bolo;

/**/
typedef struct tipo_torta
{
    data dat;
    int validade;
}torta;

/**/
typedef struct tipo_frigobar
{
    bolo *prat_cima[MAX_BOLO];
    torta *prat_baixo[MAX_TORTA];

}frigobar;

/**/
frigobar *criaFrigobar(frigobar *frig);

/**/
void alocaProdutos(frigobar *f);

/**/
void destroiFrigobar(frigobar *f);