#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libagenda.h"

agenda_t *cria_agenda()
{
    agenda_t *nova_agenda;
    /* cria espaço para a agenda, retorna NULL se não for possivel */
    if (!(nova_agenda = (agenda_t *)malloc(sizeof(agenda_t))))
        return NULL;
    if (!(nova_agenda->ptr_mes_atual = malloc(sizeof(mes_t))))
        return NULL;
    /* inicializa o primeiro mes da agenda */
    nova_agenda->mes_atual = 1;
    nova_agenda->ptr_mes_atual->mes = 1;
    nova_agenda->ptr_mes_atual->dias = NULL;
    nova_agenda->ptr_mes_atual->prox = nova_agenda->ptr_mes_atual;
    nova_agenda->ptr_mes_atual->ant = nova_agenda->ptr_mes_atual;

    return nova_agenda;
}

/* retorna um compromisso com as informacoes de data de hc, um identificador
id e uma string de descricao, ou NULL se nao for possivel criar o compromisso */
compromisso_t *cria_compromisso(horario_compromisso_t hc, int id, char *descricao)
{
    compromisso_t *novo_compromisso;
    /* cria espaço para compromisso, retorna NULL se não for possivel */
    if (!(novo_compromisso = (compromisso_t *)malloc(sizeof(compromisso_t))))
        return NULL;
    /* cria espaço para a string descricao, retorna NULL se não for possivel */
    if (!(novo_compromisso->descricao = (char *)malloc(sizeof(char) * (strlen(descricao) + 1))))
        return NULL;
    /* copia string para o espaço alocado */
    strcpy(novo_compromisso->descricao, descricao);
    novo_compromisso->id = id;
    novo_compromisso->inicio = hc.ini_h * 60 + hc.ini_m;
    novo_compromisso->fim = hc.fim_h * 60 + hc.fim_m;
    novo_compromisso->prox = NULL;

    return novo_compromisso;
}

/* destroi todos os compromissos de um dia */
static void destroi_compromissos_dia(agenda_t *agenda)
{
    while (agenda->ptr_mes_atual->dias->comprs != NULL)
        desmarca_compromisso_agenda(agenda, agenda->ptr_mes_atual->dias->dia, agenda->ptr_mes_atual->dias->comprs);
}

/* destroi todos os dias de um mes */
static void destroi_dias_mes(agenda_t *agenda)
{
    dia_t *aux_destroi_dia;

    while (agenda->ptr_mes_atual->dias != NULL)
    {
        destroi_compromissos_dia(agenda);
        aux_destroi_dia = agenda->ptr_mes_atual->dias;
        agenda->ptr_mes_atual->dias = agenda->ptr_mes_atual->dias->prox;
        free(aux_destroi_dia);
        aux_destroi_dia = NULL;
    }
}

/* destroi todos os meses de uma agenda */
static void destroi_meses(agenda_t *agenda)
{
    mes_t *aux_destroi_mes;

    while (agenda->ptr_mes_atual != agenda->ptr_mes_atual->prox)
    {
        agenda->mes_atual = agenda->ptr_mes_atual->mes;
        destroi_dias_mes(agenda);
        aux_destroi_mes = agenda->ptr_mes_atual;
        agenda->ptr_mes_atual = agenda->ptr_mes_atual->prox;
        aux_destroi_mes->ant->prox = aux_destroi_mes->prox;
        aux_destroi_mes->prox->ant = aux_destroi_mes->ant;
        free(aux_destroi_mes);
        aux_destroi_mes = NULL;
    }

    destroi_dias_mes(agenda);
    free(agenda->ptr_mes_atual);
    agenda->ptr_mes_atual = NULL;
}

/* destroi uma agenda */
void destroi_agenda(agenda_t *agenda)
{
    destroi_meses(agenda);
    free(agenda);
    agenda = NULL;
}

/* procura se o dia existe na lista, se existir
retorna o endereco dele, se nao, retorna NULL. */
static dia_t *procura_dia(dia_t *lista_dias, int dia)
{
    if (!lista_dias)
        return NULL;

    dia_t *aux_dia;

    aux_dia = lista_dias;
    while (aux_dia->prox != NULL && aux_dia->prox->dia <= dia)
        aux_dia = aux_dia->prox;

    if (aux_dia->dia == dia)
        return aux_dia;

    return NULL;
}

