#ifndef _BLOCK_H_INCLUDED_
#define _BLOCK_H_INCLUDED_

#include <iostream>
#include <string>

#include "header.h"
#include "body.h"
#include "array.h"
#include "transaction.h"

#define DEFAULT_PREV_BLOCK "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define DEFAULT_NONCE 0

using namespace std;

class Block{
    friend class Header;
    friend class Body;
    Header header;
    Body body;

public:
    Block();
    Block(Header h,Body b);
    Block(const Block &);
    ~Block();
    
    Block &operator=(const Block &);
    bool operator==(const Block &);

    Header getHeader();
    Body getBody();

    void setHeader(Header);
    void setBody(Body);
    void setTxnsHash();
    void setMerkleHash();
    string getBlockHash();
    string toString();
    string getMerkleHash();

    
    friend ostream & operator<< (ostream &, Block);
};
    string merkleRoot(Array<string> hash_arr);
    
#endif
