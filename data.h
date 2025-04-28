#include <stdbool.h>

typedef enum {
  M = 'M',
  F = 'F'
} gender_t;

typedef struct wug {
  int      genome[16];
  gender_t gender;
} wug_t;


typedef struct node {
  wug_t       *wug;
  struct node *next;
} node_t;

/* dado um endereço de wug, esta função acrescentar este ponteiro na
   lista de ponteiros para wug */
node_t* list_insert(wug_t *e, node_t *head);

/* a função 'list_insert' irá acrescentar elementos sempre na frente
   da lista, esta função poderá ser útil para reverter a
   lista. Imagine que em algum momento precisamos criar uma lista a
   partir da população corrente de wugs. Se para cada elemento da
   população usarmos a 'list_insert' vamos precisar inverter a lista
   para obter a ordem original da população. */
node_t* list_reverse(node_t *head);

/* a lista encadeada sugerida não armazena nos nós uma instância de
   wug, ao invés disso, armazena apenas ponteiros para wug. Logo, ao
   liberar a memória de uma lista, o parâmetro 'deep' informa se além
   de realizar o 'free' no node, também deverá ser feito 'free' no wug
   associado. */
void    list_free(node_t *head, bool deep);


/* função que calcula o tamanho da lista, número de elementos na
   lista. */
int     list_size(node_t *head);
