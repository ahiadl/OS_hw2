#ifndef atm_H_
#define atm_H_

#define MAX_LINE_SIZE 256


void* atm_main_loop(void* atmParamsiLocal);

class atmParams{
    public:
        int atmNum;
        const char* inputFile;
        pBank assBank;
};

typedef atmParams* pAtmParams;
typedef std::vector<std::string> Tokens;

enum{
    OPEN_MSG     = 0,
    DEPOSIT_MSG  = 1,
    WITHDRAW_MSG = 2,
    BALANCE_MSG  = 3,
    CLOSE_MSG    = 4,
    TRANSFER_MSG = 5,
};



class atm
{
	public:
		atm (pBank associated_bank ,int id_num) ;// c'tor
		
		void atm_open_account (actionParams_t* params);
		void atm_deposit (actionParams_t* params); 
		void atm_withdraw (actionParams_t* params); 
		void atm_get_balance (actionParams_t* params);
		void atm_close_account (actionParams_t* params); 
		void atm_transfer_money (actionParams_t* params); 
		
		friend class bank ;
	private:
		bank *associated_bank_ ;
		int id_num_;
		pthread_mutex_t atm_mutex_ ; //indicate that this atm is in use and unavilibale for new operation.
	
};

#endif
