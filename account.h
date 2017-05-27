//account.h

#ifndef account_H_
#define account_H_

//#include "atm.h"
//#include "bank.h"

#include "includes.h"

class account
{
	public:
		account (unsigned int account_num , string password , int balance);
		
		account (const account &obj);

		virtual ~account();
		
		int account_deposit (string password ,unsigned int amount);
		
		int account_withdraw (string password ,unsigned int amount);
		
		int account_get_balance (string password);
		
		int account_close (string password);
		
		int account_get_money (unsigned int amount); 
		
		friend class bank; //the private bank account access direct to the acouunts methids
		
		//friend class atm;
	private:
		unsigned int account_num_ ;
		string password_ ;
		int balance_ ;
		
		sem_t *sem_write ;
		sem_t *sem_read ;
		//pthread_mutex_t *mutex_access ;  //critical section mutex 
		int readers_count_ ;
		
};

#endif 
		
