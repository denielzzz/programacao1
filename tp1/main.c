#include <stdio.h>
#include "libAgenda.h"

int main()
{
	int ano;
	char saida = 'a';
	printf("--> Entre com o ano:\n");
	scanf("%d", &ano);
	struct agenda ag = criaAgenda(ano);
	while(saida != 's') // laço que não para até o usuario digitar "s"
	{
		printf("--> Entre com dia, mês, ano, hora:\n");
		struct compromisso compr = leCompromisso();
		if(validaData(compr.data_compr, ag) && verificaDisponibilidade(compr, ag)) // verifica se a data e a hora são válidas
				ag = marcaCompromisso(ag, compr); // marca compromisso se a data e a hora forem válidos2
		else
			printf("Data inválida, compromisso não inserido\n");
		printf("--> Digite um char qualquer ou s para sair:\n");
		scanf(" %c", &saida);
	}
	listaCompromissos(ag); // printa todos os compromissos marcados
	return 0;
}
