#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "libAgenda.h"
#define HORAS_DO_DIA 24
#define DIAS_DO_ANO 365

/* esta função transforma uma data lida pelo usuário para uma struct data
 * em uma data do tipo struct tm definida pela biblioteca time.h. 
 * Esta data é então transformada em segundos com a função mktime, para que
 * os outros membros da struct sejam preenchidos automaticamente, e então
 * reconvertida para uma data do tipo struct tm, reescrevendo a variável 
 * original. Com isso, pode-se obter o membro tm_yday, que representa o
 * dia do ano representado pela data: um inteiro entre 0 e 364 */
int obtemDiaDoAno(struct data d) {
	struct tm tipodata;
	time_t segundos;
	int dia_do_ano;

	tipodata.tm_mday = d.dia;
    tipodata.tm_mon = d.mes-1;
    tipodata.tm_year = d.ano-1900;
    tipodata.tm_isdst = -1;
	tipodata.tm_hour = 0;

	/* converte data para época, isto é, segundos desde 1970 */
	segundos = mktime(&tipodata);
	/* converte época em data, obtendo assim automaticamente
	 * o campo 'dia do ano' (tm_yday) que será o índice do
	 * vetor necessário para marcar um compromisso */
	tipodata = *localtime(&segundos);
	/* da reconversão da data, obtém o dia do ano, um número
	 * entre 0 e 364 */
    dia_do_ano = tipodata.tm_yday;
	return dia_do_ano;
}

/* inicializa a agenda do ano corrente, onde cada hora de cada dia deve ter o 
 * valor 0 para indicar que nao ha um compromisso marcado. Retorna uma agenda
 * livre */ 
struct agenda criaAgenda(int ano)
{
	struct agenda age_livre;
	age_livre.ano = ano;
	for(int c_dias = 0; c_dias < DIAS_DO_ANO; c_dias++) //laço que conta de 0 a 364
		for(int c_horas = 0; c_horas < HORAS_DO_DIA; c_horas++) //laço que conta de 0 a 23
			age_livre.agenda_do_ano[c_dias].horas[c_horas] = 0; // zera todos os compromissos de todos os dias de um ano
	return age_livre; // retorna a agenda livre
}

/* le um compromisso passado pelo usuario, com data completa e hora pretendida. 
 * Cabe ao usuario desta funcao validar a data e hora do compromisso */
struct compromisso leCompromisso()
{
	struct compromisso compr;
	scanf("%d %d %d %d", &compr.data_compr.dia, &compr.data_compr.mes, &compr.data_compr.ano, &compr.hora_compr); //lê respectivamente o dia, mês, ano e hora do compromisso
	return compr;
}


/* dado um compromisso, retorna a hora definida */
int obtemHora(struct compromisso compr)
{
	return compr.hora_compr; //retorna a hora do compromisso
}


/* retorna o ano atribuido a uma agenda criada */
int obtemAno(struct agenda ag)
{
	return ag.ano; //retorna o ano da agenda
}


/* Valida uma data lida do usuario;
 * Retorna 1 se a data for valida e 0 caso contrario */
int validaData(struct data d, struct agenda ag)
{
	if((obtemAno(ag) == d.ano) && (1 <= d.mes && d.mes <= 12) && (d.dia >= 1 && d.dia <= 28)) // verifica se o ano é compativel e se o mes e o dia são possíveis, para maior parte dos casos
		return 1;
	if((d.mes == 1 || d.mes == 3 || d.mes == 5 || d.mes == 7 || d.mes == 8 || d.mes == 10 || d.mes == 12) && (d.dia == 30 ||d.dia == 31)) //retorna positivo se o mês tiver 31 dias e encaixar com o dia da data
			return 1;
	if((d.mes == 4 || d.mes == 6 || d.mes == 9 || d.mes == 11) && (d.dia == 30)) // retorna positivo se o mês tiver 30 dias e encaixar com o dia da data
		return 1;
	return 0;
}

/* Retorna 0 se data e horario já estiverem ocupados, 1 se data e horario
 * estiverem livres */
int verificaDisponibilidade(struct compromisso compr, struct agenda ag)
{
	if(ag.agenda_do_ano[obtemDiaDoAno(compr.data_compr)].horas[obtemHora(compr)] == 1) // verifica se já existe algum compromisso no horario
		return 0;
	return 1;
}

/* Dada uma agenda e um compromisso valido, isto eh, com data/hora validos, 
 * hora livre e dentro do ano da agenda, muda o valor da hora do compromisso
 * de 0 (livre) para 1 (ocupado). Retorna a nova agenda com o compromisso
 * marcado. */
struct agenda marcaCompromisso(struct agenda ag, struct compromisso compr)
{
	if(obtemHora(compr) >= 0 && obtemHora(compr) <= 23) // verifica se a hora está entre 0 e 23
	{
		printf("Compromisso inserido com sucesso!\n");
		ag.agenda_do_ano[obtemDiaDoAno(compr.data_compr)].horas[obtemHora(compr)] = 1; // marca o compromisso na hora indicada
		return ag;
	}
	printf("Hora inválida, compromisso não inserido\n");
	return ag;
}

/* mostra as datas e horas de todos os compromissos marcados na agenda */
void listaCompromissos(struct agenda ag)
{
	for(int c_dias = 0; c_dias < DIAS_DO_ANO; c_dias++) // conta de 0 a 364
		for(int c_horas = 0; c_horas < HORAS_DO_DIA; c_horas++) // conta de 0 a 23
			if(ag.agenda_do_ano[c_dias].horas[c_horas] == 1) // printa se encontrar um compromisso
				printf("dia: %d, ano:  %d, hora:  %d, compromisso!\n", c_dias, ag.ano, c_horas);
}
