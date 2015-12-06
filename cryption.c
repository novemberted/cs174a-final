#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#include "cryption.h"

char *encrytion(paillier_pubkey_t* pub, char *input){
	unsigned long int u_input = stroul(input, NULL, 10);
	paillier_plaintext_t* plain = paillier_plaintext_from_ui(u_input);
	paillier_ciphertext_t* cipher = paillier_enc(cipher, pub, plain, paillier_get_rand_devurandom);
	char* output = mpz_get_str(NULL, 16, cipher->c);
	
	return output;
	
}

char *decryption(paillier_pubkey_t* pub, paillier_prvkey_t* prv, char *input){
	paillier_ciphertext_t* cipher = (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
	
	mpz_init_set_str_(cipher->c, input, 16);
	paillier_plaintext_t* plain = paillier_dec(plain, pub, prv, cipher);
	unsigned long int u_plain = mpz_get_ui(plain->m);
	
	int length = get_int_length(u_plain);
	char* output = (char *)malloc(length * sizeof(char));
	sprintf(output, "%lu", u_plain);
	
	return output;
}