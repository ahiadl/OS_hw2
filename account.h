//account.h

#ifndef account_H_
#define account_H_
#include "atm.h"
#include <string.h>
#include <pthread.h>

using namespace std::string;

class account
{
	public:
		account (unsigned int account_num , string password , unsigned int balance);
		
		~account();
		
		string account_deposit (string password , unsigned int amount); 
		
		string account_withdraw (string password , unsigned int amount); 
		
		string account_get_balance (string password);
		
		string account_close (string password);
		
		string account_get_money (); 
		
		
	private:
		unsigned int account_num_ ;
		string password_ ;
		unsigned int balance_ ;
		
		sem_t *sem_write ;
		sem_t *sem_read ;
		//pthread_mutex_t *mutex_access ;  //critical section mutex 
		int readers_count_ ;
		
}

#endif 
		
