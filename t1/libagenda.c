#include <stdlib.h>
#include <string.h>
#include "libagenda.h"

/* cria e inicializa o primeiro mes e a agenda, o retorna se for possivel,
 * retorna NULL se não for possivel */
agenda_t *cria_agenda()
{
    agenda_t *nova_agenda;
    if (!(nova_agenda = (agenda_t *)malloc(sizeof(agenda_t))))
        return NULL;
    if (!(nova_agenda->ptr_mes_atual = malloc(sizeof(mes_t))))
        return NULL;
    /* inicializa o primeiro mes */
    nova_agenda->mes_atual = 1;
    nova_agenda->ptr_mes_atual->mes = 1;
    nova_agenda->ptr_mes_atual->dias = NULL;
    nova_agenda->ptr_mes_atual->prox = nova_agenda->ptr_mes_atual;
    nova_agenda->ptr_mes_atual->ant = nova_agenda->ptr_mes_atual;

    return nova_agenda;
}

/* retorna um compromisso com as informacoes de data de hc, um identificador
 * id e uma string de descricao, ou NULL se nao for possivel criar o compromisso */
compromisso_t *cria_compromisso(horario_compromisso_t hc, int id, char *descricao)
{
    compromisso_t *novo_compromisso;
    if (!(novo_compromisso = (compromisso_t *)malloc(sizeof(compromisso_t))))
        return NULL;
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

/* destroi a descricao de um compromisso */
static void destroi_descricao_compromisso(compromisso_t *compr)
{
    free(compr->descricao);
    compr->descricao = NULL;
}

/* destroi um compromisso */
void destroi_compromisso(compromisso_t *compr)
{
    destroi_descricao_compromisso(compr);
    free(compr);
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
        destroi_dias_mes(agenda);
        aux_destroi_mes = agenda->ptr_mes_atual;
        agenda->ptr_mes_atual = agenda->ptr_mes_atual->prox;
        agenda->mes_atual = agenda->ptr_mes_atual->mes;
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
}

/* procura se o dia existe na lista, se existir
retorna seu endereco, se nao, retorna NULL. */
static dia_t *procura_dia(dia_t *lista_dias, int dia)
{
    while (lista_dias != NULL)
    {
        if (lista_dias->dia == dia)
            return lista_dias;
        lista_dias = lista_dias->prox;
    }

    return NULL;
}

/* cria e insere um dia em uma lista ordenada, retorna 1
 * se for possivel e 0 se nao for possivel */
static int insere_dia(dia_t **lista_dia, int dia)
{
    dia_t *novo_dia;
    
    /* cria espaco para o novo dia, retorna 0 se nao for possivel */
    if (!(novo_dia = (dia_t *)malloc(sizeof(dia_t))))
        return 0;
    novo_dia->dia = dia;
    novo_dia->comprs = NULL;
    novo_dia->prox = NULL;

    /* se a lista estiver vazia, isere o dia na lista */
    if (!(*lista_dia))
        *lista_dia = novo_dia;

    /* se o dia do compromisso for o menor da lista, insere o dia no comeco */
    if (dia < (*lista_dia)->dia)
    {
        novo_dia->prox = *lista_dia;
        *lista_dia = novo_dia;
    }
    /* se o dia do compromisso for maior que o primeiro da lista,
     * insere o dia ordenado na lista de dias */
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
 * retorna 1 se for possivel, ou 0 se nao for possivel */
static int insere_compromisso(compromisso_t **lista_compr, compromisso_t *compr)
{
    /* se a lista estiver vazia, insere o compromisso, senao, verifica se o
     * compromisso tem o mesmo horario de inicio que o primeiro da lista */
    if (!(*lista_compr))
        *lista_compr = compr;
    else if ((*lista_compr)->inicio == compr->inicio)
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
            aux_compr = prox_compr(aux_compr);

        if (prox_compr(aux_compr) == NULL)
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
    if (dia < 1 || dia > 31 || compr->inicio < 0 || compr->fim >= 1439 || compr->inicio >= compr->fim)
        return 0;

    dia_t *aux_dia;
    compromisso_t *novo_compr;

    /* cria um novo compromisso, retorna 0 se houver erro de alocacao de memoria */
    if(!(novo_compr = cria_compromisso(hc_compr(compr), compr->id, compr->descricao)))
        return 0;

    /* se o dia passado nao existir, o cria e insere ordenado da lista de dias */
    if (!(aux_dia = procura_dia(agenda->ptr_mes_atual->dias, dia)))
    {
        insere_dia(&agenda->ptr_mes_atual->dias, dia);
        aux_dia = procura_dia(agenda->ptr_mes_atual->dias, dia);
    }
    /* se nao for possivel marcar o novo compromisso, o destroi */
    if (!insere_compromisso(&aux_dia->comprs, novo_compr))
    {
        destroi_compromisso(novo_compr);
        novo_compr = NULL;

        return -1;
    }

    return 1;
}

/* busca um compromisso passado por parametro, se o encontrar, retorna o endereco
 * de seu compromisso anterior, se nao retorna NULL */
static compromisso_t *busca_compromisso(compromisso_t *lista_compr, compromisso_t *compr)
{
    compromisso_t *aux_compr;

    aux_compr = lista_compr;
    /* se a lista estiver vazia ou compr nao tiver antecessor */
    if ((!aux_compr) || aux_compr == compr)
        return NULL;

    while (aux_compr->prox != NULL && aux_compr->prox->inicio != compr->inicio)
        aux_compr = prox_compr(aux_compr);
    /* se nao achar o compromisso */
    if (aux_compr->prox == NULL    )
        return NULL;

    return aux_compr;
}

/* desmarca o compromisso compr da agenda, retorna 1 se for possivel,
 * e 0 caso nao for possivel */
int desmarca_compromisso_agenda(agenda_t *agenda, int dia, compromisso_t *compr)
{
    dia_t *aux_dia;
    compromisso_t *aux_destroi_compr;

    /* se o dia passado nao existir */
    if (!(aux_dia = procura_dia(agenda->ptr_mes_atual->dias, dia)))
        return 0;
    /* se o dia nao tiver compromissos */
    if (!(aux_dia->comprs))
        return 0;
    /* se o compromisso a ser desmarcado for o primeiro da lista */
    if (aux_dia->comprs->inicio == compr->inicio)
    {
        aux_destroi_compr = aux_dia->comprs;
        aux_dia->comprs = aux_dia->comprs->prox;
        destroi_compromisso(aux_destroi_compr);

        return 1;
    }

    /* se o compromisso a ser desmarcado nao for o primeiro da lista */
    compromisso_t *aux_compr;
    /* se nao for possivel achar o compromisso */
    if (!(aux_compr = busca_compromisso(aux_dia->comprs, compr)))
        return 0;
    aux_destroi_compr = aux_compr->prox;
    aux_compr->prox = prox_compr(aux_compr->prox);
    destroi_compromisso(aux_destroi_compr);

    return 1;
}

/* retorna o mes atual da agenda. */
int mes_atual_agenda(agenda_t *agenda)
{
    return agenda->mes_atual;
}

/* ajusta o mes_atual para 1 e aponta prt_mes_atual para o mes 1 na Lista de
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
        /* cria espaco para o novo mes, retorna 0 se nao for possivel */
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

/* analogo ao prox_mes_agenda porem decrementa mes_atual. */
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
        /* cria espaco para o novo mes, retorna 0 se nao for possivel */
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
    aux_dia = procura_dia(agenda->ptr_mes_atual->dias, dia);
    /* se o dia passado por parametro nao existir */
    if (!aux_dia)
        return NULL;

    return aux_dia->comprs;
}

/* retorna o proximo compromisso da lista de compromissos compr.*/
compromisso_t *prox_compr(compromisso_t *compr)
{
    return compr->prox;
}

/* as funcoes abaixo sao usadas para acessar os membros da struct compromisso
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