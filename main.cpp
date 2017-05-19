//main.c
#include<bank.h>

int main ()
{
	//todo: handle all the input files and params
	
	pthread_t atms_threads[numOfATMs];
	pthread_t bank_operations[2]; //for example - i need dedicated thread for taking commison 
	bank bank(int atm_number, pthread_t *atms_threads,pthread_t *bank_operations) 
	
	//todo: the threads should be in the main , or could
}