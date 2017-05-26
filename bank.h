//bank.h

#ifndef bank_H_
#define bank_H_
#include "account.h"
#include "atm.h"

#include "includes.h"

//forward declaration
class atm ;

class bank
{
	public:
		bank (int atm_number, pthread_t *atms, pthread_t *bank_operations);
		
		~bank();
		
		void take_commision ();
		
		void print_status () ;
		
		
		friend class atm ; //i need the atm to access the acoounts of the bank
		
		friend void bank_main_loop();
		
		friend void bank_print_loop();
	private:
		//int atm_id_ ;
		map<unsigned int,account*> bank_accounts_ ;
		map<unsigned int,account*>::iterator accounts_it ;
		vector<atm*> atms_vector ;
		//vector<pthread_t*> atms_threads ;
		account bank_account_ ;
};

typedef bank* pBank;

# endif 

