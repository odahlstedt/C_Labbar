#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // för att boolean ska funka
#include <ctype.h>

#define N (10)

int main(void)
{
	
	//int stack[N];
	int i;
	int c; // number we read
	//int d;
	int x; // value of number
	bool num = true; // reading a number
	bool err; //found error on line
	unsigned line =0; // linenumber
	printf("hej \n");

	while((c = getchar()) != EOF)  { // läser ett tecken åt taget, EOF = end of file
	printf("hej \n");
		if(err){
			printf("hej");
			printf(" du där");
			printf("%d", c);
			if(c == '\n'){
				line += 1;
				err = 0;
				i = 0;
			}
			continue; // tvingar nästa iteration att påbörjas
		}

		else if(isdigit(c)){
			x = x * 10 + c -'0';
		}
		else if(num){
			if(i == num){

			}
		}

	}


}
