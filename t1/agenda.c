#include <stdlib.h>
#include <time.h>
#include "libagenda.h"
#include "libfirma.h"
#define MAX_FUNCS 30
#define MAX_TAREFAS 100

int main()
{
    srand(time(NULL));
    func_t funcionarios[MAX_FUNCS];
    tarefa_t tarefas[MAX_TAREFAS];

    cria_funcs(funcionarios);
    cria_tarefas(tarefas);
    marca_reunioes(funcionarios);
    realiza_reunioes(funcionarios, tarefas);
    destroi_agenda_funcionarios(funcionarios);

    return 0;
}