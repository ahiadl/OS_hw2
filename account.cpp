#include "includes.h"
#include "account.h"


	account::account (unsigned int account_num , string password ,int balance)
	{
        account_num_ = account_num ;
		password_  = password ;
		balance_ = balance ;
		//  todo: verift that this o_creat flag dosent do problems
	//	sem_write = sem_open("sem_write",O_CREAT);
    //	sem_read = sem_open("sem_read",O_CREAT);
		sem_init(&sem_write,1,1) ;
		sem_init(&sem_read,1,1) ;
		readers_count_ = 0 ;
		
	}

	account::account (const account &obj)
	{
		account_num_ =obj.account_num_ ;
		password_ = obj.password_ ;
		balance_ = obj.balance_ ;
		
		sem_write  = obj.sem_write;
		sem_read  = obj.sem_read;
		//pthread_mutex_t *mutex_access ;  //critical section mutex
		readers_count_ = obj.readers_count_ ;
	}

//**********************************************************************************************************//	

	account::~account()
	{	
		sem_close(&sem_write);
		sem_close(&sem_read);
		sem_destroy(&sem_write);
		sem_destroy(&sem_read);
		
		//destroy mutex if will be used
		
	}
				
//**********************************************************************************************************//		

account& account::operator=(const account& src){
   this->account_num_ = src.account_num_;
   this->password_ = src.password_;
   this->balance_ = src.balance_; 
   this->sem_read = src.sem_read;
   this->sem_write = src.sem_write;
   return *this;
}
//**********************************************************************************************************//		
	int account::account_deposit (actionParams_t* params)
	{
		if(password_ != params->password) //bad password
		{
			  return WRONG_PASSWORD;
		}		
		else   // password match 
		{
			sem_wait(&sem_write);
			usleep(1e6);
			balance_ += params->amount ;
            params->balance = balance_;
			sem_post(&sem_write);
		}
		return GOOD_OP;
	}
				
//**********************************************************************************************************//		
 
	int account::account_withdraw (actionParams_t* params)
	{
		if (password_ != params->password)  // bad password
		{
			  return WRONG_PASSWORD;
		}
		else   // password match
		{
			sem_wait(&sem_write);
			usleep(1e6);
			if ( params->amount > balance_){ // illegal withdraw
                params->balance = balance_;
				sem_post(&sem_write);
				return AMOUNT_ILLEGAL;
			}
			else
			{	
				balance_ -= params->amount;
                params->balance = balance_;
				sem_post(&sem_write);
			}
		}
	return GOOD_OP;
	}
	
//**********************************************************************************************************//		
 
	int account::account_get_balance (actionParams_t* params)
	{	
		//string log ; 
        if(DEBUG) cout << "inside  get balance\n";
		if(password_ != params->password)
        {
            return WRONG_PASSWORD;		
        }
		else   // password match
		{	
            if(DEBUG) cout<<"before locking\n";
			sem_wait(&sem_read);
			readers_count_ ++ ;
            if(DEBUG) cout << "before waiting";
			if(readers_count_ == 1)
				sem_wait(&sem_write);
            if(DEBUG) cout << "before post\n" ;  // we dont allow any reader if we need to write .// todo: try -sem_trywait here
			sem_post(&sem_read);
			
			params->balance = balance_;
			usleep(1e6);
			
			if(DEBUG) cout << "done claulating balance\n";
            sem_wait(&sem_read) ;
			readers_count_ -- ;
            if(DEBUG) cout <<"after waiting\n";
			if(readers_count_ == 0)	sem_post(&sem_write);   // we dont allow any reader if we need to write.
            if(DEBUG) cout << "after second post\n";
            sem_post(&sem_read);
            if(DEBUG) cout<< "done get balance\n";
			return GOOD_OP;
			
		}
	
	}	
//**********************************************************************************************************//		

	 int account::account_close (actionParams_t* params)
	 {
		if(password_ != params->password) //bad password
		{
			  return WRONG_PASSWORD;
		}		
		else   // password match 
		{  
			sem_close(&sem_write);
			sem_close(&sem_read);
			sem_destroy(&sem_write);
			sem_destroy(&sem_read);
		}
		return GOOD_OP;
	 }
 
 //**********************************************************************************************************//	
 
	int account::account_get_money (actionParams_t* params)
	{	
	//this method are used for atm transfer money -- the target account don't need password
		
		sem_wait(&sem_write);
		usleep(1e6);
		balance_ += params->tranAmount ;
        params->dstBalance = balance_;
		sem_post(&sem_write)	;
		return GOOD_OP;
	}
 
 
 

		
