#include "bank.h"
#include <stdio.h>
#include <string.h>


#define BANK_THREADS 2

int main (int argc, char const* argv[])
{
    if (argv == NULL){
        cout << "Illegal Arguments";
        return -1;
    }
    if (argc-1 !=  atoi(*argv[0])){
        cout <<"Illegal Arguments";
        return -1;
    }

	pthread_t atmsThreads[numOfATMs];
	pthread_t bankThreads[2]; //for example - i need dedicated thread for taking commison 

    bank bank(atm_number,*atmsThreads, *bankThreads); 
    
    int numOfAtm = atoi(argv[0]);
    int currentAtm;
    for (currentAtm = 0; currentAtm < numOfAtm; currentAtm++){
        rc = pthread_create(&atms_threads[currentAtm], NULL, atm_main_loop, argv[currentAtm]);
        if (rc){
            cout << "ERROR creating ATM " << currentAtm << " thread";
            exit(-1);
        }
    }
    rc = pthread_create(&bankThreads[0], NULL, bank_main_loop, bank);
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }
    rc = pthread_create(&bankThreads[0], NULL, bank_print_loop,bank);
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }

    int atmWait;     
    for (atmWait = 0; atmWait < numOfAtm+2; atmWait++)
       pthread_join(atmThreads[atmWait],NULL); 
    pthread_join(bankThreads[0],NULL); 
    pthread_exit(bankThreads[0]);
    return 0; 
}
