//atm.cpp 

#include "atm.h"
//!!!! todo: i need t understand how the input gotten by the bank and deliverd to the atm 
//may be class of argument that the bank handle ?

/*void printError(int error_code, int atm_id, int account, int bal, int amount){
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
}*/



const vector<string> breakStr (char* src, const char* delim){
    char* token = std::strtok(src, delim);
    vector<string> brokenStr;
    int i = 0;
    while(token != NULL){
        brokenStr.push_back(token);
        i++;
        token = std::strtok(NULL, delim);
    }
    return brokenStr;
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
	
	void atm::atm_open_account (unsigned int account_num , string password , int balance)
	{
		pthread_mutex_lock(&atm_mutex_);
		

		account new_account = account(account_num, password, balance);

		//this->associated_bank_->bank_accounts_[account_num] = new_account;
        int rc = associated_bank_->openAccount(account_num, new_account);
		
        if (rc)
        {
        	printf("%d: New account is %d with password %s and initial balance %d)\n",id_num_,account_num,password,balance);
        }
        else
        {
        	printf("Error %d: Your transaction failed – account with the same is exists\n",account_num);
        }

		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	void atm::atm_deposit (unsigned int account_num , string password , unsigned int amount)
	{	
		pthread_mutex_lock(&atm_mutex_);

		//it = associated_bank_->bank_accounts_.find(account_num) ; //get pointer for the account
		if(associated_bank_->bank_accounts_.find(account_num) == associated_bank_->bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist\n",id_num_,account_num);
		}
		else
		{	
			int new_balance =associated_bank_->bank_accounts_.find(account_num)->second.account_deposit(password,amount);
			if(new_balance==PASS_ERROR)
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

		if(associated_bank_->bank_accounts_.find(account_num) == associated_bank_->bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist",id_num_,account_num);
		}
		else
		{	
			int new_balance = associated_bank_->bank_accounts_.find(account_num)->second.account_withdraw(password,amount);
			if(new_balance==PASS_ERROR)
			{
				printf("Error %d: Your transaction failed – password for account id %d is incorrect\n",id_num_,account_num);
			}
			else if(new_balance == NEG_ERROR)
			{
				printf("Error %d: Your transaction failed – account id %d balance is lower than %d",id_num_,account_num,amount);
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

		if(associated_bank_->bank_accounts_.find(account_num) == associated_bank_->bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist",id_num_,account_num);
		}
		else
		{	
			associated_bank_->bank_accounts_.find(account_num)->second.account_get_balance(password);
		//todo: get log massage and return it;
		}
		pthread_mutex_unlock(&atm_mutex_);
	}
//*******************************************************************************************************//
	
	//verifay password and call to the account d'tor 
	void atm::atm_close_account (unsigned int account_num , string password)
	{	
		pthread_mutex_lock(&atm_mutex_);

		if(associated_bank_->bank_accounts_.find(account_num) == associated_bank_->bank_accounts_.end()) // cant find account id
		{
			printf("Error %d: Your transaction failed – account id %d does not exist",id_num_,account_num);
		}
		else
		{	
			associated_bank_->bank_accounts_.find(account_num)->second.account_close(password);
		//todo: get log massage and return it;
		}
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	//thie method using account methods to transfer the money. 
	void atm::atm_transfer_money (unsigned int source_account , string password ,unsigned int target_account, unsigned int amount)
	{
		pthread_mutex_lock(&atm_mutex_);
		
		//associated_bank_->bank_accounts_.find(account_num)->second

		map<unsigned int ,account>::iterator src_it;
		map<unsigned int ,account>::iterator tgt_it;
		src_it = associated_bank_->bank_accounts_.find(source_account) ; //get referance pointer to account--need allocation ?
		tgt_it = associated_bank_->bank_accounts_.find(target_account) ;
		

		//src_it->second->account_withdraw(password,amount);
		associated_bank_->bank_accounts_.find(source_account)->second.account_withdraw(password,amount);

		//todo: 
		//if(the return value of withdraw indicate success then do) 
			//target_account_reff.account_get_money(amount);
			
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}


const vector<string> breakStr (const char* src, char delim){
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
}

void* atm_main_loop(void* atmParamsLocal){//int atmNum, pBank bankInst,char const* actionFile){

		cout <<"debug in the start of main loop" < "\n";
        pAtmParams me = (pAtmParams)atmParamsLocal;

        int atmNum = me->atmNum;

        pBank bankInst = me->assBank;

        char* actionFile;

        cout <<"debug in main loop before strcpy" < "\n";
        strcpy(actionFile, me->inputFile);
        cout <<"debug in main loop after strcpy" < "\n";
        delete(me);

        atm atminst(bankInst, atmNum);
        //vector<string> lineParam;

        std::ifstream infile(actionFile);
        std::string line;
        cout << "*debug -inside atm main loop before the main while" << "\n" ;
        while(std::getline(infile,line)){
        		cout << "**debug -inside atm main loop inside! the main while --line param is:"<<line << "\n" ;

        		std::stringstream lineParam(line);
                //lineParam= breakStr(line.c_str(), ' ');

                string cmd;
                lineParam >> cmd;
                unsigned int account_num;
                string password;
                int amount;
                lineParam >> account_num >> password >> amount ;
                cout <<"**debug -atm main loop before the switch lineParam value is"<<cmd<<"\n";
                if(cmd == "O"){
                		cout <<"***debug -atm main loop switch O"<<"\n";
                        atminst.atm_open_account(account_num ,  password , amount);
                }
                else if(cmd == "D"){
                        atminst.atm_deposit (account_num, password, amount);
                }
                else if(cmd == "W"){
                        atminst.atm_withdraw (account_num, password, amount);
                }
                else if(cmd == "B"){
                        atminst.atm_get_balance (account_num, password);
                }
                else if(cmd == "Q"){
                        atminst.atm_close_account(account_num, password);
                }
                else if(cmd == "T"){
                    	unsigned int dst;
                    	lineParam >> dst;
                        atminst.atm_transfer_money (account_num, password, amount, dst);
                }


                usleep(100000);
        }
    }
