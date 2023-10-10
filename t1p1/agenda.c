#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "libagenda.h"
#define MAX_FUNCS 30
#define MAX_TAREFAS 100
#define MAX_EXP 100

typedef struct func
{
    int id;
    int lid;
    int exp;
    agenda_t *agenda;
} func_t;

typedef struct tarefa
{
    int id;
    int temp;
    int dif;
} tarefa_t;

int aleat(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void cria_funcs(func_t *funcionario)
{
    for (int i = 0; i < MAX_FUNCS; i++)
    {
        funcionario[i].id = i;
        funcionario[i].lid = aleat(0, 100);
        funcionario[i].exp = aleat(20, 100);
        funcionario[i].agenda = cria_agenda();
    }
}

void cria_tarefas(tarefa_t *tarefa)
{
    for (int i = 0; i < MAX_TAREFAS; i++)
    {
        tarefa[i].id = i;
        tarefa[i].temp = aleat(600, 800);
        tarefa[i].dif = aleat(30, 80);
    }
}

horario_compromisso_t cria_horario_reuniao()
{
    horario_compromisso_t hc;

    hc.ini_h = aleat(8, 12);
    hc.ini_m = aleat(0, 45);
    while (hc.ini_m % 15)
        hc.ini_m = aleat(0, 45);
    hc.fim_h = hc.ini_h + aleat(1, 4);
    hc.fim_m = hc.ini_m;

    return hc;
}

func_t *escolhe_lider(func_t *funcionarios)
{
    func_t *lider;

    lider = &funcionarios[aleat(0, 29)];
    while (lider->lid < 30 || lider->lid > 70)
        lider = &funcionarios[aleat(0, 29)];

    return lider;
}

compromisso_t *cria_reuniao(int id_lider, int dia, int mes, horario_compromisso_t hc)
{
    compromisso_t *reuniao;
    int id_tarefa;
    char descricao[40];

    id_tarefa = aleat(0, MAX_TAREFAS - 1);
    sprintf(descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", id_lider, dia, mes, hc.ini_h, hc.ini_m, hc.fim_h, hc.fim_m, id_tarefa);
    reuniao = cria_compromisso(hc, id_tarefa, descricao);

    return reuniao;
}

int desmarca_compromisso_lider(agenda_t *agenda, int dia, compromisso_t *reuniao)
{
    dia_t *aux_dia;
    compromisso_t *aux_compr;
    compromisso_t *desmarca_compr;
    aux_dia = agenda->ptr_mes_atual->dias;
    while (aux_dia != NULL && aux_dia->dia != dia)
        aux_dia = aux_dia->prox;

    if (aux_dia == NULL)
        return 0;

    aux_compr = aux_dia->comprs;
    if (aux_dia->comprs->id == reuniao->id)
    {
        desmarca_compr = aux_dia->comprs;
        aux_dia->comprs = aux_dia->comprs->prox;
        free(desmarca_compr->descricao);
        free(desmarca_compr);
        desmarca_compr = NULL;

        return 1;
    }

    while (aux_compr->prox != NULL && aux_compr->prox->id != reuniao->id)
        aux_compr = prox_compr(aux_compr);

    if (aux_compr->prox == NULL)
        return 0;

    desmarca_compr = aux_compr->prox;
    aux_compr->prox = aux_compr->prox->prox;
    free(desmarca_compr->descricao);
    free(desmarca_compr);
    desmarca_compr = NULL;

    return 1;
}

void marca_reunioes(func_t *funcionarios)
{
    func_t *lider;
    func_t *membro;
    compromisso_t *reuniao;
    int dia, sort, pode;

    for (int mes = 0; mes < 12; mes++)
    {
        printf("M %.2d\n", mes + 1);

        for (int j = 0; j < 100; j++)
        {
            pode = 0;
            lider = escolhe_lider(funcionarios);
            dia = aleat(1, 31);
            reuniao = cria_reuniao(lider->id, dia, mes + 1, cria_horario_reuniao());
            printf("%s", reuniao->descricao);
            if (marca_compromisso_agenda(lider->agenda, dia, reuniao) == 1)
            {
                printf("\tMEMBROS:");
                sort = aleat(2, 6);
                for (int k = 0; k < sort; k++)
                {
                    membro = &funcionarios[aleat(0, 29)];
                    if (lider->lid > membro->lid + aleat(-20, 10))
                    {
                        pode++;
                        if (marca_compromisso_agenda(membro->agenda, dia, reuniao) == 1)
                            printf(" %.2d:OK", membro->id);
                        else
                            printf(" %.2d:IN", membro->id);
                    }
                }
                if (!pode)
                {
                    printf("VAZIA");
                    desmarca_compromisso_lider(lider->agenda, dia, reuniao);
                }
                printf("\n");
            }
            else
                printf("\tLIDER INDISPONIVEL \n");

            free(reuniao->descricao);
            free(reuniao);
            reuniao = NULL;
        }
        for (int i = 0; i < MAX_FUNCS; i++)
            prox_mes_agenda(funcionarios[i].agenda);
    }
}

void imprime_tarefas_realizada(tarefa_t *tarefa)
{
    int cont = 0;

    for (int i = 0; i < MAX_TAREFAS; i++)
        if (tarefa[i].temp <= 0)
            cont++;

    printf("TAREFAS REALIZADAS %d\n", cont);
}

void trabalhar(func_t *funcionarios, tarefa_t *tarefas)
{
    compromisso_t *aux_compr;
    int reun;
    reun = 0;

    for (int mes = 0; mes < 12; mes++)
    {

        for (int i = 0; i < 31; i++)
            for (int j = 0; j < MAX_FUNCS; j++)
            {
                aux_compr = compr_agenda(funcionarios[j].agenda, i + 1);
                while (aux_compr != NULL)
                {
                    reun++;
                    printf("%.2d/%.2d F %.2d: %s \n", i + 1, mes + 1, funcionarios[j].id, aux_compr->descricao);
                    if (tarefas[aux_compr->id].temp > 0)
                    {
                        tarefas[aux_compr->id].temp -= (aux_compr->fim - aux_compr->inicio) * (funcionarios[j].exp / 100.0) * ((100 - tarefas[aux_compr->id].dif) / 100.0);
                        if (tarefas[aux_compr->id].temp < 0)
                            tarefas[aux_compr->id].temp = 0;
                        printf("\tT %.2d D %.2d TCR %.2d\n", aux_compr->id, tarefas[aux_compr->id].dif, tarefas[aux_compr->id].temp);
                    }
                    else
                    {
                        printf("\t T %.2d  CONCLUIDA\n", aux_compr->id);
                        if (funcionarios[j].exp < MAX_EXP)
                            funcionarios[j].exp += 1;
                    }
                    aux_compr = prox_compr(aux_compr);
                }
            }
        for (int i = 0; i < MAX_FUNCS; i++)
            prox_mes_agenda(funcionarios[i].agenda);
    }
    printf("REUNIOES REALIZADA %d\n", reun);
    imprime_tarefas_realizada(tarefas);
}

void destroi_agenda_funcionarios(func_t *funcionarios)
{
    for (int i = 0; i < MAX_FUNCS; i++)
        destroi_agenda(funcionarios[i].agenda);
}

int main()
{
    srand(time(NULL));
    func_t funcionarios[MAX_FUNCS];
    tarefa_t tarefas[MAX_TAREFAS];

    cria_funcs(funcionarios);
    cria_tarefas(tarefas);

    marca_reunioes(funcionarios);

    trabalhar(funcionarios, tarefas);

    destroi_agenda_funcionarios(funcionarios);

    return 0;
}