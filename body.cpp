#include <string>

#include "body.h"
#include "messages.h"

using namespace std;

// Constructores.

Body::Body(){
}

Body::Body(unsigned int n, Array<Transaction> arr){
    txn_count = n;
    tx_arr = arr;
}

Body::Body(const Body &bdy){
    txn_count = bdy.txn_count;
    tx_arr = bdy.tx_arr;
}

// Destructor

Body::~Body(){
    txn_count = 0;
    tx_arr = 0;
}

// Sobrecarga de operadores.

Body &Body::operator=(const Body &bdy){
    txn_count = bdy.txn_count;
    tx_arr = bdy.tx_arr;
    return *this;
}

bool Body::operator==(const Body &bdy){
    return (txn_count == bdy.txn_count && tx_arr == bdy.tx_arr);
}


ostream & operator<< (ostream &os, Body bdy){
    os << bdy.txn_count << endl;
    os << bdy.tx_arr << endl;
    return os;
}

// Métodos de instancia.

unsigned int Body:: getTxn_count(){
    return txn_count;
}

Array<Transaction> Body:: getTx_arr(){
    return tx_arr;
}

void Body:: setTxn_count(unsigned int n){
    txn_count = n;
}
void Body:: setTx_arr(Array<Transaction> arr){
    tx_arr = arr;
}

