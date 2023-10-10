#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libagenda.h"
#include "libfirma.h"
#define MAX_FUNCS 30
#define MAX_TAREFAS 100
#define MAX_EXP 100
#define MAX_DIAS 31
#define MAX_MESES 12

/* retorna um numero aleatorio entre min e max */
int aleat(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

/* cria e inicializa funcionarios */
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

/* cria e inicializa tarefas */
void cria_tarefas(tarefa_t *tarefa)
{
    for (int i = 0; i < MAX_TAREFAS; i++)
    {
        tarefa[i].id = i;
        tarefa[i].temp = aleat(600, 800);
        tarefa[i].dif = aleat(30, 80);
    }
}

/* retorna um horario inicial e final de um compromisso */
horario_compromisso_t cria_horario_reuniao()
{
    horario_compromisso_t hc;

    hc.ini_h = aleat(8, 12);
    hc.ini_m = aleat(0, 3) * 15;
    hc.fim_h = hc.ini_h + aleat(1, 4);
    hc.fim_m = hc.ini_m;

    return hc;
}

/* retorna um ponteiro para um funcionario com lideranca entre 30 e 70 */
func_t *escolhe_lider(func_t *funcionarios)
{
    func_t *lider;

    lider = &funcionarios[aleat(0, 29)];
    while (lider->lid < 30 || lider->lid > 70)
        lider = &funcionarios[aleat(0, 29)];

    return lider;
}

/* retorna uma reuniao com os parametros fornecidos */
compromisso_t *cria_reuniao(int id_lider, int dia, int mes, horario_compromisso_t hc)
{
    compromisso_t *reuniao;
    int id_tarefa;
    char descricao[50];

    id_tarefa = aleat(0, MAX_TAREFAS - 1);
    sprintf(descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", id_lider, dia, mes, hc.ini_h, hc.ini_m, hc.fim_h, hc.fim_m, id_tarefa);
    reuniao = cria_compromisso(hc, id_tarefa, descricao);

    return reuniao;
}

/* avança a agenda de todos os funcionarios para o proximo mes */
void prox_mes_agenda_funcs(func_t *funcionarios)
{
    for (int i = 0; i < MAX_FUNCS; i++)
        prox_mes_agenda(funcionarios[i].agenda);
}

/* marca 100 reunioes por mes, em 12 meses, na agenda de 30 fucionarios */
void marca_reunioes(func_t *funcionarios)
{
    func_t *lider;
    func_t *membro;
    compromisso_t *reuniao;
    int dia, sort, pode;

    for (int mes = 1; mes <= MAX_MESES; mes++)
    {
        printf("M %.2d\n", mes);

        for (int tarefa = 0; tarefa < MAX_TAREFAS; tarefa++)
        {
            pode = 0;
            lider = escolhe_lider(funcionarios);
            dia = aleat(1, 31);
            reuniao = cria_reuniao(lider->id, dia, mes, cria_horario_reuniao());
            printf("%s", descricao_compr(reuniao));
            /* se conseguir marcar a reuniao na agenda do lider */
            if (marca_compromisso_agenda(lider->agenda, dia, reuniao) == 1)
            {
                printf("\tMEMBROS:");
                sort = aleat(2, 6);
                for (int k = 0; k < sort; k++)
                {
                    membro = &funcionarios[aleat(0, 29)];
                    if (lider->lid > membro->lid + aleat(-20, 10))
                    {
                        if (marca_compromisso_agenda(membro->agenda, dia, reuniao) == 1)
                        {
                            printf(" %.2d:OK", membro->id);
                            pode++;
                        }    
                        else
                            printf(" %.2d:IN", membro->id);
                    }
                }
                /* se nenhum membro estiver disponivel */
                if (!pode)
                {
                    printf(" VAZIA");
                    desmarca_compromisso_agenda(lider->agenda, dia, reuniao);
                }
                printf("\n");
            }
            else
                printf("\tLIDER INDISPONIVEL \n");

            destroi_compromisso(reuniao);
        }

        prox_mes_agenda_funcs(funcionarios);
    }
}

/* imprime o numero de tarefas concluidas */
void imprime_tarefas_realizada(tarefa_t *tarefa)
{
    int cont = 0;

    for (int i = 0; i < MAX_TAREFAS; i++)
        if (tarefa[i].temp <= 0)
            cont++;

    printf("TAREFAS REALIZADAS %d\n", cont);
}

/* executa e imprime as reunioes da agenda dos funcionarios */
void realiza_reunioes(func_t *funcionarios, tarefa_t *tarefas)
{
    compromisso_t *aux_compr;
    int reun;
    reun = 0;

    for (int mes = 1; mes <= MAX_MESES; mes++)
    {
        printf("M %.2d\n", mes);
        for (int dia = 1; dia <= MAX_DIAS; dia++)
            for (int func = 0; func < MAX_FUNCS; func++)
            {
                /* pega a lista de compromissos do dia do funcionario */
                aux_compr = compr_agenda(funcionarios[func].agenda, dia);
                while (aux_compr != NULL)
                {
                    printf("%.2d/%.2d F %.2d: %s \n", dia, mes, funcionarios[func].id, descricao_compr(aux_compr));
                    /* diminui o tempo da tarefa se ela ainda nao tiver sido concluida */
                    if (tarefas[aux_compr->id].temp > 0)
                    {
                        reun++;
                        tarefas[aux_compr->id].temp -= (aux_compr->fim - aux_compr->inicio) * (funcionarios[func].exp / 100.0) * ((100 - tarefas[aux_compr->id].dif) / 100.0);
                        if (tarefas[aux_compr->id].temp < 0)
                            tarefas[aux_compr->id].temp = 0;
                        printf("\tT %.2d D %.2d TCR %.2d\n", id_compr(aux_compr), tarefas[id_compr(aux_compr)].dif, tarefas[id_compr(aux_compr)].temp);
                        /* aumenta a experiencia do funcionario */
                        if (funcionarios[func].exp < MAX_EXP)
                            funcionarios[func].exp += 1;
                    }
                    else
                        printf("\t T %.2d  CONCLUIDA\n", id_compr(aux_compr));
                    aux_compr = prox_compr(aux_compr);
                }
            }
        for (int i = 0; i < MAX_FUNCS; i++)
            prox_mes_agenda(funcionarios[i].agenda);
    }
    printf("REUNIOES REALIZADA %d\n", reun);
    imprime_tarefas_realizada(tarefas);
}

/* destroi as agendas de todos os funcionarios */
void destroi_agenda_funcionarios(func_t *funcionarios)
{
    for (int i = 0; i < MAX_FUNCS; i++)
        destroi_agenda(funcionarios[i].agenda);
}
