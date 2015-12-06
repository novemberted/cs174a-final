#include "functions.h"

int get_int_length(unsigned long int value) {
	return (value == 0 ? 1 : ((int)(log10(fabs(value)) + 1) + (value < 0 ? 1 : 0)));
}

char *replace_str(char *string, char *original, char *replacement) {
	static char buffer[4096];
	char *p;

	if (!(p = strstr(string, original))) {
		return string;
	}

	strncpy(buffer, string, p - string);
	buffer[p - string] = '\0';

	sprintf(buffer + (p - string), "%s%s", replacement, p + strlen(original));

	return buffer;
}


char *encrytion(paillier_pubkey_t* pub, char *input){
	unsigned long int u_input = strtoul(input, NULL, 10);
	paillier_plaintext_t* plain = paillier_plaintext_from_ui(u_input);
	paillier_ciphertext_t* cipher = paillier_enc(cipher, pub, plain, paillier_get_rand_devurandom);
	char* output = mpz_get_str(NULL, 16, cipher->c);
	
	return output;
	
}

char *decryption(paillier_pubkey_t* pub, paillier_prvkey_t* prv, char *input){
	paillier_ciphertext_t* cipher = (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
	
	mpz_init_set_str(cipher->c, input, 16);
	paillier_plaintext_t* plain = paillier_dec(plain, pub, prv, cipher);
	unsigned long int u_plain = mpz_get_ui(plain->m);
	
	int length = get_int_length(u_plain);
	char* output = (char *)malloc(length * sizeof(char));
	sprintf(output, "%lu", u_plain);
	
	return output;
}

void connection() {
   //MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *server = "localhost";
   char *user = "root";
   char *password = "";//"cs174$"; /* set me first */
   char *database = "project";
   conn = mysql_init(NULL);
   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }
   if(mysql_query(conn, "use project")) {
	  fprintf(stderr, "%s\n", mysql_error(conn));
   }

   /* send SQL query */
   if (mysql_query(conn, "show tables")) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }
   res = mysql_use_result(conn);
   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);
   mysql_free_result(res);
	printf("database connect success");
}

char *read(char *input){// ,paillier_pubkey_t* pub, paillier_prvkey_t* prv){
	char *tokens[32];
	int i = 0;
	char* token = (char*)malloc(20*sizeof(char));
	int l = strlen(input);
	input[l-1] = '\0';
	char *query = (char*)malloc(512*sizeof(char));
	if(strcmp(input, "") == 0) {
		fprintf(stderr, "%s\n", "No query entered.");
		strcpy(query, "Invalid query");
	}else{
	token = strtok(input, " ");
	l = strlen(token);
	
	
	tokens[i] = (char*)malloc(l*sizeof(char));
	strcpy(tokens[i], token);
	i++;
	if(strcmp(tokens[0], "INSERT") == 0 || strcmp(tokens[0], "insert") == 0) {
		printf("4444");
		token = strtok(NULL, " ");
		while(token != NULL) {
			l = strlen(token);
			tokens[i] = (char*)malloc(l*sizeof(char));
			strcpy(tokens[i], token);
			
			i++;
			token = strtok(NULL, " ");
		}
		if(i!= 4) {
			strcpy(query, "Invalid query");
		}
		else {
			//char *enc = encryption(pub, tokens[3]);
			printf("6666\n");
			strcpy(query, "INSERT INTO Employees VALUES(");
			strcat(query, tokens[1]); // insert id
			strcat(query, ", ");
			strcat(query, tokens[2]); // insert age
			strcat(query, ", '");
			//strcat(query, enc); // insert encrypted salary
			strcat(query, tokens[3]);
			strcat(query, "');");
		}
	}
	else if(strcmp(tokens[0], "SELECT") == 0 || strcmp(tokens[0], "select") == 0) {
		token = strtok(NULL, " ");
		if(token != NULL) {
			l = strlen(token);
			tokens[i] = (char*)malloc(l*sizeof(char));
			strcpy(tokens[i], token);
			i++;
		}
		if(strcmp(tokens[1], "*") == 0) {
			strcpy(query, "SELECT id, age, salary FROM Employees;");
		}
		else if(strcmp(tokens[1], "SUM") == 0 || strcmp(tokens[1], "sum") == 0 || strcmp(tokens[1], "AVG") == 0 || strcmp(tokens[1], "avg") == 0) {
			strcpy(query, "SELECT");
			int wherebool = 0; // bool if there is a where condition
			int groupby = 0; // bool for group by condition
			int having = 0; // bool for having condition
			int where_i;
			int groupby_i;
			int having_i;
			token = strtok(NULL, " ");
			while(token != NULL) {
				l = strlen(token);
				tokens[i] = (char*)malloc(l*sizeof(char));
				strcpy(tokens[i], token);
				
				if(strcmp(tokens[i], "WHERE") == 0 || strcmp(tokens[i], "where") == 0) {
					wherebool = 1;
					where_i = i;
				}
				if(strcmp(tokens[i], "GROUP") == 0 || strcmp(tokens[i], "group") == 0) {
					groupby = 1;
					groupby_i = i;
				}
				if(strcmp(tokens[i], "HAVING") == 0 || strcmp(tokens[i], "having") == 0) {
					having = 1;
					having_i = i;
				}				
				i++;
				token = strtok(NULL, " ");
			}
			if(groupby)
				strcat(query, " age,");
			if(strcmp(tokens[1], "SUM") == 0 || strcmp(tokens[1], "sum") == 0) {
				strcat(query, " SUM_HE(salary) FROM Employees");
			}
			else if(strcmp(tokens[1], "AVG") == 0 || strcmp(tokens[1], "avg") == 0) {
				strcat(query, " AVG(salary) FROM Employees");
			}
			if(wherebool) {
				strcat(query, " WHERE");
				int start = where_i + 1, end;
				if(groupby)
					end = groupby_i;
				else
					end = i;
				int j;
				for(j = start; j < end; j++) {
					strcat(query, " ");
					strcat(query, tokens[j]);
				}
			}
			if(groupby)
				strcat(query, " GROUP BY age");
			if(having) {
				strcat(query, " HAVING");
				int start = having_i + 1, end = i;
				int j;
				for(j = start; j < end; j++) {
					strcat(query, " ");
					strcat(query, tokens[j]);
				}
			}
		}
		else if (tokens[1] != NULL) {
			strcpy(query, "SELECT id, age, salary FROM Employees WHERE id = ");
			strcat(query, token);
		}
		else
			strcpy(query, "Invalid query");
	}
	}
	return query;
}