/* cria e insere um dia em uma lista ordenada, retorna 1
 * se der certo e 0 se der errado */
static int insere_dia(dia_t **lista_dia, int dia)
{
    dia_t *novo_dia;

    if (!(novo_dia = (dia_t *)malloc(sizeof(dia_t))))
        return 0;
    novo_dia->dia = dia;
    novo_dia->comprs = NULL;
    novo_dia->prox = NULL;

    /* se a lista estiver vazia */
    if (!(*lista_dia))
        *lista_dia = novo_dia;

    /* se o dia do compromisso for o menor da lista */
    if (dia < (*lista_dia)->dia)
    {
        novo_dia->prox = *lista_dia;
        *lista_dia = novo_dia;
    }
    if (dia > (*lista_dia)->dia)
    {
        dia_t *aux_dia;
        aux_dia = *lista_dia;
        while (aux_dia->prox != NULL && aux_dia->prox->dia < dia)
            aux_dia = aux_dia->prox;
        novo_dia->prox = aux_dia->prox;
        aux_dia->prox = novo_dia;
    }

    return 1;
}

/* verifica se a interseccao entre dois compromissos, se tiver retorna 1,
 * se nao, retorna 0. */
static int testa_intersec(compromisso_t *compr, compromisso_t *comp2)
{
    if (compr->fim <= comp2->inicio || compr->inicio >= comp2->fim)
        return 0;

    return 1;
}

/* insere um compromisso dado em uma lista ordenada de compromissos,
 * devolve o delvolve 1 se for possivel, ou 0 se nao for possivel */
static int insere_compromisso(compromisso_t **lista_compr, compromisso_t *compr)
{
    /* se a lista estiver vazia */
    if (!(*lista_compr))
        *lista_compr = compr;
    else
    if((*lista_compr)->inicio == compr->inicio)
        return 0;

    /* se o compromisso que será inserido for o que começar mais cedo da lista */
    if (compr->inicio < (*lista_compr)->inicio)
    {
        if (testa_intersec(*lista_compr, compr))
            return 0;
        compr->prox = *lista_compr;
        *lista_compr = compr;
    }
    /* se o compromisso que será inserido nao for o que começar mais cedo da lista */
    if (compr->inicio > (*lista_compr)->inicio)
    {
        compromisso_t *aux_compr;

        aux_compr = *lista_compr;
        while (aux_compr->prox != NULL && aux_compr->prox->inicio < compr->inicio)
            aux_compr = aux_compr->prox;

        if (aux_compr->prox == NULL)
        {
            if (testa_intersec(aux_compr, compr))
                return 0;
            aux_compr->prox = compr;
        }
        else
        {
            if (testa_intersec(aux_compr, compr) || testa_intersec(compr, aux_compr->prox))
                return 0;
            compr->prox = aux_compr->prox;
            aux_compr->prox = compr;
        }
    }
    return 1;
}

/* marca um compromisso na agenda:
   valores de retorno possiveis:
    -1: compromisso tem interseccao com outro
     0: erro de alocacao de memoria
     1: sucesso
    a lista de compromisso eh ordenada pelo horario de inicio. */
int marca_compromisso_agenda(agenda_t *agenda, int dia, compromisso_t *compr)
{
    /* verifica se o dia ou o compromisso sao invalidos */
    // if (dia < 1 || dia > 31 || compr->inicio < 0 || compr->fim >= 1440 || compr->inicio >= compr->fim)
    //     return 0;

    dia_t *aux_dia;
    compromisso_t *novo_compr;
    
    novo_compr = cria_compromisso(hc_compr(compr), compr->id, compr->descricao);

    /* se o dia nao existir, cria o dia na lista */
    if (!(aux_dia = procura_dia(agenda->ptr_mes_atual->dias, dia)))
    {
        insere_dia(&agenda->ptr_mes_atual->dias, dia);
        aux_dia = procura_dia(agenda->ptr_mes_atual->dias, dia);
    }
    if (!insere_compromisso(&aux_dia->comprs, novo_compr))
    {
        free(novo_compr->descricao);
        free(novo_compr);
        novo_compr = NULL;
        
        return -1;
    }

    return 1;
}

/* busca um dia passado por parametro, retorna seu endereco
 * se encontrar, ou NULL se nao encontrar */
