//bank.c 

#include "bank.h"


//**************************************************************************************//
//charge commison 
void bank_main_loop()
{
	while(1) // todo: i need to use here mutex/semaphores ?? 
	{
		sleep(3)
		for (bank_accounts_::iterator it = bank::bank_accounts_.begin(); it != bank::bank_accounts_.end(); ++it)
		{	
			int cur_acount_num = it->first 			 //account id (int)
			account cur_acount = it->second		//account pointer (pointer)
			string cur_acount_pass = cur_acount.password_ 
			int cur_balance = cur_acount.account_get_balance(cur_acount_pass);
			
			int rand_commison = rand() % 4 + 2;
			unsigned int commison_rate = cur_balance*(rand_commison/100);
			
		    cur_acount.account_withdraw(cur_acount_num ,cur_acount_pass,commison_rate); 			
			bank::bank_account_.account_get_money(commison_rate);
			printf("Bank: commissions of %d were charged, the bank gained %d $ from account %d",rand_commison,commison_rate,cur_acount_num);
		}
	}
}


//**************************************************************************************//
//print bank status
void bank_print_loop()
{
	while(1)
	{
		sleep(0.5)
		printf("Current Bank Status\n");
		for (bank_accounts_::iterator it = bank::bank_accounts_.begin(); it != bank::bank_accounts_.end(); ++it)
		{	
			int cur_acount_num = it->first ;		 //account id (int)
			account cur_acount = it->second ;		//account pointer (pointer)
			string cur_acount_pass = cur_acount.password_ ;
			int cur_balance = cur_acount.account_get_balance(cur_acount_pass);
			
			printf("Account %d: Balance – %d $ , Account Password – $s\n",cur_acount_num,cur_balance,cur_acount_pass);
		}
		string bank_pass = bank::bank_account_.password_;
		printf("The Bank has %d $\n",bank::bank_account_.account_get_balance(bank_pass));
	}
}


//**************************************************************************************//

	bank::bank(int atm_number, pthread_t *atms_threads, pthread_t *bank_operations)
	{
		//map<unsigned int,null> bank_accounts_ ;
		for(int i=0 ; i++ ; i<atm_number) //i need to use iterators ? 
		{	
			
			
			atm* new_atm = atm::atm( &this ,i, atms_threads[i]);
			atms_vector.insert( i , new_atm);
			bank_account_ = account::account(BANLK_ACCOUNT_NUM BANLK_ACCOUNT_PASS ,0) ; 
			
		
		}
	}

//**************************************************************************************//

/*	string bank::take_commision()
	{
		
	} */
	
//**************************************************************************************//

/*	string bank::print_status()
	{
		
	} */
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
