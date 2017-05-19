//bank.c 

#include <bank.h>




//**************************************************************************************//

	bank::bank(int atm_number, pthread_t *atms_threads, pthread_t *bank_operations)
	{
		//map<unsigned int,null> bank_accounts_ ;
		for(int i=0 ; i++ ; i<atm_number) //i need to use iterators ? 
		{	
			
			
			atm* new_atm = atm::atm( &this ,i, atms_threads[i]);
			atms_vector.insert( i , new_atm);
			pthread_create(&atms_threads[i], NULL, &new_atm, int i);
			
			//todo: threads for internal operations
			//todo: join threads --here or in main.cpp ? 
		}
	}

//**************************************************************************************//

	string bank::take_commision()
	{
		
	}
	
//**************************************************************************************//

	string bank::print_status()
	{
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	