#include "functions.h"

int main(int argc, char **argv) {
	connection();
	
	//char *pub = "815eeb1fa402ba1538c3b1a3dc1a5d75";
	//char *prv = "ac7e8ed4daae4d6fb91c5270a2b0e70";

	//paillier_pubkey_t *pubkey = paillier_pubkey_from_hex(pub);
	//paillier_prvkey_t *prvkey = paillier_prvkey_from_hex(prv, pub);
	
	/*	
	printf("Enter 'exit' to quit\n");
	int runProgram = 1;
	do {
		char input[256];
		char *query;
		printf("enter in you dare \n");
		//fgets(input, 256, stdin);
		//printf("66666");
		if(strcmp(input, "exit") == 0){
			runProgram = 0;
			printf("\n");
		}else {
			query = read("INSERT 11 22 33");
			printf("%s\n", query);
			execute(query);
		}
	} while (runProgram);
	*/
	/*
	char *query;
	query = read("INSERT 11 22 33");
	printf("%s\n", query);
	execute(query);
	mysql_close(conn);
	return 0;
	*/

	char *pub = "815eeb1fa402ba1538c3b1a3dc1a5d75";
	char *prv = "ac7e8ed4daae4d6fb91c5270a2b0e70";

	paillier_pubkey_t *pubkey = paillier_pubkey_from_hex(pub);
	paillier_prvkey_t *prvkey = paillier_prvkey_from_hex(prv, pubkey);

	bool blnContinue = true;
	printf("Enter 'exit' or 'quit' to exit.\n\n");

	do {
		char command[256];
		char *query;

		printf("Enter query/command:\n> ");
		fgets(command, 256, stdin);

		if (strncmp(command, "exit", 4) == 0 || strncmp(command, "quit", 4) == 0) {
			blnContinue = false;
			printf("\n");
		} else {
            query = read(command, pubkey, prvkey);
			printf("%s\n\n", query);
            execute(query, pubkey, prvkey);
		}
	} while (blnContinue);

	mysql_close(conn);
	return 0;
}
