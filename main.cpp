#include <bank.h>
#include <stdio.h>
#include <string.h>

#define BANK_THREADS 2

int main (int argc, char const argv[])
{
    if (argv == NULL){
        cout << "Illegal Arguments";
        return;
    }
    if (argc-1 !=  atoi(argv[0])){
        cout <<"Illegal Arguments";
        return;
    }

	pthread_t atmsThreads[numOfATMs];
	pthread_t bankThreads[2]; //for example - i need dedicated thread for taking commison 

    bank bank(atm_number, *atms_threads, *bank_operations) 

    int numOfAtm = atoi(argv[0]);
    int currentAtm;
    for (currentAtm = 0; currentAtm < numOfAtm; currentAtm++){
        rc = pthread_create(&atms_threads[currentAtm], NULL, atm_main_loop, argv[currentAtm]);
        if (rc){
            cout << "ERROR creating ATM " << currentAtm << " thread";
            exit(-1);
        }
    }
    rc = pthread_create(&bankThreads[0], NULL, /*what is the function*/, /*what are the arguments?*/ );
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }
    rc = pthread_create(&bankThreads[0], NULL, /*what is the finction*/, /*what are the arguments?*/ );
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }

    int atmWait, bankWait ;     
    for (atmWait = 0; atmWait < numOfAtm+2; atmWait++)
       pthread_join(atmThreads[atmWait],NULL); 
    for (bankWait = 0; bankWait < numOfAtm+2; bankWait++)
       pthread_join(bankThreads[bankWait],NULL); 
    return 0; 
}
