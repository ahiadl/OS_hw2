//bank.h

#ifndef bank_H_
#define bank_H_
#include<map>
#include<vector> 

using namespace std::map;
using namespace std::vector ; 



class bank
{
	public:
		bank (int atm_number, pthread_t *atms, pthread_t *bank_operations);
		
		~bank();
		
		string take_commision (); 
		
		void print_status () ;
		
		friend class atm ; //i need the atm to access the acoounts of the bank
		
	private:
		int atm_id_ ;
		map<unsigned int,account*> bank_accounts_ ;
		vector<atm*> atms_vector ;
		//vector<pthread_t*> atms_threads ;
		account bank_account_ ;
}

# endif 

