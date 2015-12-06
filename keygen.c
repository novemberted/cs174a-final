#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "paillier.h"

int main (){
  paillier_pubkey_t* pub;
  paillier_prvkey_t* prv;
  
  paillier_keygen( 128, &pub, &prv, paillier_get_rand_devurandom);
  
  printf("%s, %s, \n", paillier_pubkey_to_hex(pub), paillier_prvkey_to_hex(prv));
  
  
}
