//atm.h

#ifndef atm_H_
#define atm_H_
#include<bank.h>
#include<string.h>

using namespace std;

class atm 
{
	public:
		atm (bank *associated_bank , pthread_t atm_id ) ;// c'tor
		
		//atm (const atm& atm) ;  //copy c'tor
		
		~atm () ; //d'tor
		
		string atm_open_account (unsigned int account_num , string password , unsigned int balance);
		
		string atm_deposit (unsigned int account_num , string password , unsigned int amount); 
		
		string atm_withdraw (unsigned int account_num , string password , unsigned int amount); 
		
		string atm_get_balance (unsigned int account_num , string password); 
		
		//verifay password and call to the account d'tor 
		string atm_close_account (unsigned int account_num , string password); 
		
		//thie method using account methods to transfer the money. 
		string atm_transfer_money (unsigned int source_account , string password ,unsigned int target_account, unsigned int amount); 
		
		
	private:
		bank *associated_bank_ ;
		pthread_t atm_id_ ;
		pthread_mutex_t *atm_mutex_ ; //indicate that this atm is in use and unavilibale for new operation.
	
}

#endif