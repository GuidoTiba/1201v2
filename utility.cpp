#include <string>
#include <map>

#include "validation.h"
#include "array.h"
#include "utility.h"
#include "transaction.h"
#include "block.h"
#include "header.h"
#include "body.h"
#include "list.h"
#include "unspent.h"
#include "outpoint.h"
#include "input.h"


// Función que recibe una cadena de caracteres y un delimitador y devuelve un vector donde
// se encuentran las subcadenas obtenidas.

Array<string> splitStr(string str_, char delim){
	Array <string> arr(0);
	string aux;
	istringstream iss;

	iss.str(str_);
	while(getline(iss, aux, delim))
  		arr.addValueEnd(aux);
	return arr;  
}

//  Función que crea un vector de vectores de strings donde cada vector
//  de strings corresponde a una transacción.

Array<Array<string>> txArr2Vec(Array<string> str_vec){
	Array<Array<string>> str_2vec(0);
	size_t i = 0, j = 0, k; 
	// i se utiliza para iterar sobre el vector de strings (parámetro str_vec) que
	// representa lo leído en el archivo de entrada. Cuando el valor de i alcanza el tamaño 
	// de str_vec, significa que se recorrió todo el vector. k se utiliza para iterar sobre 
	// cada conjunto de inputs y outputs (por separado) representando cuántos inputs y outputs
	// hay por transacción. Por último, j itera sobre el vector de vectores de cadenas, str_2vec
	// representando cuántas transacciones hay.


	while(i < str_vec.getSize()){
		Array<string> empty_str_vec(0);
		str_2vec.addValueEnd(empty_str_vec);

		// Carga de inputs

		for (k = 0; k < (size_t)stoi(str_vec[i]) + 1; k++){
	  		str_2vec[j].addValueEnd(str_vec[i + k]);
		}
		i += k; 

		// Carga de outputs

		for (k = 0; k < (size_t)stoi(str_vec[i]) + 1; k++){
			str_2vec[j].addValueEnd(str_vec[i + k]);
		}
		i += k;
		j++;
	}
	return str_2vec;
}

// Función que crea el vector de transacciones a partir del vector
// de vectores de strings, o sea, toma los valores y se los asigna
// a los campos de las instancias de Transaction.

Array<Transaction> txArrSet(Array<Array<string>> arr_str_arr){
	Array<Transaction> transaction_vec(0);
	size_t i;

	for (i = 0; i < arr_str_arr.getSize(); i++){
		Transaction transaction;
		transaction.setValues(arr_str_arr[i]);
		transaction_vec.addValueEnd(transaction);
	}
	return transaction_vec;
}


// Función que crea un map representando cuántos bits 0 tiene a la izquierda cada
// uno de los caracteres hexadecimales. Servirá de referencia para validar
// la dificultad pretendida del hash del bloque.

map<char,int> zeroBitsMap(){
	map<char,int> zeros;
	zeros['0'] = 4;
	zeros['1'] = 3;
	zeros['2'] = 2;
	zeros['3'] = 2;
	zeros['4'] = 1;
	zeros['5'] = 1;
	zeros['6'] = 1;
	zeros['7'] = 1;
	zeros['8'] = 0;
	zeros['9'] = 0;
	zeros['a'] = 0;
	zeros['b'] = 0;
	zeros['c'] = 0;
	zeros['d'] = 0;
	zeros['e'] = 0;
	zeros['f'] = 0;
	zeros['A'] = 0;
	zeros['B'] = 0;
	zeros['C'] = 0;
	zeros['D'] = 0;
	zeros['E'] = 0;
	zeros['F'] = 0;
	return zeros;
}

// Función que recibe el fvalues y devuelve un array de array de strings
// donde cada array de strings representa un bloque.

Array<Array<string>> fileToBlock(Array<string> fvalues){
	size_t i = 0, j = 0, k = 0;
	Array<Array<string>> block_vec(0);

	while (i < fvalues.getSize() - 1){
		Array<string> curr_block(0);
		for (j = 0; j < 4; j++){
			curr_block.addValueEnd(fvalues[i+j]);
		}
		i += j;
		for (k = 0; k < (fvalues.getSize() - i) && !(isHash(fvalues[k+i])); k++){
			curr_block.addValueEnd(fvalues[i+k]);
		}
		block_vec.addValueEnd(curr_block);
		i += k;
	}
	return block_vec;
}