void execute(char* query){//, paillier_pubkey_t *pub, paillier_prvkey_t *prv){
	if(strcmp(query, "Invalid query") == 0){
		printf("Invalid query. \n");
	}
	else if (strncmp(query, "SELECT AVG", 10) == 0 || strncmp(query, "SELECT age, AVG", 15) == 0 ){
		MYSQL_RES *sum_res, *count_res;
		char *sum = replace_str(query, "AVG", "SUM_HE");
		if (mysql_query(conn, sum)){
			fprintf(stderr, "MySQL error %s\n", mysql_error(conn)); 
		}
		else {
			sum_res = mysql_store_result(conn);
			
			if(sum_res == NULL){
				fprintf(stderr, "MySQL error: there's no employees in this query. \n");
				return;
			}
		}
		char *count = replace_str(sum, "SUM_ME(salary)", "COUNT(*)");
		if(mysql_query(conn, count)){
			fprintf(stderr, "MySQL error %s\n", mysql_error(conn)); 
		} 
		else{
			count_res = mysql_store_result(conn);
		
			if(count_res == NULL){
				fprintf(stderr, "MySQL error: there's no employees in this query. \n");
				return;
			}
		}
		
		int sum_fields = mysql_num_fields(sum_res);
		int count_fields = mysql_num_fields(count_res);
		if(sum_fields == count_fields) {
			int j;
			MYSQL_ROW sum_rows, count_rows;
			MYSQL_FIELD *fields;
			for(j = 0; j < sum_fields; j++) {
				if(j == sum_fields - 1) 
					printf("%10s", "average");
				else {
					printf("%10s", fields[j].name);
				}
			}
			printf("\n");
			while((sum_rows = mysql_fetch_row(sum_res)) && (count_rows = mysql_fetch_row(count_res))) {
				for(j = 0; j < sum_fields; j++) {
					if(j == sum_fields - 1) {
						//double sum = atof(decryption(pub, prv, sum_rows[j]));
						//double count = atof(count_rows[j]);
						double average = 0;//sum / count;
						printf("%10.2f", average);
					}
					else {
						printf("%10s", sum_rows[j]);
					}
				}
				printf("\n");
			}
			printf("\n");
			mysql_free_result(sum_res);
			mysql_free_result(count_res);
		}
	
	}
	else {
		MYSQL_RES *res;
		if(mysql_query(conn, query)) {
			fprintf(stderr, "MySQL error %s\n", mysql_error(conn));
		}
		else {
			res = mysql_store_result(conn);
			
			if(res == NULL) {
				fprintf(stderr, "MySQL error: There are no employees in this query.\n");
			}
			else {
				int res_fields = mysql_num_fields(res);
				MYSQL_ROW r;
				MYSQL_FIELD *f;
				int j;
				f = mysql_fetch_fields(res);
				for(j = 0; j < res_fields; j++) {
					if(strcmp(f[j].name, "SUM_HE(salary)") == 0)
						printf("%10s", "sum");
					else
						printf("%10s", f[j].name);
				}
				printf("\n");
				while((r = mysql_fetch_row(res))) {
					for(j = 0; j < res_fields; j++) {
						if(j == res_fields - 1) {
							//char *sum = decryption(pub, prv, r[j]);
							int sum = 0;
							printf("%10d", sum);
						}
						else
							printf("%10s", r[j]);
					}
					printf("\n");
				}
				printf("\n");
				mysql_free_result(res);
			}
		}
	}
}


