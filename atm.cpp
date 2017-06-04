#include "includes.h"
#include "bank.h"
#include "atm.h"

//******************************************************************
//print log - prints the relecant log line into the log file.
//it gets the operation code and relevant params and print the suitable
//log message. for debug we added a mechanism that allows us to print the messages to
//the screen instead of into a file.
//*****************************************************************
void printLog(int msg_code, actionParams_t* params){
    std::ofstream logFile;
    logFile.open("log.txt",std::ios::app|std::ios::out);
    switch(msg_code){
        case OPEN_MSG:
            if (SCREEN_PRINT) cout << params->atmNum <<": New aacount id is "<< params->accountNum <<" with password "<< params->password << " and initial balance "<<params->balance <<"\n";
            else logFile << params->atmNum <<": New aacount id is "<< params->accountNum <<" with password "<< params->password << " and initial balance "<<params->balance <<"\n";
            break;
        case DEPOSIT_MSG:
            if (SCREEN_PRINT) cout << params->atmNum <<": Account "<< params->accountNum <<" new balance is "<< params->balance << " after "<<params->amount << " $ was deposited\n";
            else logFile << params->atmNum <<": Account "<< params->accountNum <<" new balance is "<< params->balance << " after "<<params->amount << " $ was deposited\n";
            break;
        case WITHDRAW_MSG:
            if (SCREEN_PRINT) cout << params->atmNum <<": Account "<< params->accountNum <<" new balance is "<< params->balance << " after "<<params->amount << " $ was withdrew\n";
            else logFile << params->atmNum <<": Account "<< params->accountNum <<" new balance is "<< params->balance << " after "<<params->amount << " $ was withdrew\n";
            break;
        case BALANCE_MSG:  
            if (SCREEN_PRINT) cout << params->atmNum <<": Account "<< params->accountNum <<" balance is "<< params->balance << "\n";
            else logFile << params->atmNum <<": Account "<< params->accountNum <<" balance is "<< params->balance << "\n";
            break;
        case CLOSE_MSG: 
            if (SCREEN_PRINT) cout << params->atmNum <<": Account "<< params->accountNum <<"is now closed. Balance was " << params->balance <<"\n";
            else logFile << params->atmNum <<": Account "<< params->accountNum <<"is now closed. Balance was " << params->balance <<"\n";
            break;
        case TRANSFER_MSG: 
            if (SCREEN_PRINT) cout << params->atmNum <<": Transfer "<< params->tranAmount <<"from account "<< params->accountNum <<" to account "<< params->targetAccount <<". new account balance is " <<params->balance <<" new target account balance "<< params->dstBalance << "\n";
            else logFile << params->atmNum <<": Transfer "<< params->tranAmount <<"from account "<< params->accountNum <<" to account "<< params->targetAccount <<". new account balance is " <<params->balance <<" new target account balance "<< params->dstBalance << "\n"; 
            break;
        default: break;
    }
    logFile.close();
}

//******************************************************************
//print error- gets a error code and params and print the
//relevant error msg to the file\screec(for debug)
//*****************************************************************
void printError(int error_code, actionParams_t* params){
    std::ofstream logFile;
    logFile.open("log.txt",std::ios::app|std::ios::out);
    switch(error_code){
        case WRONG_PASSWORD:
            if (SCREEN_PRINT) cout <<"Error " << params->atmNum <<": Your transaction failed – password for account "<< params->accountNum << " is incorrect\n";
            else logFile << "Error " << params->atmNum <<": Your transaction failed – password for account "<< params->accountNum << " is incorrect\n";
            break;
        case ACCOUNT_NOT_EXIST:
            if (SCREEN_PRINT) cout << "Error "<< params->atmNum <<": Your transaction failed – account id "<< params->accountNum << " does not exists\n";        
            else logFile << "Error "<< params->atmNum <<": Your transaction failed – account id "<< params->accountNum << " does not exists\n";        
            break;
        case AMOUNT_ILLEGAL:
            if (SCREEN_PRINT) cout << "Error "<< params->atmNum <<": Your transaction failed – account id " << params->accountNum << " balance is lower than "<<params->amount <<"\n";
            else logFile << "Error "<< params->atmNum <<": Your transaction failed – account id " << params->accountNum << " balance is lower than "<<params->amount <<"\n";
            break;
        case ACCOUNT_ALRDY_EXIST:
            if (SCREEN_PRINT) cout << "Error "<< params->atmNum <<": Your transaction failed – account with the same id exists\n";
            else logFile << "Error "<< params->atmNum <<": Your transaction failed – account with the same id exists\n";
            break;
        default: break;
    }
    logFile.close();
}

//*******************************************************************************************************//
//constructor of atm
atm::atm (pBank associated_bank ,int id_num)
{
	associated_bank_ = associated_bank;
	id_num_ = id_num;
	pthread_mutex_init(&atm_mutex_ , NULL) ;
}

