#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#include "paillier.h"


my_bool SUM_HE_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
    paillier_ciphertext_t* i = paillier_create_enc_zero(); // create a cipertext variable that's equal 0
    initid->ptr = (char*)i; 
    
    // check the arguments format
    if (args->arg_count != 1)
    {
        strcpy(message,"SUM_HE() requires one arguments");
        return 1;
    }
    
    if (args->arg_type[0] != STRING_RESULT)
    {
        strcpy(message,"SUM_HE() requires a string");
        return 1;
    }
    return 0;
}

void SUM_HE_deinit(UDF_INIT *initid){
    free((paillier_ciphertext_t* )initid->ptr);
    
}

void SUM_HE_clear(UDF_INIT *initid, char *is_null, char *error){
	paillier_ciphertext_t* temp = (paillier_ciphertext_t*)initid->ptr;
	mpz_init_set_ui(temp->c, 1);
    
}

void SUM_HE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error){
   	char *pub = "";
	paillier_pubkey_t* pubkey = paillier_pubkey_from_hex(pub);
	paillier_ciphertext_t* temp1 = (paillier_ciphertext_t*)initid->ptr;
	
	char *argument = (char *)args->args[0];
	paillier_ciphertext_t* temp2 = (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
	mpz_init_set_str(temp2->c, argument, 16);
	
	paillier_mul(pubkey, temp1, temp1, temp2);
	paillier_freepubkey(pubkey);
	paillier_freeciphertext(temp2);
}

char *SUM_HE(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error){
	paillier_ciphertext_t* sum = (paillier_ciphertext_t*)initid->ptr;
	char *sum_string = mpz_get_str(NULL, 16, sum->c);
	
	return sum_string;
    
    
}