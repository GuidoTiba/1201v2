#ifndef _HEADER_H_INCLUDED_
#define _HEADER_H_INCLUDED_

#include <iostream>
#include <string>
#include "array.h"

#define NONCE_INCREM 1
#define BREAK_LINE '\n'

using namespace std;

class Header {
    string prev_block;
    string txns_hash;
    unsigned int bits;
    unsigned int nonce;

public:

    Header();
    Header(string prev_block ,string txns_hash, unsigned int bits, unsigned int nonce);
    Header(const Header &);
    ~Header();
    Header &operator=(const Header &);
    bool operator==(const Header &);

    string const getPrev_block();
    string const getTxns_hash();
    string toStr();
    unsigned int const getBits();
    unsigned int const getNonce();

    void setPrevBlock(string str);
    void setTxnsHashManually(string str);
    void setTxnsHash(Array<string> tx_arr);
    void setBits(unsigned int b);
    void setNonce(unsigned int n);
    void validateHash(unsigned int difficulty);
    bool validateHeaderDifficulty();

    friend ostream & operator<< (ostream &os, Header headr);
};


#endif
