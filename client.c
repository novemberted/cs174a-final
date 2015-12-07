#include "helperMethods.h"

int main(int argc, char **argv) {
	connection();

	char *pub = "815eeb1fa402ba1538c3b1a3dc1a5d75";
	char *prv = "ac7e8ed4daae4d6fb91c5270a2b0e70";

	paillier_pubkey_t *pubkey = paillier_pubkey_from_hex(pub);
	paillier_prvkey_t *prvkey = paillier_prvkey_from_hex(prv, pubkey);

	bool runProgram = true;
	printf("Enter 'exit' to quit.\n");

	do {
		char input[256];
		char *query;

		printf("Enter query/command:\n> ");
		fgets(input, 256, stdin);

		if (strncmp(input, "exit", 4) == 0 || strncmp(command, "EXIT", 4) == 0) {
			runProgram = false;
			printf("\n");
		} 
		else {
		  query = read(input, pubkey, prvkey);
		  printf("%s\n", query);
		  execute(query, pubkey, prvkey);
		}
	} while (runProgram);

	mysql_close(conn);
	return 0;
}
