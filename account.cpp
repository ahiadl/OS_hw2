// account.c

#include<account.h>



	account::account (unsigned int account_num , string password , unsigned int balance)
	{
		account_num_ = account_num ;
		strcpy(password_ , password) ;  
		balance_ = balance ;
		sem_init(sem_write,1,1) ;
		sem_init(sem_read,1,1) ;
		pthread_mutex_init(&mutex_crit, NULL)(sem_queue,1,1) ;
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

	string account::account_deposit (ustring password , unsigned int amount); 
	{
		string log;
		if(strcmp(password_ , password) != 0) //bad password
		{
			  //todo: handle wrong password case and log massage.
		}		
		else   // password match 
		{
			sem_wait(sem_write);
			usleep(1e6);
			this.balance_ += amount ; 
			//todo : handle log massage
			sem_post(sem_write)	;
		}
		return log;
	}
				
//**********************************************************************************************************//		
 
	string account::account_withdraw (string password , unsigned int amount); 
	{
		string log ;
		if(strcmp(password_ , password) != 0) // bad password 
		{
			  //todo: handle wrong password case and log massage.
		}
		else   // password match
		{
			sem_wait(sem_write);
			usleep(1e6);
			if ( amount > this.balance_) // illegal withdraw
			{
				//todo: handle illegal withdraw case 
				//todo: handle log massage ; 
				sem_post(sem_write);
			}
			else
			{	
				this.balance_ -= amount ;
				//todo: handle log ; 
				sem_post(sem_write);
				
			}
		}
	return log ;	
	}
	
//**********************************************************************************************************//		
 
	string account::account_get_balance (string password)
	{	
		string log ; 
		if(password_ != password) //todo : bad password, handle this case  
		{
			 //todo: handle wrong password case and log massage. 
		}
		else   // password match
		{	
			sem_wait(sem_read) ;
			readers_count_ ++ ;
			if(readers_count_ > 0)
				sem_trywait(sem_write);   // we dont allow any reader if we need to write .
			sem_post(sem_read);
			
			usleep(1e6);
			//todo: "read" (get the balance_ to log) --we can return it as astring and atoi[] it.
			
			sem_wait(sem_read) ;
			readers_count_ -- ;
			if(readers_count_ == 0)
				sem_post(sem_write);   // we dont allow any reader if we need to write.
			sem_post(sem_read);
			
			
		}
	return log
	}	
//**********************************************************************************************************//		

	 string account::account_close (string password)
	 {
		string log;
		if(strcmp(password_ , password) != 0) //bad password
		{
			  //todo: handle wrong password case and log massage.
		}		
		else   // password match 
		{
			sem_close(sem_write);
			sem_close(sem_read);
			sem_destroy(sem_write);
			sem_destroy(sem_read);
		}
		return log;
	 }
 
 //**********************************************************************************************************//	
 
	string account::account_get_money (unsigned int amount)
	{	
	//this method are used for atm transfer money -- the target account don't need password
		string log;
		sem_wait(sem_write);
		usleep(1e6);
		this.balance_ += amount ; 
		//todo : handle log massage
		sem_post(sem_write)	;
		return log;
	}
 
 
 
 

		
