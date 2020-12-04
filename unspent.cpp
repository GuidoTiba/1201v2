#include "unspent.h"
#include "utility.h"
#include "messages.h"
#include "array.h"
#include "outpoint.h"

using namespace std;

// Constructores.

Unspent::Unspent(){
    value = 0;
}

Unspent::Unspent(float val, Outpoint outpnt){
    value = val;
    outpoint = outpnt;
}

Unspent::~Unspent(){
    value = 0;
}

// Sobrecarga de operadores.

Unspent &Unspent::operator=(const Unspent &uns){
    value = uns.value;
    outpoint = uns.outpoint;
    return *this;
}

bool Unspent::operator==(const Unspent &unsp){
    return (value == unsp.value && outpoint == unsp.outpoint);
}


// Getters

float Unspent::getValue(){
    return value;
}

Outpoint Unspent::getOutpoint(){
    return outpoint;
}


//Setters

void Unspent::setValue(float val){
    value = val;
}
void Unspent::setOutpoint (Outpoint outpnt){
	outpoint=outpnt;
}

ostream & operator<< (ostream &os, Unspent unsp){
    os<< unsp.getValue() << DELIM_DATA << unsp.getOutpoint();
    return os;
}
