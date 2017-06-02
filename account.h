//account.h

#ifndef account_H_
#define account_H_


class account
{
	public:
		account(){};
        account (unsigned int account_num , string password , int balance);
		account (const account &obj);
		virtual ~account();
		int account_deposit (actionParams_t* params);
		int account_withdraw (actionParams_t* params);
		int account_get_balance (actionParams_t* params);
		int account_close (actionParams_t* params);
		int account_get_money (actionParams_t* params); 
        account& operator=(const account& src);
		friend class bank; //the private bank account access direct to the acouunts methids
		
	private:
		unsigned int account_num_ ;
		string password_ ;
		int balance_ ;
		
		sem_t sem_write ;
		sem_t sem_read ;
		int readers_count_ ;
		
};

#endif 
		
