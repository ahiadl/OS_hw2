//bank.h

#ifndef bank_H_
#define bank_H_
#include "account.h"
//#include "atm.h"

#include "includes.h"

//forward declaration

//class atm ;

void* bank_main_loop(void* bankPtr);
void* bank_print_loop(void* bankPtr);
class bank
{
	public:
		bank (unsigned int account_num , string password , int balance);
		void* take_commission();
        void* print_status();
        int openAccount(unsigned int account_num, account new_account);
		
		friend class atm ; //i need the atm to access the acoounts of the bank
	private:
		string bank_pass ;
		unsigned int bank_account_num ;
		int bal;
		map<unsigned int,account> bank_accounts_ ;
		map<unsigned int,account>::iterator accounts_it ;
		account bank_account_;

		sem_t bank_read;
		sem_t bank_write;
		int reader_count;

};

typedef bank* pBank;

# endif 

