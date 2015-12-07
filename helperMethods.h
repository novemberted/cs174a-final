#ifndef HELPERMETHODS_H
#define HELPERMETHODS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <mysql.h>
#include <my_global.h>
#include <gmp.h>

#include "paillier.h"

MYSQL *conn;

char *encryption(paillier_pubkey_t* pub, char *input);
char *decryption(paillier_pubkey_t* pub, paillier_prvkey_t* prv, char *input);
char *changeStr(char *s, char *orig, char *newStr);
void connection();
char *read(char *input, paillier_pubkey_t* pub, paillier_prvkey_t* prv);
void execute(char* query, paillier_pubkey_t *pub, paillier_prvkey_t *prv);

#endif