static dia_t *busca_dia(dia_t *lista_dias, int dia)
{
    dia_t *aux_dia;

    aux_dia = lista_dias;

    while (aux_dia->dia != dia && aux_dia->prox != NULL)
        aux_dia = aux_dia->prox;
    /* se nao achar o dia */
    if (aux_dia->dia != dia)
        return NULL;

    return aux_dia;
}

/* busca um compromisso passado por parametro, se encontrar retorna o endereco
 * de quem aponta para ele, se nao retorna NULL */
static compromisso_t *busca_compromisso(compromisso_t *lista_compr, compromisso_t *compr)
{
    compromisso_t *aux_compr;

    aux_compr = lista_compr;

    while (aux_compr->prox != compr && aux_compr->prox != NULL)
        aux_compr = aux_compr->prox;
    /* se nao achar o compromisso */
    if (aux_compr->prox != compr)
        return NULL;

    return aux_compr;
}

/* Desmarca o compromisso compr da agenda:
   valores de retorno possiveis:
    1: em caso de sucesso
    0: caso nao tenha encontrado o compr */
int desmarca_compromisso_agenda(agenda_t *agenda, int dia, compromisso_t *compr)
{
    dia_t *aux_dia;

    /* se o dia passado nao existir */
    if (!(aux_dia = busca_dia(agenda->ptr_mes_atual->dias, dia)))
        return 0;
    /* se o dia nao tiver compromissos */
    if (!(aux_dia->comprs))
        return 0;
    /* se o compromisso a ser desmarcado for o primeiro da lista */
    if (aux_dia->comprs == compr)
    {
        aux_dia->comprs = aux_dia->comprs->prox;
        free(compr->descricao);
        free(compr);
        compr = NULL;

        return 1;
    }
    /* se o compromisso a ser desmarcado nao for o primeiro da lista */
    compromisso_t *aux_compr;
    if (!(aux_compr = busca_compromisso(aux_dia->comprs, compr)))
        return 0;
    aux_compr->prox = aux_compr->prox->prox;
    free(compr->descricao);
    free(compr);
    compr = NULL;

    return 1;
}

/* Retorna o mes atual da agenda. */
int mes_atual_agenda(agenda_t *agenda)
{
    return agenda->mes_atual;
}

/* Ajusta o mes_atual para 1 e aponta prt_mes_atual para o mes 1 na Lista de
 * meses  */
void prim_mes_agenda(agenda_t *agenda)
{
    while (agenda->ptr_mes_atual->mes != 1)
        agenda->ptr_mes_atual = agenda->ptr_mes_atual->prox;

    agenda->mes_atual = agenda->ptr_mes_atual->mes;
}

/* avanca a agenda para o proximo mes, incrementando mes_atual.
 * se o novo mes_atual nao existir, ele é criado. a funcao retorna o inteiro
 * mes_atual em caso de sucesso ou 0 caso contrario.  */
int prox_mes_agenda(agenda_t *agenda)
{
    /* se estiver no mes 12 */
    if (agenda->ptr_mes_atual->mes == 12)
    {
        prim_mes_agenda(agenda);
        return agenda->mes_atual;
    }
    /* se o proximo mes ainda nao tiver sido criado */
    if (mes_atual_agenda(agenda) != agenda->ptr_mes_atual->prox->mes - 1)
    {
        mes_t *novo_mes;

        if (!(novo_mes = (mes_t *)malloc(sizeof(mes_t))))
            return 0;

        novo_mes->mes = agenda->mes_atual + 1;
        novo_mes->dias = NULL;
        novo_mes->prox = agenda->ptr_mes_atual->prox;
        novo_mes->ant = agenda->ptr_mes_atual;
        agenda->ptr_mes_atual->prox->ant = novo_mes;
        agenda->ptr_mes_atual->prox = novo_mes;
        agenda->ptr_mes_atual = novo_mes;
        agenda->mes_atual = novo_mes->mes;

        return agenda->mes_atual;
    }
    /* se o proximo mes ja existir */
    agenda->ptr_mes_atual = agenda->ptr_mes_atual->prox;
    agenda->mes_atual = agenda->ptr_mes_atual->mes;

    return agenda->mes_atual;
}

