#ifndef _TRANSACTION_H_INCLUDED_
#define _TRANSACTION_H_INCLUDED_

#include "array.h"
#include "input.h"
#include "output.h"
#include "sha256.h"

using namespace std;

class Transaction{
    unsigned int n_tx_in; 
    unsigned int n_tx_out;
    Array<Input> input_arr;
    Array<Output> output_arr;

public:

    Transaction();
    Transaction(const Transaction &);
    Transaction(int tx_in, int tx_out);
    Transaction(Array<Input>, Array<Output>);
    ~Transaction();

    unsigned int getN_tx_in()const;
    unsigned int getN_tx_out()const;
    Array<Input> getInput_arr();
    Array<Output> getOutput_arr();

    string getTxHash();

    void setN_tx_in(unsigned int);
    void setN_tx_out(unsigned int);
    void setInput_arr(Array<Input>);
    void setOutput_arr(Array<Output>);
    void setValues(Array <string>);
    string toString();
    bool operator==(const Transaction &);

    friend ostream & operator<< (ostream &os, Transaction t);
};

#endif
