//bank.h

#ifndef bank_H_
#define bank_H_
#include "account.h"
#include "atm.h"

#include <map>
#include <vector> 
#include <string>
#include <string.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::map ;
using std::vector ;
using std::string ;

#define BANLK_ACCOUNT_NUM 987889898
#define BANLK_ACCOUNT_PASS 959595320

#define SUCCESS 0 ;
#define PASS_ERROR -1
#define NEG_ERROR -2 

//forward declaration
class atm;
class account ;

class bank
{
	public:
		bank (int atm_number, pthread_t *atms, pthread_t *bank_operations);
		
		~bank();
		
		char* take_commision (); 
		
		void print_status () ;
		
		
		friend class atm ; //i need the atm to access the acoounts of the bank
		
		friend void bank_main_loop();
		
		friend void bank_print_loop();
	private:
		//int atm_id_ ;
		map<unsigned int,account*> bank_accounts_ ;
		vector<atm*> atms_vector ;
		//vector<pthread_t*> atms_threads ;
		account bank_account_ ;
};

# endif 

