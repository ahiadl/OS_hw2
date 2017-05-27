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
	//bank (int atm_number, pthread_t atms, pthread_t bank_operations);
		bank (unsigned int account_num , string password , int balance);
		
		~bank();
		
		void* take_commission ();

        void* print_status () ;
        
        void openAccount(unsigned int account_num, account new_account);
        		
		
		friend class atm ; //i need the atm to access the acoounts of the bank
		
		//friend void bank_main_loop();
		
		//friend void bank_print_loop();
	private:
		//int atm_id_ ;
		string bank_pass ;
		unsigned int bank_account_num ;
		int bal;
		map<unsigned int,account> bank_accounts_ ;
		map<unsigned int,account>::iterator accounts_it ;
		//vector<atm> atms_vector ;
		//vector<pthread_t*> atms_threads ;
		account bank_account_() ;

};

typedef bank* pBank;

# endif 

