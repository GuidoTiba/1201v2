#include "outpoint.h"
#include "messages.h"

#define NULL_HASH "0000000000000000000000000000000000000000000000000000000000000000"

using namespace std;

// Constructores.

Outpoint::Outpoint(){
    tx_id = NULL_HASH;
    idx = 0;
}

Outpoint::Outpoint(string tx, unsigned int id){
    tx_id = tx;
    idx = id;
}
Outpoint::Outpoint(const Outpoint &out){
    tx_id = out.tx_id;
    idx = out.idx;
}

// Destructor.

Outpoint::~Outpoint(){
}

// Sobrecarga de operadores

Outpoint &Outpoint::operator=(const Outpoint &out){
    tx_id = out.tx_id;
    idx = out.idx;
    return *this;
}

bool Outpoint::operator==(const Outpoint &out){
    return tx_id == out.tx_id && idx == out.idx;
}

ostream & operator<< (ostream &os, Outpoint outp)
{
    os<< outp.tx_id<< DELIM_DATA << outp.idx;
    return os;
}

// Métodos de instancia.

string Outpoint::getTx_id(){
    return tx_id;
}

unsigned int const Outpoint::getIdx(){
    return idx;
}

void Outpoint::setTx_id(string txid){
    tx_id = txid;
}

void Outpoint::setIdx(unsigned int id){
    idx = id;
}
