#include "transaction.h"
#include "messages.h"
#define BREAK_LINE '\n'
using namespace std;
   
// Constructores.

Transaction::Transaction(){
    n_tx_in = 0;
    n_tx_out = 0;
}

Transaction::Transaction(const Transaction & transac){
    input_arr = transac.input_arr;
    output_arr = transac.output_arr;
    n_tx_in = transac.n_tx_in;
    n_tx_out = transac.n_tx_out;
}

Transaction::Transaction(int tx_in, int tx_out){
    n_tx_in = tx_in;
    n_tx_out = tx_out;
    Array<Input> input_arr(tx_in);
    Array<Output> output_arr(tx_out);
}

Transaction::Transaction(Array<Input> input_array, Array<Output> output_array){
    input_arr = input_array;
    output_arr = output_array;
    n_tx_in = input_arr.getSize();
    n_tx_out = output_arr.getSize();
}

// Destructor.

Transaction::~Transaction(){
}

// Sobrecarga de operadores.

ostream & operator<< (ostream &os, Transaction t){
 	os << t.n_tx_in<< endl;
    if(t.n_tx_in){
        os << t.input_arr<< endl;
    }
	os << t.n_tx_out;
	if(t.n_tx_out){
        cout << endl;
        os << t.output_arr;
    }

	return os;
}

bool Transaction::operator==(const Transaction &t){
  return (n_tx_in == t.n_tx_in && n_tx_out== t.n_tx_out && input_arr == t.input_arr && output_arr == t.output_arr);
}

// Métodos de instancia.

unsigned int Transaction::getN_tx_in()const{
    return n_tx_in;
}

unsigned int Transaction::getN_tx_out()const{
    return n_tx_out;
}

Array<Input> Transaction::getInput_arr(){
    return input_arr;
}

Array<Output> Transaction::getOutput_arr(){
    return output_arr;
}

void Transaction::setN_tx_in(unsigned int tx_in){
    n_tx_in = tx_in;
}

void Transaction::setN_tx_out(unsigned int tx_out){
    n_tx_out = tx_out;
}

void Transaction::setInput_arr(Array<Input> input_array){
    input_arr = input_array;
}

void Transaction::setOutput_arr(Array<Output> output_array){
    output_arr = output_array;
}

// Función que recibe un arreglo con todos los valores levantados del archivo .txt y luego procede a 
// crear tanto los inputs como los outputs. Finalmente los agrupa en sus vectores correspondientes
// y procede a cargar los campos en la transaccion.

void Transaction::setValues(Array <string> fvalues){
    Array <Input> v_inputs;
    Array <Output> v_outputs;

    size_t i,j;
    for (i=0; i<(size_t)stoi(fvalues[0]); i++){
    	Input input_aux;
    	input_aux.setInput(fvalues[i+1]);
    	v_inputs.addValueEnd(input_aux);
    }
    for(i=i+1, j=0; j<(size_t)stoi(fvalues[i]);j++){
 		Output output_aux;
		output_aux.setOutput(fvalues[j+i+1]);
		v_outputs.addValueEnd(output_aux);
 	}
 	setN_tx_in(stoi(fvalues[0]));
 	setN_tx_out(stoi(fvalues[i]));
	setInput_arr(v_inputs);
	setOutput_arr(v_outputs);
}

string Transaction::toString(){
    string tx_str;
    size_t i;

    tx_str += to_string(getN_tx_in()) + BREAK_LINE;
    for (i = 0; i < getInput_arr().getSize(); i++){
        tx_str += getInput_arr()[i].toString() + BREAK_LINE;
    }
    tx_str += to_string(getN_tx_out()) + BREAK_LINE;
    for (i = 0; i < getOutput_arr().getSize() - 1; i++){

        tx_str += getOutput_arr()[i].toString() + BREAK_LINE;
    }
    tx_str += getOutput_arr()[i].toString();
    return tx_str;
}

string Transaction::getTxHash(){
    return sha256( sha256(((*this).toString() + BREAK_LINE)) );
}
