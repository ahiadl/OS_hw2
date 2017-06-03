//bank.c 

#include "includes.h"
#include "account.h"
#include "bank.h"
#define DEBUG_BANK 1




//**************************************************************************************//
//charge commison 

void* bank_main_loop (void* bankPtr){
    pBank ourBank = (pBank)bankPtr;
    int semVal;
    int readSemVal;
    while(1){
        sleep(3);
         if(DEBUG) cout << "Locking commision sem\n";
        sem_getvalue(&ourBank->bank_write, &semVal);
        sem_getvalue(&ourBank->bank_read, &readSemVal);
         if(DEBUG)cout << "comission sem val before lock: " << semVal << "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
        sem_wait(&ourBank->bank_write);
         if(DEBUG) cout << "Locked commision sem\n";
        sem_getvalue(&ourBank->bank_write, &semVal);
         if(DEBUG)cout << "comission sem val after lock: " << semVal<<  "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
        ourBank->take_commission();
        sem_getvalue(&ourBank->bank_write, &semVal);
         if(DEBUG)cout << "comission sem val befor release: " << semVal<< "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
         if(DEBUG) cout << "releasing commision sem\n";
        sem_post(&ourBank->bank_write);
         if(DEBUG) cout << "released commision sem\n";
        sem_getvalue(&ourBank->bank_write, &semVal);
         if(DEBUG)cout << "comission sem val after release: " << semVal<<  "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
    }
    return NULL;
}

void* bank_print_loop(void* bankPtr){
    pBank ourBank = (pBank)bankPtr;
    int semVal;
    int readSemVal;
    while(1){
	    sleep(0.5);
         if(DEBUG) cout << "Locking print sem\n";
        sem_getvalue(&ourBank->bank_write, &semVal);
        sem_getvalue(&ourBank->bank_read, &readSemVal);
         if(DEBUG)cout << "print sem val before lock: " << semVal<<  "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
        sem_wait(&ourBank->bank_write);
        sem_getvalue(&ourBank->bank_write, &semVal);
        sem_getvalue(&ourBank->bank_read, &readSemVal);
         if(DEBUG)cout << "print sem val after lock: " << semVal<<  "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
         if(DEBUG) cout << "Locked print sem\n";
        ourBank->print_status();
         if(DEBUG) cout << "releasing print sem\n";
        sem_getvalue(&ourBank->bank_write, &semVal);
        sem_getvalue(&ourBank->bank_read, &readSemVal);
         if(DEBUG)cout << "print sem val before release: " << semVal<<  "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
        sem_post(&ourBank->bank_write);
        sem_getvalue(&ourBank->bank_read, &readSemVal);
        sem_getvalue(&ourBank->bank_write, &semVal);
         if(DEBUG)cout << "print sem val after release: " << semVal<<  "read sem val: "<< readSemVal <<"read count: " << ourBank->reader_count<< "\n";
         if(DEBUG) cout << "released print sem\n";
    }
    return NULL;
}

//**************************************************************************************//

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
			logFile/*cout*/<< "Bank: commissions of " <<rand_commison<<" were charged, the bank gained "<< params.tranAmount << " $ from account " << params.accountNum << "\n";
	    }
        logFile.close();
	}
	
//**************************************************************************************//

void bank::print_status()
{
    actionParams_t params;
    printf("\033[2J");
    printf("\033[1;1H");
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
    if ((bank_accounts_.end() == src_it) || (bank_accounts_.end() == src_it)) 
        return ACCOUNT_NOT_EXIST;                                                              	
    else{
        if(DEBUG)cout << "before withdraw\n";
        params->amount = params->tranAmount;        
        int rc = withdraw_bank(params);
        if(DEBUG)cout << "before get money\n";
        if(GOOD_OP != rc) return rc;
        rc = (params->isDstBank) ? bank_account_.account_get_money(params) : tgt_it->second.account_get_money(params);
        if(DEBUG)cout << "done transfer bank\n";
        return rc;
    }
}