//*******************************************************************************************************//
//debug function that prints the semaphore value
void printSemStatus (pBank curBank, string phase){
    	if(!DEBUG) return;
        int semVal;
        int readSemVal;
        sem_getvalue(&curBank->bank_write, &semVal);
        sem_getvalue(&curBank->bank_read, &readSemVal);
        cout << phase <<" write sem : " << semVal << "read sem val: "<< readSemVal <<"read count: " << curBank->reader_count<< "\n";
}

//*******************************************************************************************************//
//open account method. lock the entire bank and calls bank open account method and prints suitable log
void atm::atm_open_account (actionParams_t* params)
{   
    printSemStatus(associated_bank_, "open account before lock");
    sem_wait(&associated_bank_->bank_write);
    pthread_mutex_lock(&atm_mutex_);
    
    int retVal = associated_bank_->openAccount(params);
    if (GOOD_OP == retVal) printLog(OPEN_MSG, params);
    else printError(retVal,params);
    
    pthread_mutex_unlock(&atm_mutex_);
    sem_post(&associated_bank_->bank_write);
    printSemStatus(associated_bank_, "open account after after release");
}

//*******************************************************************************************************/
//atm deposit method. mark itself as reader, and calls bank deposit method and prints suitable log
void atm::atm_deposit (actionParams_t* params)
{
    printSemStatus(associated_bank_, "deposit before 1st wait for read");
	sem_wait(&associated_bank_->bank_read);
	associated_bank_-> reader_count ++;
	if (associated_bank_->reader_count == 1) {
		sem_wait(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);
    pthread_mutex_lock(&atm_mutex_);
    
    int retVal = associated_bank_->deposit_bank(params);
    if (GOOD_OP == retVal) printLog(DEPOSIT_MSG,params);
    else printError(retVal, params);
    
    pthread_mutex_unlock(&atm_mutex_);
	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count--;
	if (associated_bank_->reader_count == 0) {
		sem_post(&associated_bank_->bank_write);
	}
	printSemStatus(associated_bank_, "deposit before 2nd post");	
	sem_post(&associated_bank_->bank_read);
	printSemStatus(associated_bank_, "deposit after 2nd post");
}

//*******************************************************************************************************/
//atm withdraw method. mark itself as reader, and calls bank withdraw method and prints suitable log
void atm::atm_withdraw (actionParams_t* params)
{
	printSemStatus(associated_bank_, "withdraw before 1st wait for read");
	sem_wait(&associated_bank_->bank_read);
	associated_bank_-> reader_count ++;
  	printSemStatus(associated_bank_, "withdraw after 1st reader advance");
	if (associated_bank_->reader_count == 1) {
		sem_wait(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);
    pthread_mutex_lock(&atm_mutex_);
    
    int retVal = associated_bank_->withdraw_bank(params);
    if (GOOD_OP == retVal) printLog(WITHDRAW_MSG,params);
    else printError(retVal, params);
    
    pthread_mutex_unlock(&atm_mutex_);
	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count--;
	if (associated_bank_->reader_count == 0) {
		sem_post(&associated_bank_->bank_write);
	}
	printSemStatus(associated_bank_, "withdraw before 2nd post");	
	sem_post(&associated_bank_->bank_read);
	printSemStatus(associated_bank_, "withdraw after 2nd post");
}

//*******************************************************************************************************//
//atm get_balance method. mark itself as Bank reader, and calls bank get_balance method and prints suitable log
void atm::atm_get_balance (actionParams_t* params)
{	
	printSemStatus(associated_bank_, "get_balance before 1st wait for read");
	sem_wait(&associated_bank_->bank_read);
	associated_bank_-> reader_count ++;
   	printSemStatus(associated_bank_, "get_balance after 1st reader advance");
	if (associated_bank_->reader_count == 1) {
		sem_wait(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);
    pthread_mutex_lock(&atm_mutex_);
    
    int retVal = associated_bank_->get_balance_bank(params);
    if (GOOD_OP == retVal) printLog(BALANCE_MSG,params);
    else printError(retVal, params);
    
    pthread_mutex_unlock(&atm_mutex_);
	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count--;
	if (associated_bank_->reader_count == 0) {
		sem_post(&associated_bank_->bank_write);
	}
	printSemStatus(associated_bank_, "get_balance before 2nd post");	
	sem_post(&associated_bank_->bank_read);
	printSemStatus(associated_bank_, "get_balance after 2nd post");
}
//*******************************************************************************************************//
//atm close method. mark itself as Bank writer, and calls bank close_account method and prints suitable log
void atm::atm_close_account (actionParams_t* params)
{
    printSemStatus(associated_bank_, "close_account before lock");	
    sem_wait(&associated_bank_->bank_write);
    printSemStatus(associated_bank_, "close_account after lock");
    pthread_mutex_lock(&atm_mutex_);

    int retVal = associated_bank_->close_account_bank(params);
    if (GOOD_OP == retVal) printLog(CLOSE_MSG,params);
    else printError(retVal, params);

    pthread_mutex_unlock(&atm_mutex_);
    printSemStatus(associated_bank_, "close_account before release");
    sem_post(&associated_bank_->bank_write);
    printSemStatus(associated_bank_, "close_account after after release");
}

//*******************************************************************************************************//
//atm transfer method. mark itself as Bank reader, and calls bank transfer method and prints suitable log
void atm::atm_transfer_money (actionParams_t* params)
{   
   	printSemStatus(associated_bank_, "transfer before 1st wait for read");
	sem_wait(&associated_bank_->bank_read);
	associated_bank_-> reader_count ++;
   	printSemStatus(associated_bank_, "transfer after 1st reader advance");
	if (associated_bank_->reader_count == 1) {
		sem_wait(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);
    pthread_mutex_lock(&atm_mutex_);
    
    int retVal = associated_bank_->transfer_money_bank(params);
    if (GOOD_OP == retVal) printLog(TRANSFER_MSG,params);
    else printError(retVal, params);
    pthread_mutex_unlock(&atm_mutex_);

	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count--;
	if (associated_bank_->reader_count == 0) {
		sem_post(&associated_bank_->bank_write);
	}
	printSemStatus(associated_bank_, "transfer before 2nd post");	
	sem_post(&associated_bank_->bank_read);
	printSemStatus(associated_bank_, "transfer after 2nd post");
}

//*******************************************************************************************************//
//************************************************************************
//atm_main_loop - create the atm, parse the actions file, calls relevant atm method.
//end when the file is over.
//this is the code that the thread mainly runs
//
//************************************************************************
void* atm_main_loop(void* atmParamsLocal){//int atmNum, pBank bankInst,char const* actionFile){
        //parse all data from main
		if(DEBUG)cout <<"debug in the start of main loop" << "\n";
        pAtmParams me = (pAtmParams)atmParamsLocal;
        int atmNum = me->atmNum;
        pBank bankInst = me->assBank;
        if(DEBUG)cout << "Size of file name string " << sizeof(char)*strlen(me->inputFile) <<"\n";
        char* actionFile = new char[strlen(me->inputFile)];
        if(DEBUG)cout <<"debug in main loop before strcpy" << "\n";
        strcpy(actionFile, me->inputFile);
        if(DEBUG)cout <<"debug in main loop after strcpy" << "\n";
        delete(me);
        //accessing the file.
        std::ofstream logFile;
        logFile.open("log.txt",std::ios::app|std::ios::out);
        if(DEBUG)logFile << "ATM " << atmNum <<" is ready\n";
        logFile.close();
        //create params struct for actions the params struct is a package of data regarding the cut account we handle. it also contains
        //the atm num it was sent from, only for print log needs.
        actionParams_t params;
        params.atmNum = atmNum;
        atm atminst(bankInst, atmNum);
        //scanning the file line by line, extracting relevant data from relevant fields 
        std::ifstream infile(actionFile);
        std::string line;
        if(DEBUG)cout << "*debug -inside atm main loop before the main while" << "\n" ;
        while(std::getline(infile,line)){
                istringstream isl(line);
                if (line.empty()) continue;
                vector<string> lineCmd{istream_iterator<string>{isl}, istream_iterator<string>{}}; 		
                if(DEBUG)cout << "**debug -inside atm main loop inside! the main while --line param is:"<<line << "\n" ;       
                params.accountNum = atoi(lineCmd[1].c_str());
                params.password = lineCmd[2];

                if(DEBUG)cout <<"**debug -atm main loop before the switch lineCmd value is"<<lineCmd[0]<<"\n";

                switch(*lineCmd[0].c_str()){
                    case 'O': 
                		if(DEBUG)cout <<"***debug -atm main loop switch O"<<"\n";
                        params.balance = atoi(lineCmd[3].c_str()); 
                        atminst.atm_open_account(&params);
                        break;
                    case 'D':
                        params.amount = atoi(lineCmd[3].c_str()); 
                        atminst.atm_deposit (&params);
                        break;
                    case 'W':
                        params.amount = atoi(lineCmd[3].c_str()); 
                        atminst.atm_withdraw (&params);
                        break;
                    case 'B':
                        atminst.atm_get_balance (&params);
                        break;
                    case 'Q':
                        atminst.atm_close_account(&params);
                        break;
                    case 'T':
                        params.tranAmount    = atoi(lineCmd[3].c_str());     
                        params.targetAccount = atoi(lineCmd[3].c_str()); 
                        atminst.atm_transfer_money (&params);
                        break;
                    default:
                        printf("Bad action on atm %d",atmNum);
                        break;
                }
                usleep(100000);
        }
        delete(actionFile);
        if (DEBUG) cout << "Done atm loop\n";
        return NULL;
    }
