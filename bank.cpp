//bank.c 

#include "includes.h"
#include "account.h"
#include "bank.h"
#define DEBUG_BANK 1




//**************************************************************************************//
//bank main loop - this function iterativly calls the bank method which charge commission
//the call is timered with 3 second time.
//we lock the entire bank so commission will be charged in a cross account manner with same
//chronological order of the operations order
//**************************************************************************************// 

void* bank_main_loop (void* bankPtr){
    pBank ourBank = (pBank)bankPtr;
    while(1){
        sleep(3);
        sem_wait(&ourBank->bank_write);
        ourBank->take_commission();
        sem_post(&ourBank->bank_write);
    }
    return NULL;
}
//******************************************************************************************//
//bank print loop - this function iterativly calls the bank print method. it is driven by 
//timer of 0.5 sec time. this function locks the entire bank since no changes should be happening in
//the bank or else the print will be in accurate.
//*****************************************************************************************//
void* bank_print_loop(void* bankPtr){
    pBank ourBank = (pBank)bankPtr;
    while(1){
	    sleep(0.5);
        sem_wait(&ourBank->bank_write);
        ourBank->print_status();
        sem_post(&ourBank->bank_write);
    }
    return NULL;
}

//**************************************************************************************//
//constructor. here we init the semaphores
	bank::bank(unsigned int account_num , string password , int balance)
	{   
    	bank_pass = password ;
		bank_account_num = account_num ;
		bal = balance ;
		account temp(bank_account_num,bank_pass ,bal);
        bank_account_ = temp;
        sem_init(&bank_write,1,1) ;
		sem_init(&bank_read,1,1) ;
		reader_count = 0 ;
	}
//**************************************************************************************//
//open account method, this method does not requires any direct method of the account object as it only creates a new account 
//(requires account constructor and operator=    
	int bank::openAccount(actionParams_t *params)
	{
		if(DEBUG) cout<<"inside bank open account\n";
        if (bank_accounts_.end() == bank_accounts_.find(params->accountNum)){
            account new_account = account(params->accountNum, params->password, params->balance);
			bank_accounts_.insert(pair<unsigned int ,account>(params->accountNum,new_account));
			return GOOD_OP;
		}
        if(DEBUG) cout << "done bank open account\n";
        return ACCOUNT_ALRDY_EXIST;
	}
//**************************************************************************************//	
//take commision method - scan the entire accounts map and charge a randomly chosen commision
//when this method operate the charge main loop lock the entire bank in order to keep the chronologic integrity
void bank::take_commission()
	{
        actionParams_t params;       
        params.isDstBank=1;
        std::ofstream logFile;
        logFile.open("log.txt",std::ios::app|std::ios::out);
	    for (accounts_it = bank::bank_accounts_.begin(); accounts_it != bank::bank_accounts_.end(); ++accounts_it)
	    {
		    params.accountNum  = accounts_it->first ; 	    //account id (int)
			account cur_acount = accounts_it->second	;	//account pointer (pointer)
			params.password = cur_acount.password_;
			cur_acount.account_get_balance(&params);
			int rand_commison = rand() % 4 + 2;
            params.tranAmount = (params.balance*rand_commison)/100;
            params.targetAccount = bank_account_num;
			transfer_money_bank(&params);
			if (SCREEN_PRINT) cout<< "Bank: commissions of " <<rand_commison<<" were charged, the bank gained "<< params.tranAmount << " $ from account " << params.accountNum << "\n";
            else logFile<< "Bank: commissions of " <<rand_commison<<" were charged, the bank gained "<< params.tranAmount << " $ from account " << params.accountNum << "\n";
	    }
        logFile.close();
	}
	
