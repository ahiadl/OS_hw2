//bank.c 

#include "includes.h"
#include "account.h"
#include "bank.h"
#define DEBUG_BANK 1




//**************************************************************************************//
//charge commison 

void* bank_main_loop (void* bankPtr){
    pBank ourBank = (pBank)bankPtr;
    ourBank->take_commission();
    return NULL;

}

void* bank_print_loop(void* bankPtr){
    pBank ourBank = (pBank)bankPtr;
    ourBank->print_status();
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
		if (bank_accounts_.end() == bank_accounts_.find(params->accountNum)){
            account new_account = account(params->accountNum, params->password, params->balance);
			bank_accounts_.insert(pair<unsigned int ,account>(params->accountNum,new_account));
			return GOOD_OP;
		}else
            return ACCOUNT_ALRDY_EXIST;
	}
//**************************************************************************************//	
    void* bank::take_commission()
	{
        actionParams_t params;        
		while(1){ // todo: i need to use here mutex/semaphores ??
			sleep(3);
		    sem_wait(&bank_write);
			for (accounts_it = bank::bank_accounts_.begin(); accounts_it != bank::bank_accounts_.end(); ++accounts_it)
			{
				    params.accountNum  = accounts_it->first ; 	    //account id (int)
					account cur_acount = accounts_it->second	;	//account pointer (pointer)
					params.password = cur_acount.password_;
					cur_acount.account_get_balance(&params);
					int rand_commison = rand() % 4 + 2;
                    params.tranAmount = params.balance*(rand_commison/100);
                    params.targetAccount = bank_account_num; 
					transfer_money_bank(&params);
					printf("Bank: commissions of %d were charged, the bank gained %d $ from account %d",rand_commison,params.tranAmount,params.accountNum);
		    }
		    sem_post(&bank_write);
		}
	}
	
//**************************************************************************************//

void* bank::print_status()
{
    actionParams_t params;
	while(1){
		sleep(0.5);

       /* sem_wait(&bank_read);
        reader_count++;
		if (reader_count == 1) {
			sem_wait(&bank_write);
		}
        sem_post(&bank_read);*/

		sem_wait(&bank_write);

		printf("Current Bank Status\n");
		for (accounts_it = bank_accounts_.begin(); accounts_it != bank_accounts_.end(); ++accounts_it)
		{
			if(DEBUG_BANK) cout << "Start print Iteration\n";
            params.accountNum = accounts_it->first ;		 //account id (int)
			account cur_acount = accounts_it->second ;		 //account pointer (pointer)
			params.password = cur_acount.password_ ;
			get_balance_bank(&params);
			printf("Account %d: Balance – %d $ , Account Password – %s\n",params.accountNum,params.balance,params.password.c_str());
		}
        if(DEBUG_BANK) cout<<"DONE print Iterations\n";
		params.accountNum = bank_account_num; 
        params.password= bank_pass;
        if(DEBUG_BANK) cout << "printing bank balance \n";
        bank_account_.account_get_balance(&params);
		printf("The Bank has %d $\n",params.balance);
	
		sem_post(&bank_write);

	    /*sem_wait(&bank_read);
	    reader_count--;
	    if(reader_count==0) {
			sem_post(&bank_write);
		}
	    sem_post(&bank_read);*/
    }
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
        if (GOOD_OP == rc) bank_accounts_.erase(params->accountNum);
        return rc;
    }
}

int bank::transfer_money_bank(actionParams_t* params){
    map<unsigned int ,account>::iterator src_it;
    map<unsigned int ,account>::iterator tgt_it;
    if (!params->isDstBank){
        tgt_it = bank_accounts_.find(params->targetAccount) ;
    }
    src_it = bank_accounts_.find(params->accountNum) ; //get referance pointer to account--need allocation ?
    if ((bank_accounts_.end() == src_it) || (bank_accounts_.end() == src_it)) 
        return ACCOUNT_NOT_EXIST;                                                              	
    else{
        params->amount = params->tranAmount;        
        int rc = withdraw_bank(params);
        if(GOOD_OP != rc) return rc;
        rc = (params->isDstBank) ? bank_account_.account_get_money(params):  tgt_it->second.account_get_money(params);
        return rc;
    }
}










