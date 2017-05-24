//atm.cpp 

#include <atm.h>

//!!!! todo: i need t understand how the input gotten by the bank and deliverd to the atm 
//may be class of argument that the bank handle ?

void printError(int error_code, int atm_id, int account, int bal, int amount){
    switch(error_code){
        case WRONG_PASSWORD:
            printf("Error %d: Your transaction failed – account id %d does not exist\n",atm_id, account);
            break;
        case ACCOUNT_NOT_EXIST:
            printf("Error %d: Your transaction failed – no account with id %d exists",account);        
            break;
        case  MOUNT_ILLEGAL:
            printf("Error %d: Your transaction failed – account id %d balance is lower than %d",atm_id,account,amount);
            break;
        casr ACCOUNT_ALRDY_EXIST:
            printf("Error %d: Your transaction failed – account with the same id exists",atm_id);
            break;
        default: break;
    }
}



const vector<string> breakStr (const char* src, const char& delim){
    char* token = std::strtok(src, delim);
    vector<string> brokenStr;
    int i = 0;
    while(token != NULL){
        brokenStri.push_back(token);
        i++;
        token = std::strtok(NULL, delim);
    }
    return brokenStr;
}


void atm_main_loop(int atmNum, pBank bank,char const actionFile){
    atm atminst(bank,atmNum);
    std::string act;
    vector<string> lineParam;
    FILE* file = fopen(actionFile, "r")
    while(fgets(act, MAX_LINE_SIZE, file){
            lineParam = breakStr(act, " ");
            int account_num = std::stoi(lineParam[1]);
            int password = std::stoi (lineParam[2]));
            switch (lineParam[0]){
                case "O":
                    atm_open_account(account_num ,  password , std::stoi(lineParam[3]));
                    break;
                case "D":
                    atm_deposit (account_num, password, std::stoi(lineParam[3]));
                    break;
                case "W":
                    atm_withdraw (account_num, password, std::stoi(lineParam[3]));
                    break;
                case "B":
                    atm_get_balance (account_num, password);
                    break;
                case "Q":
                    atm_close_account(account_num, password);
                    break;
                case "T":
                    atm_transfer_money (account_num, password, std::stoi(lineParam[3]), std::stoi(lineParam[4]));
                    break;
                default: break;
            }
            usleep(100000);
    }
    fclose(file);  
}


//*******************************************************************************************************//

	atm::atm (bank *associated_bank ,int id_num, pthread_t atm_id )
	{
		associated_bank_ = associated_bank;
		atm_id_ = atm_id;
		id_num_ = id_num;
		pthread_mutex_init(&atm_mutex_ , NULL) ;
	}
	
//*******************************************************************************************************//	
		
	//atm::atm (const atm& atm) ;  //copy c'tor
	
//*******************************************************************************************************//	
	
	atm::~atm () ; //d'tor
	{
		pthread_exit(&atm_id_) ;
		pthread_mutex_destroy(&atm_mutex_) ;
		delete[] associated_bank_ ;
		
	}
	
//*******************************************************************************************************//
	
	void atm::atm_open_account (unsigned int account_num , string password , unsigned int balance)
	{
		pthread_mutex_lock(&atm_mutex_);
		
		
		account* new_account = account::account(account_num, password, balance);
		associated_bank_::bank_accounts_.insert(account_num, &new_account) ; 
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	void atm::atm_deposit (unsigned int account_num , string password , unsigned int amount)
	{	
		pthread_mutex_lock(&atm_mutex_);
		
		
		account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account
		if(account_reff == associated_bank_::bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist\n",id_num_,account_num);
		}
		else
		{	
			int new_balance = account_reff.account_deposit(password,amount);
			if(new_balance==PASS_ERROR);
			{
				printf("Error %d: Your transaction failed – password for account id %d is incorrect\n",id_num_,account_num);
			}
			else 
			{
				printf("%d: Account %d new balance is %d after %d $ was deposited)\n",id_num_,account_num,new_balance,amount);
			}
		}
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	void atm::atm_withdraw (unsigned int account_num , string password , unsigned int amount)
	{
		pthread_mutex_lock(&atm_mutex_);
		
		
		account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account
		if(account_reff == associated_bank_::bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist",id_num_,account_num);
		}
		else
		{	
			int new_balance = account_reff.account_withdraw(password,amount);
			if(new_balance==PASS_ERROR);
			{
				printf("Error %d: Your transaction failed – password for account id %d is incorrect\n",id_num_,account_num);
			}
			else if(new_balance == NEG_ERROR)
			{
				printf("Error %d: Your transaction failed – account id %d balance is lower than %d",id_num_,account_nu,amount);
			}
			else
			{
				printf("%d: Account %d new balance is %d after %d $ was withdrew\n",id_num_,account_num,new_balance,amount);
			}
		}
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	void atm::atm_get_balance (unsigned int account_num , string password)
	{	
		pthread_mutex_lock(&atm_mutex_);
		
		
		account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account
		if(account_reff == associated_bank_::bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist",id_num_,account_num);
		}
		else
		{	
			account_reff.account_get_balance(password);
		//todo: get log massage and return it;
		}
		pthread_mutex_unlock(&atm_mutex_);
	}
//*******************************************************************************************************//
	
	//verifay password and call to the account d'tor 
	void atm::atm_close_account (unsigned int account_num , string password)
	{	
		pthread_mutex_lock(&atm_mutex_);
		
		
		account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account
		if(account_reff == associated_bank_::bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist",id_num_,account_num);
		}
		else
		{	
			account_reff.account_close(password);
		//todo: get log massage and return it;
		}
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	//thie method using account methods to transfer the money. 
	void atm::atm_transfer_money (unsigned int source_account , string password ,unsigned int target_account, unsigned int amount)
	{
		pthread_mutex_lock(&atm_mutex_);
		
		string log;
		&source_account_reff = associated_bank_::bank_accounts_.find(source_account) ; //get referance pointer to account--need allocation ?
		&target_account_reff = associated_bank_::bank_accounts_.find(target_account) ;
		
		source_account_reff.account_withdraw(password,amount);
		//todo: 
		//if(the return value of withdraw indicate success then do) 
			//target_account_reff.account_get_money(amount);
			
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}
	
	
	
	