//**************************************************************************************//
// bank print method - the bank print method, scans the accounts vector and print each one data.
// when this method operates the print main loop lock the entire bank in order to keep the chronologic integrity 
void bank::print_status()
{
    actionParams_t params;
    if(!SCREEN_PRINT) printf("\033[2J");
    if(!SCREEN_PRINT) printf("\033[1;1H");
	printf("Current Bank Status\n");
	for (accounts_it = bank_accounts_.begin(); accounts_it != bank_accounts_.end(); ++accounts_it)
	{
		if(DEBUG) cout << "Start print Iteration\n";
        params.accountNum = accounts_it->first ;		 //account id (int)
		account cur_acount = accounts_it->second ;		 //account pointer (pointer)
		params.password = cur_acount.password_ ;
		get_balance_bank(&params);
		printf("Account %d: Balance – %d $ , Account Password – %s\n",params.accountNum,params.balance,params.password.c_str());
	}
    if(DEBUG) cout<<"DONE print Iterations\n";
	params.accountNum = bank_account_num; 
    params.password= bank_pass;
    if(DEBUG) cout << "printing bank balance \n";
    bank_account_.account_get_balance(&params);
	printf("The Bank has %d $\n",params.balance);

}



//**************************************************************************************//
//generally: the bank merely checks the validity of the password and that the account existing in its list
//(or not exist - depend on the operation)

int bank::deposit_bank (actionParams_t* params){
    if(bank_accounts_.find(params->accountNum) == bank_accounts_.end()) // cant find account id
        return ACCOUNT_NOT_EXIST;
    else
        return bank_accounts_.find(params->accountNum)->second.account_deposit(params);
}

int bank::withdraw_bank (actionParams_t* params){
    if(bank_accounts_.find(params->accountNum) == bank_accounts_.end()) // cant find account id	
        return ACCOUNT_NOT_EXIST;                                                              	
    else                                                                                       	
        return bank_accounts_.find(params->accountNum)->second.account_withdraw(params);  	
}                                                                                              	
	
int bank::get_balance_bank(actionParams_t* params){
    if(bank_accounts_.find(params->accountNum) == bank_accounts_.end()) // cant find account id	
        return ACCOUNT_NOT_EXIST;                                                              	
    else                                                                                       	
        return bank_accounts_.find(params->accountNum)->second.account_get_balance(params);  	
}                                                                                              

int bank::close_account_bank(actionParams_t* params){
    if(bank_accounts_.find(params->accountNum) == bank_accounts_.end()) // cant find account id	 	
        return ACCOUNT_NOT_EXIST;                                                              	
    else{                                                                                       	
        int rc = bank_accounts_.find(params->accountNum)->second.account_close(params);
        if (GOOD_OP == rc) bank_accounts_.erase(bank_accounts_.find(params->accountNum));
        return rc;
    }
}

int bank::transfer_money_bank(actionParams_t* params){
    if(DEBUG)cout << "inside transfer money\n";
    map<unsigned int ,account>::iterator src_it;
    map<unsigned int ,account>::iterator tgt_it;
    if (!params->isDstBank){
        tgt_it = bank_accounts_.find(params->targetAccount) ;
    }
    if(DEBUG)cout<< "before serching for dst in map\n";
    src_it = bank_accounts_.find(params->accountNum) ; //get referance pointer to account--need allocation ?
    if(DEBUG)cout << "after searching src in map\n";
    int allAccountValid =params->isDstBank ? !(bank_accounts_.end() == src_it) : !((bank_accounts_.end() == src_it) || (bank_accounts_.end() == tgt_it));
    if(!allAccountValid) return ACCOUNT_NOT_EXIST;
    if(DEBUG)cout << "before withdraw\n";
    params->amount = params->tranAmount;        
    int rc = withdraw_bank(params);
    if(DEBUG)cout << "before get money\n";
    if(GOOD_OP != rc) return rc;
    rc = (params->isDstBank) ? bank_account_.account_get_money(params) : tgt_it->second.account_get_money(params);
    if(DEBUG)cout << "done transfer bank\n";
    return rc;
}










