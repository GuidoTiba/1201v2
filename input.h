#ifndef _INPUT_H_INCLUDED_
#define _INPUT_H_INCLUDED_

#include <iostream>
#include <string>
#include "outpoint.h"

using namespace std;

class Input {
 	friend class Outpoint;
 	Outpoint outpoint;
 	string addr;

public:
 	Input();
 	Input(Outpoint, string);
	Input(const Input &);
	~Input();
 	Input &operator=(const Input &);
 	bool operator==(const Input &);

 	Outpoint getOutpoint();
 	string const getAddr();

 	void setInput(string);
 	void setOutpoint(Outpoint);
 	void setAddr(string);
	string toString();
 	
 	friend ostream & operator<< (ostream &, Input);
};


#endif
