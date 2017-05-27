
#include "includes.h"


#define BANK_THREADS 2

int main (int argc, const char* argv[])
{
    if (argv == NULL){
        cout << "Illegal Arguments";
        return -1;
    }
    if (argc-1 !=  atoi(argv[0])){
        cout <<"Illegal Arguments";
        return -1;
    }
    //todo: init this values; !!!
    int numOfAtm = atoi(argv[0]);
    int currentAtm=0;

    string bank_pass = "959595320" ;
    unsigned int bank_account_num = BANLK_ACCOUNT_NUM ;

    //account bank_account_ = account(bank_account_num,bank_pass ,0) ;

    bank bank(bank_account_num,bank_pass ,0);
    vector<atm> atms_vector[numOfAtm];

    pthread_t atmsThreads[numOfAtm];
	pthread_t bankThreads[2]; //for example - i need dedicated thread for taking commison

    //create atms vector -- moved out from the bank due to cycling dependence
    for(int i=0 ; i<numOfAtm ; i++) //i need to use iterators ?
	{
		atm new_atm = atm(*bank ,i);
		atms_vector[i] = new_atm ;
	}




    int rc;

    for (currentAtm = 0; currentAtm < numOfAtm; currentAtm++){
        int pthread_create(&atmsThreads[currentAtm], NULL, &atm_main_loop, (void*)argv[currentAtm]);
        if (atmsThreads[currentAtm]){
            cout << "ERROR creating ATM " << currentAtm << " thread";
            exit(-1);
        }
    }
    int pthread_create(&bankThreads[0], NULL, &bank::take_commision, bank);
    if(bankThreads[0]){
        cout << "ERROR creating bank";
        exit(-1);
    }
    rc = pthread_create(&bankThreads[0], NULL, &bank::print_status,bank);
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }

    int atmWait;     
    for (atmWait = 0; atmWait < numOfAtm+2; atmWait++)
      int pthread_join(atmsThreads[atmWait],NULL);
    pthread_join(bankThreads[0],NULL); 

    return 0; 
}
