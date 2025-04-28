#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "data.h"
#include "util.h"

/* dado um array de int (com 0s ou 1s) e um gender, criar um wug e
   retorna seu endereço. */

wug_t* create_wug(int genome[16], gender_t g);


/* funções auxiliares para imprimir um wug e imprimir a população
   atual de wugs. Estas funções serão úteis para debug. */

void  print_wug(const wug_t *a);
void  print_population(const wug_t population[], const int size);


/* dado um genoma, esta função internamente sabe o genoma do superwug
   e o mapeamento de zonas. Usando estas duas informações, esta função
   atualiza o segundo argumento, um array de 4 posições de inteiros
   (0s ou 1s). */

void genome2features(const int genome[16], int features[4]);


/* A função abaixo sumariza a população atual de wugs, agrupando o
   número de wugs por característica (features, 4 inteiros) e
   sexo. Para esta função, poderá ser especialmente útil converter um
   vetor de caracteristicas (4 int) em um único número. Veja util.c */

int  report_population(const wug_t population[], const int size);


/* dado um wug, esta função devolve seu rank, isto é, o número de
   características que ele tem com valor excepcional (valor 1). */

int  rank(const wug_t i);


/* esta função é responsável por dado um wug, adiciona-lo na
   população. Na implementação sugerida, a população é um array de
   wugs. Esta função deve adicionar wugs no array mantendo-o ordenado
   (ordem inversa de rank), wugs com maior rank na frente. Além disso,
   o limite do array não pode ser ultrapassado, a capacidade da
   população. O retorno é um booleano que indica se o novo wug foi
   ignorado (rank menor do que o wug de rank mais baixo na população
   já no limite de wugs). */

bool insert_ranked(wug_t population[], wug_t e, int *size, int capacity);


/* a função proliferate implementa a proliferação de wugs via
   clonagem. A cada chamada, para cada wug, são criamos 16 novos wugs
   com uma mutação em um gene. Os novos wugs não são considerados na
   mesma chamada da função, sendo adicionados à população de uma única
   fez. Nesta função, novos wugs podem ser colocados em uma lista
   encadeada que será depois percorrida para que os wugs sejam
   inseridos na população (array de wugs) usando a 'insert_ranked'. */

void proliferate(wug_t population[], int *size, int capacity);



/* abaixo a função 'breed' e as funções auxiliares para ela. A 'breed'
   implementa o acasalamento de wugs. A população atual deve ser
   percorrida, do wug com maior rank para o de menor rank. Para cada
   wug (digamos A), um outro wug da população de sexo oposto e melhor
   match com este A deve ser selecionado (digamos B). Uma vez que A e
   B sejam selecionados, criamos os seus descendentes (offsprings). A
   e B não serão considerados para novo acasalamento nesta mesma
   chamada da função 'breed'. */


/* A função 'match' retorna o número de caracteristicas comuns de dois
   wugs de sexo oposto compartilham. */

int  match(const wug_t *a, const wug_t *b);


/* a função find_pair, dado uma lista encadeada de ponteiros para wug,
   retorna ponteiros para um wug male e female, sendo ambos um par com
   melhor match na população. A lista é modificada, para que ambos os
   wugs retornados nos segundo e terceiro parâmetros não estejam mais
   na lista. */

void find_pair(node_t **head, wug_t **male, wug_t **female);


/* dados dois wugs, a função create_offsprings cria novos wugs
   respeitando as regras de acasalamento e mutação, para cada par, 18
   novos wugs são criados. Estes novos wugs não poderão ser
   imediatamente inseridos na população, dado que na mesma chamada da
   função 'breed', create_offsprings será chamada algumas vezes, para
   cada par. Os descendentes dos acasalamentos são adicionados em uma
   lista para serem incluídos na população de uma única vez pela
   função 'breed'. */

void create_offsprings(wug_t fem, wug_t masc, node_t **clones);


/* a função 'breed' implementa a lógica principal das regras de
   acasalamento. Para cada elemento da população ela usará find_pair
   para construir casais e create_offsprings para criação de novos
   wugs. Por fim, quando nenhum novo par puder ser formado, os novos
   wugs devem ser incluídos na população com a insert_ranked e a lista
   de novos wugs liberada da memória. */

void breed(wug_t population[], int *size, int capacity);


