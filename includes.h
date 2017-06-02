/*
 * includes.h
 *
 *  Created on: May 24, 2017
 *      Author: rs
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_
#define DEBUG 0 
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
#include <fcntl.h>
#include <algorithm>
#include <iterator>

using std::cout;
using std::map ;
using std::vector ;
using std::string ;
using std::pair ;

using namespace std;

enum {
    GOOD_OP             =  0,
    WRONG_PASSWORD      = -1,
    ACCOUNT_NOT_EXIST   = -2,
    AMOUNT_ILLEGAL      = -3,
    ACCOUNT_ALRDY_EXIST = -4,
};

class actionParams_t{
    public:
        unsigned int accountNum;
        string password;
        int amount;
        int balance;
        int tranAmount;
        unsigned int targetAccount;
        int atmNum;
        int dstBalance;
        int isDstBank;
};

#define BANLK_ACCOUNT_NUM 987889898
#define BANLK_ACCOUNT_PASS 959595320

#define SUCCESS 0 ;
#define PASS_ERROR -1
#define NEG_ERROR -2

#endif /* INCLUDES_H_ */
