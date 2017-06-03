//bank.h

#ifndef bank_H_
#define bank_H_
#ifndef ACCOUNT_H_
#include "account.h"
#endif


void* bank_main_loop(void* bankPtr);
void* bank_print_loop(void* bankPtr);



class bank
{
	public:
		bank (unsigned int account_num , string password , int balance);
		void take_commission();
        void print_status();
        
        int openAccount(actionParams_t* params);
        int deposit_bank (actionParams_t* params);
        int withdraw_bank (actionParams_t* params);
        int get_balance_bank(actionParams_t* params);
        int close_account_bank(actionParams_t* params);
	    int transfer_money_bank(actionParams_t* params);	
		friend class atm ; //i need the atm to access the acoounts of the bank
        friend void* bank_main_loop (void* bankPtr);
        friend void* bank_print_loop(void* bankPtr);
        friend void printSemStatus(bank* curBank, string phase); 
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

