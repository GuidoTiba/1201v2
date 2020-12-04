#ifndef _OUTPOINT_H_INCLUDED_
#define _OUTPOINT_H_INCLUDED_

#include <iostream>
#include <string>

#define DELIM_DATA " "

using namespace std;

class Outpoint {
    string tx_id;
    unsigned int idx;
public:

    Outpoint();
    Outpoint(string, unsigned int);
    Outpoint(const Outpoint &);
    ~Outpoint();

    Outpoint &operator=(const Outpoint &);
    bool operator==(const Outpoint &);

    string getTx_id();
    unsigned int const getIdx();
    void setTx_id(string);
    void setIdx(unsigned int);

    friend ostream & operator<< (ostream &, Outpoint);
};

#endif
