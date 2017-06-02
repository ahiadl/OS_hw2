#include "includes.h"
#include "bank.h"
#include "atm.h"

void printLog(int msg_code, actionParams_t* params){
    std::ofstream logFile;
    logFile.open("log.txt",std::ios::app|std::ios::out);
    switch(msg_code){
        case OPEN_MSG:
            logFile << params->atmNum <<": New aacount id is "<< params->accountNum <<" with password "<< params->password << " and initial balance "<<params->balance <<"\n";
            break;
        case DEPOSIT_MSG:
            logFile << params->atmNum <<": Account "<< params->accountNum <<" new balance is "<< params->balance << " after "<<params->amount << " $ was deposited\n";
            break;
        case WITHDRAW_MSG:
            logFile << params->atmNum <<": Account "<< params->accountNum <<" new balance is "<< params->balance << " after "<<params->amount << " $ was withdrew\n";
            break;
        case BALANCE_MSG:  
            logFile << params->atmNum <<": Account "<< params->accountNum <<" balance is "<< params->balance << "\n";
            break;
        case CLOSE_MSG:  
            logFile << params->atmNum <<": Account "<< params->accountNum <<"is now closed. Balance was " << params->balance <<"\n";
            break;
        case TRANSFER_MSG: 
            logFile << params->atmNum <<": Transfer "<< params->tranAmount <<"from account "<< params->accountNum <<" to account "<< params->targetAccount <<". new account balance is " <<params->balance <<" new target account balance "<< params->dstBalance << "\n";
            break;
        default: break;
    }
    logFile.close();
}


void printError(int error_code, actionParams_t* params){
    std::ofstream logFile;
    logFile.open("log.txt",std::ios::app|std::ios::out);
    switch(error_code){
        case WRONG_PASSWORD:
            logFile <<"Error " << params->atmNum <<": Your transaction failed – password for account "<< params->accountNum << " is incorrect\n";
            break;
        case ACCOUNT_NOT_EXIST:
            logFile << "Error "<< params->atmNum <<": Your transaction failed – account id "<< params->accountNum << " does not exists\n";        
            break;
        case AMOUNT_ILLEGAL:
            logFile << "Error "<< params->atmNum <<": Your transaction failed – account id " << params->accountNum << " balance is lower than "<<params->amount <<"\n";
            break;
        case ACCOUNT_ALRDY_EXIST:
            logFile << "Error "<< params->atmNum <<": Your transaction failed – account with the same id exists\n";
            break;
        default: break;
    }
    logFile.close();
}

//*******************************************************************************************************//

atm::atm (pBank associated_bank ,int id_num)
{
	associated_bank_ = associated_bank;
	id_num_ = id_num;
	pthread_mutex_init(&atm_mutex_ , NULL) ;
}

//*******************************************************************************************************//	
		
	//atm::atm (const atm& atm) ;  //copy c'tor
	
//*******************************************************************************************************//	
	
	/*atm::~atm ()  //d'tor
	{
		pthread_exit(&atm_id_) ;
		pthread_mutex_destroy(&atm_mutex_) ;
		delete[] associated_bank_ ;
		
	}*/
	
//*******************************************************************************************************//
	
void atm::atm_open_account (actionParams_t* params)
{
    sem_wait(&associated_bank_->bank_write);
    pthread_mutex_lock(&atm_mutex_);
    int retVal = associated_bank_->openAccount(params);
    if (GOOD_OP == retVal) printLog(OPEN_MSG, params);
    else printError(retVal,params);
    pthread_mutex_unlock(&atm_mutex_);
    sem_post(&associated_bank_->bank_write);
}

//*******************************************************************************************************//
	
void atm::atm_deposit (actionParams_t* params)
{	

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
	sem_post(&associated_bank_->bank_read);
}

//*******************************************************************************************************//
	
void atm::atm_withdraw (actionParams_t* params)
{
    
	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count++;
	if (associated_bank_->reader_count == 1) {
		sem_wait(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);

    pthread_mutex_lock(&atm_mutex_);
    int retVal = associated_bank_->withdraw_bank(params);
    if (GOOD_OP == retVal) printLog(WITHDRAW_MSG,params);
    else printError(retVal, params);
    pthread_mutex_unlock(&atm_mutex_);
    sem_post(&associated_bank_->bank_write);

	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count--;
	if (associated_bank_->reader_count == 0) {
		sem_post(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);

}

//*******************************************************************************************************//
	
void atm::atm_get_balance (actionParams_t* params)
{	
	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count++;
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
	sem_post(&associated_bank_->bank_read);

}
//*******************************************************************************************************//
	
void atm::atm_close_account (actionParams_t* params)
{	
    sem_wait(&associated_bank_->bank_write);
    pthread_mutex_lock(&atm_mutex_);
    int retVal = associated_bank_->get_balance_bank(params);
    if (GOOD_OP == retVal) printLog(CLOSE_MSG,params);
    else printError(retVal, params);
    pthread_mutex_unlock(&atm_mutex_);
    sem_post(&associated_bank_->bank_write);
}

//*******************************************************************************************************//
void atm::atm_transfer_money (actionParams_t* params)
{
	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count++;
	if (associated_bank_->reader_count == 1) {
		sem_wait(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);

    pthread_mutex_lock(&atm_mutex_);
    int retVal = associated_bank_->transfer_money_bank(params);
    if (GOOD_OP == retVal) printLog(TRANSFER_MSG,params);
    else printError(retVal, params);
    pthread_mutex_unlock(&atm_mutex_);
    sem_post(&associated_bank_->bank_write);

	sem_wait(&associated_bank_->bank_read);
	associated_bank_->reader_count--;
	if (associated_bank_->reader_count == 0) {
		sem_post(&associated_bank_->bank_write);
	}
	sem_post(&associated_bank_->bank_read);
}

//*******************************************************************************************************//

/*const vector<string> breakStr (char* src, const char delim){
    char tempLine[sizeof(*src)] = {0};
    strcpy(tempLine, src);
    char* token = std::strtok(tempLine, &delim);
    vector<string> brokenStr;
    int i = 0;
    while(token != NULL){
        brokenStr.push_back(token);
        i++;
        token = std::strtok(NULL, &delim);
    }
    return brokenStr;
}*/

void* atm_main_loop(void* atmParamsLocal){//int atmNum, pBank bankInst,char const* actionFile){

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
        
        std::ofstream logFile;
        logFile.open("log.txt",std::ios::app|std::ios::out);
        logFile << "ATM " << atmNum <<" is ready\n";
        logFile.close();
        
        actionParams_t params;
        params.atmNum = atmNum;
        atm atminst(bankInst, atmNum);
        
        std::ifstream infile(actionFile);
        std::string line;
        if(DEBUG)cout << "*debug -inside atm main loop before the main while" << "\n" ;
        while(std::getline(infile,line)){
                istringstream isl(line);
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
        return NULL;
    }