Block stringToBlock(Array<string> string_block){
	Block ret_block;
	Header aux_header;
	Body aux_body;
	Array<string> txs_str_vec(0);
	Array<Array<string>> tx_vec_vec_aux(0);
	Array<Transaction> tx_array_aux(0);

	aux_header.setPrevBlock(string_block[0]);
	aux_header.setTxnsHashManually(string_block[1]);
	aux_header.setBits(stoul(string_block[2]));
	aux_header.setNonce(stoul(string_block[3]));

	aux_body.setTxn_count(stoul(string_block[4]));

	for (size_t i = 5; i < string_block.getSize(); i++){
		txs_str_vec.addValueEnd(string_block[i]);
	}

	tx_vec_vec_aux = txArr2Vec(txs_str_vec);
	tx_array_aux = txArrSet(tx_vec_vec_aux);
	aux_body.setTx_arr(tx_array_aux);

	ret_block.setHeader(aux_header);
	ret_block.setBody(aux_body);

	return ret_block;

}

float sumValue(Array<string> cmd_arr){
	float sumatory = 0;
	for(size_t i=3; i<cmd_arr.getSize();i+=2)
		sumatory+=stof(cmd_arr[i]);
	return sumatory;
		
}

void userUnspent(string hash_user, List <Unspent> *unspent_list, List <Block> * algochain){
	Node <Unspent> *it_unspent = unspent_list->getFirst();
	Node <Block> *it_algochain = algochain->getFirst();

	while (it_algochain){
		Array<Transaction> txn_arr(0);
		unsigned int txn_count;

		txn_arr = it_algochain->getData().getBody().getTx_arr();
		txn_count = it_algochain->getData().getBody().getTxn_count();
		for (size_t i = 0; i < txn_count; i++){ 

			Array<Output> output_arr(0);
			unsigned int output_count;

			output_arr = txn_arr[i].getOutput_arr();
			output_count = txn_arr[i].getN_tx_out();

			for (size_t j = 0; j < output_count; j++){

				if (hash_user == output_arr[j].getAddr()){
					Outpoint outpoint(txn_arr[i].getTxHash(), j);
					Unspent new_unspent(output_arr[j].getValue(), outpoint);
					Node<Unspent> unspent_node(new_unspent);
					unspent_list->addNodeEnd(unspent_node);
				}
			}
			Array<Input> input_arr(0);
			unsigned int input_count;

			input_arr = txn_arr[i].getInput_arr();
			input_count = txn_arr[i].getN_tx_in();
			for (size_t j = 0; j < input_count; j++){
				if (hash_user == input_arr[j].getAddr()){
					bool flag = false;
					it_unspent = unspent_list->getFirst();

					while (it_unspent){
						if (it_unspent->getData().getOutpoint() == input_arr[j].getOutpoint()){
							it_unspent->remove(unspent_list);
							it_unspent = unspent_list->getFirst();
							flag = true;
						}else
							it_unspent = it_unspent->getNext();
					}

					if(!flag)
						cerr << "No hay una entrada de dinero que justifique esta salida de dinero." << endl;
				}
			}
		}
		it_algochain = it_algochain->getNext();
	}
	return;
}


float totalUnspent(List<Unspent> *unspent_list, Array<Transaction> *mempool, string hash_user){
	Node <Unspent> *it_unspent = unspent_list->getFirst();

	if((*mempool).getSize()==0)
		return sumUnspent(unspent_list);
	for(size_t i= 0; i<(*mempool).getSize();i++)
	{
		Array<Output> output_arr(0);
		unsigned int output_count;
		output_arr = (*mempool)[i].getOutput_arr();
		output_count = (*mempool)[i].getN_tx_out();
		for (size_t j = 0; j < output_count; j++){
			if (hash_user == output_arr[j].getAddr()){
				Outpoint outpoint((*mempool)[i].getTxHash(), j);
				Unspent new_unspent(output_arr[j].getValue(), outpoint);
				Node<Unspent> unspent_node(new_unspent);
				unspent_list->addNodeEnd(unspent_node);
			}
		}
		Array<Input> input_arr(0);
		unsigned int input_count;
		input_arr = (*mempool)[i].getInput_arr();
		input_count = (*mempool)[i].getN_tx_in();
		for (size_t j = 0; j < input_count; j++){
			if (hash_user == input_arr[j].getAddr()){
				bool flag = false;
				it_unspent = unspent_list->getFirst();
				while (it_unspent){
					if (it_unspent->getData().getOutpoint() == input_arr[j].getOutpoint()){
						it_unspent->remove(unspent_list);
						it_unspent = unspent_list->getFirst();
						flag = true;
					}else
						it_unspent = it_unspent->getNext();
				}
				if(!flag)
					cerr << "No hay una entrada de dinero que justifique esta salida de dinero." << endl;
			}
		}
	}
	return sumUnspent(unspent_list);
}


float sumUnspent(List<Unspent> *unspent_list)
{
	float unspent;
	Node <Unspent> *it_unspent(unspent_list->getFirst());
	while (it_unspent){
		unspent += it_unspent->getData().getValue();
		it_unspent = it_unspent->getNext();
	}
	return unspent;
}
