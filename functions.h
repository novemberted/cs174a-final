#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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

int get_int_length(unsigned long int value);
char *encrytion(paillier_pubkey_t* pub, char *input);
char *decryption(paillier_pubkey_t* pub, paillier_prvkey_t* prv, char *input);
char *replace_str(char *string, char *original, char *replacement);
void connection();
char *read(char *input);// ,paillier_pubkey_t* pub, paillier_prvkey_t* prv);
void execute(char* query);//, paillier_pubkey_t *pub, paillier_prvkey_t *prv);

#endif