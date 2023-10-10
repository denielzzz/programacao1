/* 
 * Feito por Marcos Castilho em 06/04/2023
 * para a disciplina CI1001 - Programacao 1
 * obs.: a funcao obtemDiaDoAno foi feita
 * pelo prof. Andre Gregio.
 */

#include <stdio.h>
#include <time.h>
#include "libAgenda.h"

#define LIVRE 0
#define OCUPADA 1

int obtemDiaDoAno(struct data d){
    struct tm tipodata={0};
    time_t segundos;
    int dia_do_ano;

    tipodata.tm_mday = d.dia;
    tipodata.tm_mon = d.mes - 1;
    tipodata.tm_year = d.ano - 1900;
    tipodata.tm_isdst = -1;
    tipodata.tm_hour = 0;

    /* converte data para epoca, isto eh, segundos desde 1970 */
    segundos = mktime(&tipodata);

    /* converte epoca em data, obtendo assim automaticamente
     * o campo 'dia do ano' (tm_yday) que sera o indice do
     * vetor necessario para marcar um compromisso */
    tipodata = *localtime(&segundos);

    /* da reconversao da data, obtem o dia do ano, um numero
     * entre 0 e 364 */
    dia_do_ano = tipodata.tm_yday;

    return dia_do_ano;
}

/* inicializa a agenda do ano corrente e retorna a agenda livre */
struct agenda criaAgenda(int ano)
{
	struct agenda age_livre;

	age_livre.ano = ano;

	/* zera todas as horas de todos os dias de um ano */
	for(int c_dias = 0; c_dias < DIAS_DO_ANO; c_dias++)
		for(int c_horas = 0; c_horas < HORAS_DO_DIA; c_horas++)
			age_livre.agenda_do_ano[c_dias].horas[c_horas] = LIVRE;

	return age_livre;
}

/* le um compromisso do teclado (dia, mes, ano e hora, nesta ordem)
 * devolve o compromisso no parametro e retorna 1 se o compromisso
 * eh valido ou 0 caso contrario */
int leCompromisso(struct agenda *ag, struct compromisso *compr)
{
	scanf("%d %d %d %d", &compr->data_compr.dia, &compr->data_compr.mes,
				&compr->data_compr.ano, &compr->hora_compr);

	if(validaData(ag, &compr->data_compr) && validaHora(compr))
		return 1;

	return 0;
}

/* dado um compromisso, retorna a hora definida */
int obtemHora(struct compromisso *compr)
{
	return compr->hora_compr;
}

/* retorna o ano atribuido a uma agenda criada */
int obtemAno(struct agenda *ag)
{
	return ag->ano;
}


/* valida uma data lida do usuario;
 * retorna 1 se a data for valida e 0 caso contrario */
int validaData(struct agenda *ag, struct data *d)
{
	if((obtemAno(ag) == d->ano) && ((1 <= d->mes) && (d->mes <= 12)) && 
				       ((d->dia >= 1) && (d->dia <= 28)))
		return 1;
	else
		/* verifica se os dias são condizentes com os meses */
		switch(d->mes)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				if(d->dia == 29 || d->dia == 30 || d->dia == 31)
					return 1;
				break;

			case 4:
			case 6:
			case 9:
			case 11:
				if(d->dia == 29 || d->dia == 30)
					return 1;
				break;
		}

	return 0;
}

/* retorna 0 se data e horario já estiverem ocupados, ou 1 caso contrario */
int verificaDisponibilidade(struct agenda *ag, struct compromisso *compr)
{
	/* verifica se já existe algum compromisso no horario */
	if(ag->agenda_do_ano[obtemDiaDoAno(compr->data_compr)].horas[obtemHora(compr)] == OCUPADA)
		return 0;

	return 1;
}

/* valida uma hora lida do usuario; 
 * retorna 1 se a hora for valida e 0 caso contrario */
int validaHora(struct compromisso *compr)
{
	if(compr->hora_compr >= 0 && compr->hora_compr <= 23)
		return 1;

	return 0;
}

/* esta funcao considera que o comprimisso eh valido e a agenda esta livre
 * para da data/hora fornecidos, portanto quem chama esta funcao tem que
 * garantir estas informacoes. Portanto, a funcao simplesmente muda o valor
 * da hora do compromisso de livre para ocupado */
void marcaCompromisso(struct agenda *ag, struct compromisso *compr)
{
	ag->agenda_do_ano[obtemDiaDoAno(compr->data_compr)].horas[obtemHora(compr)] = OCUPADA;
}

/* mostra as datas e horas de todos os compromissos marcados na agenda.
 * se a agenda nao tiver compromissos agendados nao imprime nada */
void listaCompromissos(struct agenda *ag)
{
	/* percorre o vetor de dias */
	for(int c_dias = 0; c_dias < DIAS_DO_ANO; c_dias++)
		/* percorre o vetor de horas */
		for(int c_horas = 0; c_horas < HORAS_DO_DIA; c_horas++)
			/* printa se achar um compromisso */
			if(ag->agenda_do_ano[c_dias].horas[c_horas] == OCUPADA)
			{
				printf("dia: %d, ", c_dias);
				printf("ano: %d, ", ag->ano);
				printf("hora: %d, compromisso!\n", c_horas);
			}
}
