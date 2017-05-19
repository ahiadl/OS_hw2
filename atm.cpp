//atm.cpp 

#include <atm.h>

//!!!! todo: i need t understand how the input gotten by the bank and deliverd to the atm 
//may be class of argument that the bank handle ?

//*******************************************************************************************************//

	atm::atm (bank *associated_bank , pthread_t atm_id )
	{
		associated_bank_ = associated_bank;
		atm_id_ = atm_id;
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
	
	string atm_open_account (unsigned int account_num , string password , unsigned int balance);
	{
		pthread_mutex_lock(&atm_mutex_);
		
		string log;
		account* new_account = account::account(account_num, password, balance);
		associated_bank_::bank_accounts_.insert(account_num, &new_account) ; 
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	string atm_deposit (unsigned int account_num , string password , unsigned int amount); 
	{	
		pthread_mutex_lock(&atm_mutex_);
		
		string log
		&account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account--need allocation ?
		account_reff.account_deposit(password,amount);
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	string atm_withdraw (unsigned int account_num , string password , unsigned int amount); 
	{
		pthread_mutex_lock(&atm_mutex_);
		
		string log;
		&account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account--need allocation ?
		account_reff.account_withdraw(password,amount);
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	string atm_get_balance (unsigned int account_num , string password); 
	{	
		pthread_mutex_lock(&atm_mutex_);
		
		string log;
		&account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get pointer for the account--need allocation ?
		account_reff.account_get_balance(password);
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}
//*******************************************************************************************************//
	
	//verifay password and call to the account d'tor 
	string atm_close_account (unsigned int account_num , string password); 
	{	
		pthread_mutex_lock(&atm_mutex_);
		
		string log;
		&account_reff = associated_bank_::bank_accounts_.find(account_num) ; //get referance pointer to account--need allocation ?
		account_reff.account_close(password);
		//todo: get log massage and return it;
		
		pthread_mutex_unlock(&atm_mutex_);
	}

//*******************************************************************************************************//
	
	//thie method using account methods to transfer the money. 
	string atm_transfer_money (unsigned int source_account , string password ,unsigned int target_account, unsigned int amount); 
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
	
	
	
	