/* Analogo ao prox_mes_agenda porem decrementa mes_atual. */
int ant_mes_agenda(agenda_t *agenda)
{
    /* se estiver no mes 1 e o mes 12 existir */
    if (agenda->ptr_mes_atual->ant->mes == 12)
    {
        agenda->ptr_mes_atual = agenda->ptr_mes_atual->ant;
        agenda->mes_atual = agenda->ptr_mes_atual->mes;

        return agenda->mes_atual;
    }
    /* se o mes anterior nao existir */
    if (mes_atual_agenda(agenda) != agenda->ptr_mes_atual->ant->mes + 1)
    {
        mes_t *novo_mes;

        if (!(novo_mes = (mes_t *)malloc(sizeof(mes_t))))
            return 0;
        /* se estiver no mes 1 */
        if (mes_atual_agenda(agenda) == 1)
        {
            novo_mes->mes = 12;
            novo_mes->dias = NULL;
            novo_mes->prox = agenda->ptr_mes_atual;
            novo_mes->ant = agenda->ptr_mes_atual->ant;
            agenda->ptr_mes_atual->ant->prox = novo_mes;
            agenda->ptr_mes_atual->ant = novo_mes;
            agenda->ptr_mes_atual = novo_mes;
            agenda->mes_atual = novo_mes->mes;

            return agenda->mes_atual;
        }

        novo_mes->mes = agenda->mes_atual - 1;
        novo_mes->dias = NULL;
        novo_mes->prox = agenda->ptr_mes_atual;
        novo_mes->ant = agenda->ptr_mes_atual->ant;
        agenda->ptr_mes_atual->ant->prox = novo_mes;
        agenda->ptr_mes_atual->ant = novo_mes;
        agenda->ptr_mes_atual = novo_mes;
        agenda->mes_atual = novo_mes->mes;

        return agenda->mes_atual;
    }
    /* se o mes anterior ja existir */
    agenda->ptr_mes_atual = agenda->ptr_mes_atual->ant;
    agenda->mes_atual = agenda->ptr_mes_atual->mes;

    return agenda->mes_atual;
}

/* retorna um ponteiro para a lista ligada de compromissos de um dia do mes
   ou NULL se vazia. */
compromisso_t *compr_agenda(agenda_t *agenda, int dia)
{
    dia_t *aux_dia;
    /* se o mes atual nao tiver nem um dia criado */
    if (!(aux_dia = agenda->ptr_mes_atual->dias))
        return NULL;

    while (aux_dia->dia != dia && aux_dia->prox != NULL)
        aux_dia = aux_dia->prox;
    /* se o dia passado nao existir */
    if (aux_dia->dia != dia)
        return NULL;

    return aux_dia->comprs;
}

/* Retorna o proximo compromisso da lista de compromissos compr.*/
compromisso_t *prox_compr(compromisso_t *compr)
{
    return compr->prox;
}

/* As funcoes abaixo sao usadas para acessar os membros da struct compromisso
   obtidos com a funcao prox_compr. */
horario_compromisso_t hc_compr(compromisso_t *compr)
{
    horario_compromisso_t hc;

    hc.ini_h = compr->inicio / 60;
    hc.ini_m = compr->inicio % 60;
    hc.fim_h = compr->fim / 60;
    hc.fim_m = compr->fim % 60;

    return hc;
}

int id_compr(compromisso_t *compr)
{
    return compr->id;
}

char *descricao_compr(compromisso_t *compr)
{
    return compr->descricao;
}

/* Imprime a agenda do mes atual (mes atual) */
void imprime_agenda_mes(agenda_t *agenda)
{
    dia_t *aux_dia;
    compromisso_t *aux_compr;

    aux_dia = agenda->ptr_mes_atual->dias;

    printf("================================\n");
    printf("Compromissos do mês: %d\n", agenda->ptr_mes_atual->mes);
    while (aux_dia != NULL)
    {
        printf("================================\n");
        printf("Dia: %d\n\n", aux_dia->dia);
        aux_compr = aux_dia->comprs;
        while (aux_compr != NULL)
        {
            printf("Compromisso de ID: %.2d\n", aux_compr->id);
            printf("Endereço: %p\n", aux_compr);
            printf("Compromisso: %s\n", aux_compr->descricao);
            printf("Inicio: %d\n", aux_compr->inicio);
            printf("Fim: %d\n", aux_compr->fim);
            printf("Proximo: %p\n", aux_compr->prox);
            printf("\n");
            aux_compr = aux_compr->prox;
        }
        aux_dia = aux_dia->prox;
    }
}