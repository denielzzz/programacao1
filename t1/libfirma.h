/*
 * TAD firma
 * Autor:
 *    Daniel Celestino de Lins Neto
*/

/* struct funcionario contendo um identificador,
 * a liderança, a experiencia e um ponteiro para uma agenda*/
typedef struct func
{
    int id;
    int lid;
    int exp;
    agenda_t *agenda;
} func_t;

/* struct tarefa contendo um identificador,
 * o tempo restante para ser concluida e a dificuldade */
typedef struct tarefa
{
    int id;
    int temp;
    int dif;
} tarefa_t;

/* retorna um numero aleatorio entre min e max */
int aleat(int min, int max);

/* cria e inicializa funcionarios com um identificador,
 * uma lideranca, uma experiencia e uma agenda */
void cria_funcs(func_t *funcionario);

/* cria e inicializa tarefas com um identificador,
 * um tempo até concluir e uma dificuldade */
void cria_tarefas(tarefa_t *tarefa);

/* retorna um horario inicial e final de um compromisso */
horario_compromisso_t cria_horario_reuniao();

/* retorna um ponteiro para um funcionario com lideranca entre 30 e 70 */
func_t *escolhe_lider(func_t *funcionarios);

/* retorna uma reuniao com os parametros fornecidos */
compromisso_t *cria_reuniao(int id_lider, int dia, int mes, horario_compromisso_t hc);

/* avança a agenda de todos os funcionarios para o proximo mes */
void prox_mes_agenda_funcs(func_t *funcionarios);

/* marca 100 reunioes por mes, em 12 meses, na agenda de 30 fucionarios */
void marca_reunioes(func_t *funcionarios);

/* imprime o numero de tarefas concluidas */
void imprime_tarefas_realizada(tarefa_t *tarefa);

/* executa e imprime as reunioes da agenda dos funcionarios */
void realiza_reunioes(func_t *funcionarios, tarefa_t *tarefas);

/* destroi a agenda de todos os funcionarios */
void destroi_agenda_funcionarios(func_t *funcionarios);
