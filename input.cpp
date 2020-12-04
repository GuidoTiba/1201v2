#include <string>
#include "input.h"
#include "array.h"
#include "file.h"
#include "output.h"
#include "messages.h"
#include <iostream>

using namespace std;

// Constructores.

Input::Input(){
	Outpoint outpoint;
	addr.clear();
}

Input::Input(Outpoint oup, string add){
	outpoint = oup;
	addr = add;
}

Input::Input(const Input &out){
	outpoint = out.outpoint;
	addr = out.addr;
}

// Destructor.

Input::~Input(){
}

// Sobrecarga de operadores.

Input &Input::operator=(const Input &out){
	outpoint = out.outpoint;
	addr = out.addr;
	return *this;
}

bool Input::operator==(const Input &out){
	return (outpoint == out.outpoint && addr == out.addr);
}

ostream & operator<< (ostream &os, Input input){
	os << input.outpoint << DELIM_DATA << input.addr;
	return os;
}

// Métodos de instancia.

Outpoint Input::getOutpoint(){
	return outpoint;
}
string const Input::getAddr(){
	return addr;
} 

void Input::setOutpoint(Outpoint out){
	outpoint = out;
}

void Input::setAddr(string add){
	addr = add;
}

 // Método que recibe una cadena de caracteres con toda la información correspondiente a los miembros de la clase Input. 
 // Luego separa dicha cadena por medio de la función splitStr, la cual retorna un vector con los valores de los miembros los cuales 
 // serán asignados por medio de los setters. 

void Input::setInput(string str){
	Array<string> str_field(0);
	str_field = splitStr(str, DELIM_TX);
	Input newinput;
	Outpoint new_outpoint(str_field[0], stoul(str_field[1]));
  
	setAddr(str_field[2]);
	setOutpoint(new_outpoint);
}

string Input::toString(){
	string input_str;
	input_str += outpoint.getTx_id();
	input_str += " ";
	input_str += to_string(outpoint.getIdx());
	input_str += " ";
	input_str += addr;
	
	return input_str;
}
