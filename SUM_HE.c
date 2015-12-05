#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;     /* Microsofts 64 bit types */
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#endif
#include <mysql.h>
#include <ctype.h>
static pthread_mutex_t LOCK_hostname;

#include "paillier.c"


extern "C" my_bool SUM_HE_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
	longlong* i = new longlong; // create the variable
	*i = 0; 
	initid->ptr = (char*)i;
	        
	// check the arguments format
	if (args->arg_count != 1)
	{
	     strcpy(message,"SUM_HE() requires one arguments");
	     return 1;
	}

	 if (args->arg_type[0] != INT_RESULT)
	 {
	     strcpy(message,"SUM_HE() requires an integer");
	     return 1;
	  }       
	return 0;
}

extern "C" void SUM_HE_deinit(){
	delete (longlong*)initid->ptr;
	
}

void SUM_HE_clear(){
	 *((longlong*)initid->ptr) = 0;
	
}

void SUM_HE_add(){
	paillier_mul( e3920cb31b02300ecf5e081bf1c647a3, *((longlong*)initid->ptr), *((longlong*)initid->ptr), *((longlong*)args->args[0]) );
	
}

longlong SUM_HE(){
	 return *((longlong*)initid->ptr);
	
}