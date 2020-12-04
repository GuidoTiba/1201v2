#ifndef _BODY_H_INCLUDED_
#define _BODY_H_INCLUDED_

#include <iostream>
#include "array.h"
#include "transaction.h"

using namespace std;

class Body{
    unsigned int txn_count;
    Array<Transaction> tx_arr;

public:
    
    Body();
    Body(unsigned int, Array<Transaction>);
    Body(const Body &);
	~Body();
    
    Body &operator=(const Body &);
    bool operator==(const Body &); 

    unsigned int getTxn_count();
    Array<Transaction> getTx_arr();
    
    void setTxn_count(unsigned int);
    void setTx_arr(Array<Transaction>);

	friend ostream & operator<< (ostream &os, Body bdy);
};
#endif
