#include <stdio.h>
#include "libAgenda.h"

int main()
{
	int ano;
	struct compromisso compr, *ptr_compr;
	struct agenda ag, *ptr_ag;
	char saida;

	ptr_compr = &compr;
	ptr_ag = &ag;
	scanf("%d", &ano);
	ag = criaAgenda(ano);

	do
	{
		/* le um compromisso e verifica se sua data e valida */
		if(leCompromisso(ptr_ag, ptr_compr))
			if(verificaDisponibilidade(ptr_ag, ptr_compr))
			{
				marcaCompromisso(ptr_ag, ptr_compr);
				printf("Compromisso inserido com sucesso!\n");
			}
			else
				printf("Data/Hora ocupada, compromisso nao inserido\n");
		else
			printf("Data e/ou hora invalidos, compromisso nao inserido\n");

		scanf(" %c", &saida);

	} while(saida != 's');

	listaCompromissos(ptr_ag);

	return 0;
}
