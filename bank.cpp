//bank.c 

#include "bank.h"


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
//	bank::bank(int atm_number, pthread_t atms_threads, pthread_t bank_operations)

	bank::bank(unsigned int account_num , string password , int balance)
	{   
        cout << "started ctor\n";
    	bank_pass = password ;
		bank_account_num = account_num ;
		bal = balance ;
        cout << "done set parameters\n";
		account temp(bank_account_num,bank_pass ,bal);
        cout << "done creating temp accout\n";
        bank_account_ = temp;
        cout << "done ctor\n";
        sem_init(&bank_write,1,1) ;
		sem_init(&bank_read,1,1) ;
		reader_count = 0 ;


	}

//**************************************************************************************//
    
  //void banjopenAccount(unsigned int account_num, account new_account){
		int bank::openAccount(unsigned int account_num , account new_account)
		{

	  	//map<unsigned int,account*> ref_map = this.bank_accounts_;

        //this.bank_accounts_insert(std::pair <unsigned int,account*>(account_num,new_account));

			if (bank_accounts_.end()  == bank_accounts_.find(account_num))
			{
				bank_accounts_.insert(pair<unsigned int ,account>(account_num,new_account));
				return 1;
			}
			else
			{
				return 0;
			}
		}


	void* bank::take_commission()
	{
		while(1) // todo: i need to use here mutex/semaphores ??
			{
				sleep(1);
				//sem_wait(&bank_write);
				for (accounts_it = bank::bank_accounts_.begin(); accounts_it != bank::bank_accounts_.end(); ++accounts_it)
				{
					int cur_acount_num = accounts_it->first ; 			 //account id (int)
					account cur_acount = accounts_it->second	;	//account pointer (pointer)
					string cur_acount_pass = cur_acount.password_;
					int cur_balance = cur_acount.account_get_balance(cur_acount_pass);

					int rand_commison = rand() % 4 + 2;
					unsigned int commison_rate = (cur_balance*rand_commison)/100;

					cur_acount.account_withdraw(cur_acount_pass,commison_rate);
					bank::bank_account_.account_get_money(commison_rate);
					printf("Bank: commissions of %d were charged, the bank gained %d $ from account %d\n",rand_commison,commison_rate,cur_acount_num);
				}
				//sem_post(&bank_write);
			}
	}
	
//**************************************************************************************//

	void* bank::print_status()
	{
		while(1)
	{
		sleep(0.5);

        sleep(100);

        sem_wait(&bank_read);
        printf("Current Bank Status\n");
        if(reader_count==0){
        	sem_trywait(&bank_write);
        }
        reader_count++;
        sem_post(&bank_read);

		for (accounts_it = bank::bank_accounts_.begin(); accounts_it != bank::bank_accounts_.end(); ++accounts_it)
		{

			int cur_acount_num = accounts_it->first ;		 //account id (int)
			account cur_acount = accounts_it->second ;		//account pointer (pointer)
			string cur_acount_pass = cur_acount.password_ ;
			int cur_balance = cur_acount.account_get_balance(cur_acount_pass);

			printf("Account %d: Balance – %d $ , Account Password – %s\n",cur_acount_num,cur_balance,cur_acount_pass.c_str());
		}
		string bank_pass = bank::bank_account_.password_;
		printf("The Bank has %d $\n",bank::bank_account_.account_get_balance(bank_pass));

		sem_wait(&bank_read);
		reader_count--;
		if(reader_count==0){
			sem_post(&bank_write);
		}
		sem_post(&bank_read);
	}
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
