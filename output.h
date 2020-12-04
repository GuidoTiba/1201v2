#ifndef _OUTPUT_H_INCLUDED_
#define _OUTPUT_H_INCLUDED_

#include <iostream>
#include <string>

using namespace std;

class Output {
    float value;
    string addr;
    
public:

    Output();
    Output(float, string);
    Output(const Output &);
    ~Output();
    
    Output &operator=(const Output &);
    bool operator==(const Output &);

    float const getValue();
    string const getAddr();

    void setOutput(string);
    void setValue (float);
    void setAddr (string);
    string toString();
    
    friend ostream & operator<< (ostream &, Output);
};



#endif