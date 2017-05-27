//bank.c 

#include "bank.h"


//**************************************************************************************//
//charge commison 
/*void bank_main_loop()
{
	while(1) // todo: i need to use here mutex/semaphores ?? 
	{
		sleep(3)
		for (bank_accounts_::iterator it = bank::bank_accounts_.begin(); it != bank::bank_accounts_.end(); ++it)
		{	
			int cur_acount_num = it->first ; 			 //account id (int)
			account cur_acount = it->second	;	//account pointer (pointer)
			string cur_acount_pass = cur_acount.password_ ;
			int cur_balance = cur_acount.account_get_balance(cur_acount_pass);
			
			int rand_commison = rand() % 4 + 2;
			unsigned int commison_rate = cur_balance*(rand_commison/100);
			
		    cur_acount.account_withdraw(cur_acount_num ,cur_acount_pass,commison_rate); 			
			bank::bank_account_.account_get_money(commison_rate);
			printf("Bank: commissions of %d were charged, the bank gained %d $ from account %d",rand_commison,commison_rate,cur_acount_num);
		}
	}
}



//print bank status
void bank_print_loop()
{
	while(1)
	{
		sleep(0.5);
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
}*/


//**************************************************************************************//
//	bank::bank(int atm_number, pthread_t atms_threads, pthread_t bank_operations)

	bank::bank(unsigned int account_num , string password , int balance)
	{
		bank_pass = password ;
		bank_account_num = account_num ;
		bal = balance ;
		//map<unsigned int,null> bank_accounts_ ;


		bank_account_ = account(bank_account_num,bank_pass ,bal) ;
		bank_accounts_ ;

		
	}

//**************************************************************************************//
    
  //void banjopenAccount(unsigned int account_num, account new_account){
		void bank::openAccount(unsigned int account_num , account new_account)
		{

	  	//map<unsigned int,account*> ref_map = this.bank_accounts_;

        //this.bank_accounts_insert(std::pair <unsigned int,account*>(account_num,new_account));

		bank_accounts_.insert(pair<unsigned int ,account>(account_num,new_account));
		}



	void bank::take_commision()
	{
		while(1) // todo: i need to use here mutex/semaphores ??
			{
				sleep(3);
				for (accounts_it = bank::bank_accounts_.begin(); accounts_it != bank::bank_accounts_.end(); ++accounts_it)
				{
					int cur_acount_num = accounts_it->first ; 			 //account id (int)
					account cur_acount = accounts_it->second	;	//account pointer (pointer)
					string cur_acount_pass = cur_acount.password_;
					int cur_balance = cur_acount.account_get_balance(cur_acount_pass);

					int rand_commison = rand() % 4 + 2;
					unsigned int commison_rate = cur_balance*(rand_commison/100);

					cur_acount.account_withdraw(cur_acount_pass,commison_rate);
					bank::bank_account_.account_get_money(commison_rate);
					printf("Bank: commissions of %d were charged, the bank gained %d $ from account %d",rand_commison,commison_rate,cur_acount_num);
				}
			}
	}
	
//**************************************************************************************//

	void bank::print_status()
	{
		while(1)
	{
		sleep(0.5);
		printf("Current Bank Status\n");
		for (accounts_it = bank::bank_accounts_.begin(); accounts_it != bank::bank_accounts_.end(); ++accounts_it)
		{
			int cur_acount_num = accounts_it->first ;		 //account id (int)
			account cur_acount = accounts_it->second ;		//account pointer (pointer)
			string cur_acount_pass = cur_acount.password_ ;
			int cur_balance = cur_acount.account_get_balance(cur_acount_pass);

			printf("Account %d: Balance – %d $ , Account Password – $c\n",cur_acount_num,cur_balance,cur_acount_pass);
		}
		string bank_pass = bank::bank_account_.password_;
		printf("The Bank has %d $\n",bank::bank_account_.account_get_balance(bank_pass));
	}
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
