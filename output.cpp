
#include "output.h"
#include "utility.h"
#include "messages.h"
#include "array.h"

using namespace std;

// Constructores.

Output::Output(){
    value = 0;
}

Output::Output(float val, string add){
    value = val;
    addr = add;
}

Output::Output(const Output &out){
    value = out.value;
    addr = out.addr;
}

// Destructor.

Output::~Output(){
    value = 0;
    addr.clear();
}

// Sobrecarga de operadores.

Output &Output::operator=(const Output &out){
    value = out.value;
    addr = out.addr;
    return *this;
}

bool Output::operator==(const Output &out){
    return (value == out.value && addr == out.addr);
}

ostream & operator<< (ostream &os, Output outp){
    os<< outp.value << DELIM_DATA << outp.addr;
    return os;
}

// Métodos de instancia.

float const Output::getValue(){
    return value;
}

string const Output::getAddr(){
    return addr;
}

void Output::setValue(float val){
    value = val;
}

void Output::setAddr(string add){
    addr = add;
}

// Función que recibe una cadena de caracteres con los valores del output, separa esos valores dependiendo el
// delimitador utilizado y genera un vector con los valores de los campos, luego setea en los campos 
// correspondientes del output dichos valores.

void Output:: setOutput(string str){
    Array<string> str_field(0);
    Output newoutput;

    str_field = splitStr(str, DELIM_TX);
    setValue(stof(str_field[0]));
    setAddr(str_field[1]);
}

string Output::toString(){
	string output_str;
    string value_to_str = to_string(getValue());
    
    value_to_str.erase(value_to_str.find_last_not_of('0') + 1, std::string::npos);
    if(value_to_str.back() == '.'){
        value_to_str.erase(value_to_str.find_last_not_of('.') + 1, std::string::npos);
    }
	output_str += value_to_str;
	output_str += " ";
	output_str += addr;
	return output_str;
}
