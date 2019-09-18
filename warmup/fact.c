#include "common.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int
factorial(int n){
	if(n == 1){
		return 1;
	}

	return n*factorial(n-1);

}

bool isNumber(char* word){
	while(*word != '\0'){
		if(*word < 47 || *word > 57){
			return false;
		}
		word++;
	}
	return true;
}

int
main(int argc, char* argv[])
{
	if(argc == 2 && isNumber(argv[1])){
		int n = atoi(argv[1]);
		if(n > 12){
			printf("Overflow\n");
		}
		else if(n == 0){
			printf("Huh?\n");
		}
		else{
			printf("%d\n", factorial(n));
		}
	}
	else{
		printf("Huh?\n");
	}
	
	return 0;
}
