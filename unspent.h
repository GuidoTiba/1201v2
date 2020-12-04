#ifndef _UNSPENT_H_INCLUDED_
#define _UNSPENT_H_INCLUDED_


#include <iostream>
#include <string>
#include "outpoint.h"

using namespace std;

class Unspent{
    float value;
    Outpoint outpoint;
    
public:

    Unspent();
    Unspent(float, Outpoint);
    ~Unspent();
    
    Unspent &operator=(const Unspent &);
    bool operator==(const Unspent &);

    float getValue();
    Outpoint getOutpoint();

    void setValue (float);
    void setOutpoint(Outpoint outpoint);
    friend ostream & operator<< (ostream &, Unspent);

};

#endif
