#include "includes.h"
#include "atm.h"

#define BANK_THREADS 2

int main (int argc, const char* argv[])
{
    cout <<"Welcome to Ahiads' & Roeis' Bank\n";
    if(argc == 1){
        cout << "No Argument sent to the Bank\n";
        return -1;
    }
    if (argv[0] == NULL){
        cout << "Illegal Arguments";
        return -1;
    }
    if (argc-2 !=  atoi(argv[1])){
        cout <<"Illegal Arguments : argc: "<<argc<<"argv[0]"<<atoi(argv[0]) ; 
        return -1;
    }
    //todo: init this values; !!!
    int numOfAtm = atoi(argv[1]);
    int currentAtm=0;

    unsigned int bank_account_num = BANLK_ACCOUNT_NUM ;

    //account bank_account_ = account(bank_account_num,bank_pass ,0) ;
    string bank_pass = "123456"; 
    bank bank(bank_account_num,bank_pass ,0);
    vector<atm> atms_vector[numOfAtm];
    //vector<atm>::iterator it = atms_vector->begin();
    cout << "numOfAtm: " << numOfAtm<< "\n";
    pthread_t atmsThreads[numOfAtm];
	pthread_t bankThreads[2]; //for example - i need dedicated thread for taking commison
    cout << "created pthreads structs"<< "\n";
    //create atms vector -- moved out from the bank due to cycling dependence
    /*
    for(int i=0 ; i<numOfAtm ; i++) //i need to use iterators ?
	{
        cout << "inside loop itr: " << i<< "\n";
		atm new_atm = atm(&bank ,i);
		atms_vector->push_back(new_atm);
        cout << "done Iteration\n";
	}

    int rc;
    pAtmParams curAtmPar;
    for (currentAtm = 0; currentAtm < numOfAtm; currentAtm++){
        cout << "Started thread Iteratin: "<<currentAtm<< "\n";
        curAtmPar = new(std::nothrow) atmParams;
        curAtmPar->atmNum = currentAtm;
        curAtmPar->assBank = &bank;
        curAtmPar->inputFile = argv[currentAtm];
        pthread_create(&atmsThreads[currentAtm], NULL, &atm_main_loop, (void*)curAtmPar);
        if (atmsThreads[currentAtm]){
            cout << "ERROR creating ATM " << currentAtm << " thread";
            exit(-1);
        }
        cout << "Done ATM Iteration \n";
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
    pthread_cancel(bankThreads[1]);*/
    return 0; 
}
