 // account.c

#include "account.h"



	account::account (unsigned int account_num , string password ,int balance)
	{
		account_num_ = account_num ;
		password_  = password ;
		balance_ = balance ;
		sem_init(sem_write,1,1) ;
		sem_init(sem_read,1,1) ;
		//pthread_mutex_init(&mutex_crit, NULL);//(sem_queue,1,1) ;
		readers_count_ = 0 ;
		
	}
		
//**********************************************************************************************************//	

	account::~account()
	{	
		sem_close(sem_write);
		sem_close(sem_read);
		sem_destroy(sem_write);
		sem_destroy(sem_read);
		
		//destroy mutex if will be used
		
	}
				
//**********************************************************************************************************//		

	int account::account_deposit (string password , unsigned int amount)
	{
		if(password_ != password) //bad password
		{
			  return PASS_ERROR;
		}		
		else   // password match 
		{
			sem_wait(sem_write);
			usleep(1e6);
			balance_ += amount ;
			sem_post(sem_write)	;
		}
		return balance_;
	}
				
//**********************************************************************************************************//		
 
	int account::account_withdraw (string password ,unsigned int amount)
	{
		if (password_ != password)  // bad password
		{
			  return PASS_ERROR;
		}
		else   // password match
		{
			sem_wait(sem_write);
			usleep(1e6);
			if ( amount > balance_) // illegal withdraw
			{
				//todo: handle illegal withdraw case;
				sem_post(sem_write);
				return NEG_ERROR;
			}
			else
			{	
				balance_ -= amount ;
				//todo: handle log ; 
				sem_post(sem_write);
				
			}
		}
	return balance_ ;
	}
	
//**********************************************************************************************************//		
 
	int account::account_get_balance (string password)
	{	
		//string log ; 
		if(password_ != password) //todo : bad password, handle this case  
		{
			 return PASS_ERROR;
		}
		else   // password match
		{	
			sem_wait(sem_read) ;
			readers_count_ ++ ;
			if(readers_count_ > 0)
				sem_trywait(sem_write);   // we dont allow any reader if we need to write .
			sem_post(sem_read);
			
			int cur_balance = balance_ ;
			usleep(1e6);
			
			sem_wait(sem_read) ;
			readers_count_ -- ;
			if(readers_count_ == 0)
				sem_post(sem_write);   // we dont allow any reader if we need to write.
			sem_post(sem_read);
			return cur_balance;
			
		}
	
	}	
//**********************************************************************************************************//		

	 int account::account_close (string password)
	 {
	
		if(password_ != password) //bad password
		{
			  return PASS_ERROR;
		}		
		else   // password match 
		{
			sem_close(sem_write);
			sem_close(sem_read);
			sem_destroy(sem_write);
			sem_destroy(sem_read);
		}
		return SUCCESS;
	 }
 
 //**********************************************************************************************************//	
 
	int account::account_get_money (unsigned int amount)
	{	
	//this method are used for atm transfer money -- the target account don't need password
		
		sem_wait(sem_write);
		usleep(1e6);
		balance_ += amount ;
		sem_post(sem_write)	;
		return balance_;
	}
 
 
 

		
