#include "includes.h"
#include "atm.h"

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
    //vector<atm>::iterator it = atms_vector->begin();

    pthread_t atmsThreads[numOfAtm];
	pthread_t bankThreads[2]; //for example - i need dedicated thread for taking commison

    //create atms vector -- moved out from the bank due to cycling dependence
    for(int i=0 ; i<numOfAtm ; i++) //i need to use iterators ?
	{

		atm new_atm = atm(&bank ,i);
		atms_vector->push_back(new_atm);
	}

    int rc;
    pAtmParams curAtmPar;
    for (currentAtm = 0; currentAtm < numOfAtm; currentAtm++){
        curAtmPar = new(std::nothrow) atmParams;
        curAtmPar->atmNum = currentAtm;
        curAtmPar->assBank = &bank;
        curAtmPar->inputFile = argv[currentAtm];
        pthread_create(&atmsThreads[currentAtm], NULL, &atm_main_loop, (void*)curAtmPar);
        if (atmsThreads[currentAtm]){
            cout << "ERROR creating ATM " << currentAtm << " thread";
            exit(-1);
        }
    }
    rc = pthread_create(&bankThreads[0], NULL, &bank_main_loop, (void*)&bank);
    if(bankThreads[0]){
        cout << "ERROR creating bank";
        exit(-1);
    }
    rc = pthread_create(&bankThreads[0], NULL,&bank_print_loop,(void*)&bank);
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }

    int atmWait;     
    for (atmWait = 0; atmWait < numOfAtm+2; atmWait++)
      pthread_join(atmsThreads[atmWait],NULL);
    pthread_join(bankThreads[0],NULL); 
    pthread_cancel(bankThreads[1]);
    return 0; 
}
