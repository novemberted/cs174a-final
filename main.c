#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>

//#include "cryption.h"

MYSQL *conn;

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
   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
}

char* read(char *input){//,paillier_pubkey_t* pub, paillier_prvkey_t* prv){
	char *tokens[50];
	int i = 0;
	
	int l = strlen(input);
	input[l-1] = "\0";
	char *query = (char*)malloc(512*sizeof(char));
	
	if(strcmp(input, "") == 0) {
		fprintf(stderr, "%s\n", "No query entered.");
		strcpy(query, "Invalid query");
	}
	
	char token  = strtok(input, " ");
	l = strlen(token);
	
	tokens[i] = (char*)malloc(l*sizeof(char));
	strcpy(tokens[i], token);
	i++;
	if(strcmp(token, "INSERT") == 0 || strcmp(token, "insert") == 0) {
		token = strtok(input, " ");
		while(token != NULL) {
			l = strlen(token);
			tokens[i] = (char*)malloc(l*sizeof(char));
			strcpy(tokens[i], token);
			
			i++;
			token = strtok(input, " ");
		}
		if((i-1) != 4) {
			strcpy(query, "Invalid query");
		}
		else {
			//char *enc = encryption(pub, tokens[3]);
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
	else if(strcmp(token, "SELECT") == 0 || strcmp(token, "select") == 0) {
		token = strtok(input, " ");
		if(token != NULL) {
			l = strlen(token);
			tokens[i] = (char*)malloc(l*sizeof(char));
			strcpy(tokens[i], token);
			i++;
		}
		if(strcmp(token, "*") == 0) {
			strcpy(query, "SELECT id, age, salary FROM Employees;");
		}
		else if(strcmp(token, "SUM") == 0 || strcmp(token, "sum") == 0 || strcmp(token, "AVG") == 0 || strcmp(token, "avg") == 0) {
			strcpy(query, "SELECT");
			int wherebool = 0; // bool if there is a where condition
			int groupby = 0; // bool for group by condition
			int having = 0; // bool for having condition
			int where_i;
			int groupby_i;
			int having_i;
			token = strtok(input, " ");
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
					end = i - 1;
				int j;
				for(j = start; j < end; j++) {
					strcat(query, " ");
					strcat(query, "tokens[j]");
				}
			}
			if(groupby)
				strcat(query, "GROUP BY age");
			if(having) {
				strcat(query, " HAVING");
				int start = having_i + 1, end = i - 1;
				int j;
				for(j = start; j < end; j++) {
					strcat(query, " ");
					strcat(query, tokens[j]);
				}
			}
		}
		else if (token != NULL) {
			strcpy(query, "SELECT id, age, salary FROM Employees WHERE id = ");
			strcat(query, token);
		}
		else
			strcpy(query, "Invalid query");
	}
	return query;
}

void execute(char* query){//, paillier_pubkey_t *pub, paillier_prvkey_t *prv){
	if(strcmp(query, "Invalid query")){
		printf("Invalid query. \n");
	}else if (strncmp(query, "SELECT AVG", 10) == 0 || strncmp(query, "SELECT age, AVG", 15) == 0 ){
		MYSQL_RES *sum_res, *count_res;
		char *sum = replace_str(query, "AVG", "SUM_HE");
		if (mysql_query(conn, sum)){
			fprintf(stderr, "MySQL error %s\n", mysql_error(conn)); 
		}
		else {
			sum_res = mysql_store_result(conn);
			
			if(sum_res == NULL){
				fprint(stderr, "MySQL error: there's no employees in this query. \n");
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
		}
	}

}



int main(int argc, char **argv) {
	connection();
	printf("Enter 'exit' to quit\n");
	int run = 1;
	do {
		char input[200];
		char *query;
		
		printf("enter in you dare \n");
		fgets(input, 200, stdin);
		
		query = read(input);
		printf("%s\n", query);
		execute(query);
		
	}while (run);
	close_connection();
	return 0;
	
}