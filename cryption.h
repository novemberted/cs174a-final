#ifndef CRYPTION_H
#define CRYPTION_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#include "paillier.h"

char *encrytion(paillier_pubkey_t* pub, char *input);
char *decryption(paillier_pubkey_t* pub, paillier_prvkey_t* prv, char *input);

#endif