#include "includes.h"
#include "bank.h"
#include "atm.h"

using namespace std;

#define BANK_THREADS 2
#define DEBUG_MAIN 0
int main (int argc, const char* argv[])
{
    //validating num of atms suitable to num of files given by the user
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
    int numOfAtm = atoi(argv[1]);
    int currentAtm=0;
    
    //removing existing file and open a new one insread.
    remove("log.txt");
    ofstream logFile;
    logFile.open("log.txt");
    logFile.close();

    //initing the Bank parameters and object
    unsigned int bank_account_num = BANK_ACCOUNT_NUM ;
    string bank_pass = "123456"; 
    bank bank(bank_account_num,bank_pass ,0);

    //create threads for each atm, printign loop and commission loop.
    pthread_t atmsThreads[numOfAtm];
	pthread_t bankThreads[2]; //for example - i need dedicated thread for taking commison
    int rc;
    pAtmParams curAtmPar;
    for (currentAtm = 0; currentAtm < numOfAtm; currentAtm++){
        if(DEBUG_MAIN) cout << "Started thread Iteratin: "<<currentAtm<< "\n";
        curAtmPar = new(std::nothrow) atmParams;
        curAtmPar->atmNum = currentAtm+1;
        curAtmPar->assBank = &bank;
        curAtmPar->inputFile = argv[currentAtm+2];
        if(DEBUG_MAIN) printf("**debug argument passing to thread argv[curr] is: %s \n", argv[currentAtm+2]);
        rc = pthread_create(&atmsThreads[currentAtm], NULL, &atm_main_loop, (void*)curAtmPar);
        if (rc){
            cout << "ERROR creating ATM " << currentAtm << " thread";
            exit(-1);
        }
        if(DEBUG_MAIN) cout << "Done ATM Iteration \n";
    }
    rc = pthread_create(&bankThreads[0], NULL, &bank_main_loop, (void*)&bank);
    if(rc){
        cout << "ERROR creating bank";
        exit(-1);
    }
    rc = pthread_create(&bankThreads[1], NULL,&bank_print_loop,(void*)&bank);
    if(rc){
        cout << "ERROR creating bank" << "\n";
        exit(-1);
    }
    //whaiting for all atms threads to be done - execute all action on files
    if(DEBUG_MAIN) cout << "debug- in main after bank threads create" << "\n" ;
    int atmWait;     
    for (atmWait = 0; atmWait < numOfAtm; atmWait++){
        pthread_join(atmsThreads[atmWait],NULL);
        if(DEBUG) cout << "Joined all threads\n";
    }
    // canceling print and commission threads
    pthread_cancel(bankThreads[0]); 
    pthread_cancel(bankThreads[1]);
    return 0; 
}
