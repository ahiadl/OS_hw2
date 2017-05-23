//atm.h

#ifndef atm_H_
#define atm_H_
#include "bank.h"
#include<string.h>

#define MAX_LINE_SIZE 256


enum {
    WRONG_PASSWORD = -1,
    ACCOUNT_NOT_EXIST = -2,
    AMOUNT_ILLEGAL =-3,
}

using namespace std;

class atm 
{
	public:
		atm (bank *associated_bank ,int id_num, pthread_t atm_id ) ;// c'tor
		
		//atm (const atm& atm) ;  //copy c'tor
		
		~atm () ; //d'tor
		
		void atm_open_account (unsigned int account_num , string password , unsigned int balance);
		
		void atm_deposit (unsigned int account_num , string password , unsigned int amount); 
		
		void atm_withdraw (unsigned int account_num , string password , unsigned int amount); 
		
		void atm_get_balance (unsigned int account_num , string password); 
		
		//verifay password and call to the account d'tor 
		void atm_close_account (unsigned int account_num , string password); 
		
		//thie method using account methods to transfer the money. 
		void atm_transfer_money (unsigned int source_account , string password ,unsigned int target_account, unsigned int amount); 
		
		
	private:
		bank *associated_bank_ ;
		pthread_t atm_id_ ;
		int id_num_;
		pthread_mutex_t *atm_mutex_ ; //indicate that this atm is in use and unavilibale for new operation.
	
}

#endif
