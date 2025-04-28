
/* para imprimir um wug, é conveniente podermos ter uma função que
   transformar o array de int, genoma, em uma string. */

char* array_string(const int arr[], const int size);


/* as funções a seguir são convenientes para transformar um array de
   inteiros (contendo 1 ou 0), as features, em um número inteiro
   armazenado em 8 bits. Uma função é o inverso da outra. */

uint8_t features2bits(const int feats[4]);
void    bits2features(const uint8_t value, int features[4]);


