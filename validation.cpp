#include <iostream>

#include "validation.h"
#include "header.h"
#include "messages.h"
#include "block.h"
#include "options.h"
#define HASH_SIZE 64


// Función que valida el formato de las transacciones recibidas.

bool validateTxFormat(Array<string> fvalues, int *amount){
	size_t i = 0, k;

	// i se utiliza para iterar sobre el vector de strings fvalues (que contiene todas las líneas de las 
	// transacciones recibidas). k se utiliza para iterar sobre cada conjunto de inputs y outputs
	// (por separado) representando cuántos inputs y outputs hay por transacción.


	// La función se ejecuta mientras haya elementos en la posición i del vector.

	while(i < fvalues.getSize()){
		(*amount)++;
		// Se verifica que el primer elemento leído de la transacción sea un número (por medio de isNumber),
		// ya que representa el número de inputs que tiene esa transacción. De ser así, se itera desde 0
		// hasta el número de inputs y por cada iteración, se valida el formato de cada input, mediante
		// la función validateInputFormat. De encontrarse algún formato inválido o cantidad errónea de inputs,
		// se retorna false, luego de enviar el correspondiente mensaje de error y se termina el programa.
	
		if(isNumber(fvalues[i])){
			if((size_t)stoi(fvalues[i]) <= fvalues.getSize() - i -1){
				for (k = 0; k < (size_t)stoi(fvalues[i]); k++){
					if (!validateInputFormat(fvalues[i+k+1])==1){
						return false;
					}
				}
			}
			else{
				cerr << ERR_INPUTS_QUANT << endl;
				return false;
			}
		}
		else{
			cerr << ERR_FORMAT_QUANT_INPUT << endl;
			return false;
		}

	    // Se actualiza el valor de i para que quede en la posición siguiente del vector fvalues 
		// que representa el número de outputs de la misma transacción.

		i += k + 1; 

		// Si i es mayor al tamaño de fvalues, significa que el archivo no tiene más líneas y el 
		// formato es erróneo, o sea faltaría la información que representa cuántos outputs tiene
		// la transacción y los mismos outputs. 

		if(i > fvalues.getSize() - 1){
			cerr << ERR_FILE_INCOMPLETE << endl;
			return false;
		}

		// Se verifica que el elemento leído de la transacción sea un número (por medio de isNumber),
		// ya que representa el número de outputs que tiene esa transacción. De ser así, se itera desde 0
		// hasta el número de outputs y por cada iteración, se valida el formato de cada output, mediante
		// la función validateOutputFormat. De encontrarse algún formato inválido o cantidad errónea de outputs,
		// se retorna false, luego de enviar el correspondiente mensaje de error y se termina el programa.

		if(isNumber(fvalues[i])){
			if((size_t)stoi(fvalues[i]) <= fvalues.getSize() - i -1){
				for (k = 0; k < (size_t)stoi(fvalues[i]); k++){
					if(!validateOutputFormat(fvalues[k+i+1]))
						return false;
				}
			}else{
				cerr << ERR_OUTPUTS_QUANT << endl;
				return false;
			}
		}
		else{
			cerr << ERR_FORMAT_QUANT_OUTPUT << endl;
			return false;
		}

		// Se actualiza la posición de i. Si no hay más transacciones, no se entrará de vuelta
		// al while. Caso contrario, se sigue iterando por cada transacción 

		i += k + 1;
	}
	return true;
}




bool validateInputFormat(string input){

	// Un input está compuesto por 3 términos, que se declaran en orden debajo:

	string tx_id_hash; 	// el hash de la transacción de donde este input toma fondos
	string idx; 		// Int no negativo, índice sobre la secuencia de outputs de la transaccion con hash tx id,
	string addr;		// la dirección de origen de los fondos (debe coincidir con la dirección del output referenciado)

	string delim = TX_DELIM; // delimitador para cada uno de los campos del input
	size_t pos;			     // la posicion del string "input", o sea, el que valido.

	// Se verifica que lleguen 3 términos.

    // el método "find" devuelve la posición de la primera aparición de "delim" o "npos" si
	// "delim" no se encuentra en la cadena.

	if((pos = input.find(delim)) != string::npos){ 
		tx_id_hash = input.substr(0, pos); // se guarda como tx_id desde el principio de la cadena hasta el 1er "delim"
		input = input.substr(pos + delim.length()); // se actualiza input, cortando desde "pos" hasta el final
		
		// A continuación, se valida lo asignado a tx_id_hash (debe ser un hash de 64 caracteres hexadecimales).
		
		if(!isHexa(tx_id_hash)){ 
			cerr << ERR_INPUT_TX_ID_HEXA << endl;
			return false;
		}
		if(tx_id_hash.length() != 64){
			cerr << ERR_INPUT_TX_ID_LENGTH << endl;
			return false;
		}
	}
	else{
		cerr << ERR_INPUT_ONE_ARG <<endl;
		return false;
	}
	if((pos = input.find(delim)) != string::npos){	// se actualiza "pos" hasta el siguiente "delim"
		idx = input.substr(0, pos); // se guarda como idx desde el principio de la cadena hasta el "delim"
		input = input.substr(pos + delim.length()); // se actualiza input desde el "delim" inclusive hasta el final
		// Se utiliza la función isNumber para validar el formato del campo idx (debe ser un número entero positivo).
		if(!isNumber(idx)){
			cerr << ERR_INPUT_IDX << endl;
			return false;
		}
	}
	else{
		cerr << ERR_INPUT_TWO_ARGS <<endl;
		return false;			
	}

	// Por último, para que el formato sea correcto, NO se debe encontrar un nuevo "delim" (ya que, 
	// de encontrarse, significaría que hay 4 términos o más, lo cual no es un formato válido de input).

	if((pos = input.find(delim)) == string::npos){	
		addr = input; // aquí, addr es lo que resta de input.

		// Addr se valida de la misma forma que se valida tx_id_hash.

		if(!isHexa(addr)){
			cerr << ERR_INPUT_ADDR_HEXA << endl;
			return false;
		}
		if(addr.length() != 64){
			cerr << ERR_INPUT_ADDR_LENGTH << endl;
			return false;
		}
	}
	else{
		cerr << ERR_INPUT_TOO_MANY_ARGS <<endl;
		return false;			
	}
	return true;
}


bool validateOutputFormat(string output){
	// Un output está compuesto por 2 términos, que se declaran en orden debajo:
	string value; 	// el valor del output (en algocoins), tiene que ser un float positivo.
	string addr; 		// la dirección de destino de los fondos (hash de 64 caracteres hexadecimales)

	string delim = TX_DELIM; 
	size_t pos;

	// Para los outputs, se deberá verificar que haya 2 términos, el procedimiento es análogo a la
	// función validateInputFormat. Teniendo en cuenta que para validar el formato de "value" se 
	// utiliza la función isFloat. Addr se valida de la misma manera que en validateInputFormat.

	if((pos = output.find(delim)) != string::npos){
		value = output.substr(0, pos);
		output = output.substr(pos + delim.length());
		if(!isFloat(value)){
			cerr << ERR_OUTPUT_VALUE << endl;
			return false;
		}
	}
	else{
		cerr << ERR_OUTPUT_ONE_ARG << endl;
		return false;
	}
	
	if((pos = output.find(delim)) == string::npos){
		addr = output;								
		if(!isHexa(addr)){
			cerr << ERR_OUTPUT_ADDR_HEXA << endl;
			return false;
		}
		if(addr.length() != 64){
			cerr << ERR_OUTPUT_ADDR_LENGTH << endl;
			return false;
		}
	}
	else{
		cerr << ERR_OUTPUT_TO_MANY_ARGS << endl;
		return false;			
	}
	return true;

}



bool validateBlockFormat(Array<string> block)
{
	int n_tx;
	Array <string> tx_aux;

	if(!isHash(block[0]) || !isHash(block[1]) || !isNumber(block[2]) || !isNumber(block[3]) ||!isNumber(block[4]) )
		return false;

	for(size_t i=5; i<block.getSize();i++)
		tx_aux.addValueEnd(block[i]);
	//cout<<tx_aux;
	if(!validateTxFormat(tx_aux,&n_tx) || n_tx != stoi(block[4]))
		return false;
	return true;
}

// Devuelve true si el string es un int válido

bool isNumber(const string & s){
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
			++it;
    return (!s.empty() && it == s.end());
}

// Devuelve true si el string es un hexadecimal válido

bool isHexa(const string & s){  
    string::const_iterator it = s.begin();
    while (it != s.end() && isxdigit(*it)) 
		++it;
    return (!s.empty() && it == s.end());
}

bool isHash(string str){
	return isHexa(str) && str.length() == HASH_SIZE;
}

// Devuelve true si el string es un float válido

bool isFloat(const string& s){         
    string::const_iterator it = s.begin();
    bool decimalPoint = false;
    long unsigned int minSize = 0;
    while(it != s.end()){
		if(*it == '.'){
		if(!decimalPoint)
			decimalPoint = true;
		else break;
		}else if(!isdigit(*it)){
		break;
		}
		++it;
    }
    return s.size() > minSize && it == s.end();
}

// Función que recibe el hash, el map contenedor zero_bits, la dificultad pretendida y 
// retorna true cuando el hash cumple con la dificultad.

bool validateDifficulty(string hash, map<char,int> zero_bits, int diff){
	string::const_iterator it = hash.begin();
	bool isValid = false;

	// Se itera sobre el hash mientras el string tenga caracteres y mientras isValid sea False.
  // Se lee el primer caracter hexadecimal del hash y, por medio de la función zero_bits, se verifica
  // cuántos bits cero contiene a la izquierda dicho número hexadecimal.

  // Si dicha cantidad de ceros supera la dificultad pretendida, entonces la dificultad queda verificada, 
  // isValid es True y no se re-ingresa al while. 

  // Si dicha cantidad de ceros, en cambio, NO supera la dificultad pretendida, 
  // se valida si dicho número es distinto de 0, ya que de ser así, la dificultad no queda validada.

  // Si dicha cantidad de ceros NO supera la dificultad pretendida y ES un 0, se itera nuevamente sobre
  // el hash para validar el siguiente caracter.

	while (it != hash.end() && !isValid){
		if(zero_bits[(*it)] >= (int)diff){
			isValid = true;
		}
		else if((*it) != '0'){
			break;
		}
		diff = diff - zero_bits[(*it)];
		++it;
	}
	return isValid;
}

bool validateInitFormat(Array<string> cmd_arr){

    if(cmd_arr.getSize() != 4){
        return false;
    }


    if(!validateUserFormat(cmd_arr[1])){
        return false;
    }

    if(!validateValueFormat(cmd_arr[2])){
        return false;
    }

    if(!validateBitsFormat(cmd_arr[3])){
        return false;
    }

    return true;
}

bool validateUserFormat(string user){
    size_t pos;
    string delim = TX_DELIM;

    if(user.empty()){
        return false;
    }

    // el método "find" devuelve la posición de la primera aparición de "delim" o "npos" si
    // "delim" no se encuentra en la cadena.
    if((pos = user.find(delim)) != string::npos){
        return false;
    }

    return true;
}

bool validateValueFormat(string value){

    if(value.empty()){
        return false;
    }
    if(!isFloat(value)){
        return false;
    }

    return true;
}

bool validateBitsFormat(string bits){

    if(bits.empty()){
        return false;
    }
    if(!isNumber(bits)){
        return false;
    }

    return true;
}


bool validateSaveFormat(Array <string> cmd_arr)
{
	if (cmd_arr.getSize()!= 2)
		return false;
	else return true;
}

// La función transfer aceptará cualquier combinación de caracteres como user (source).
bool validateTransferFormat(Array<string> cmd_arr){
	if(cmd_arr[1].empty() || cmd_arr.getSize() < 3 || cmd_arr.getSize() % 2 != 0) 
		return false;
	for(size_t i = 2; i<cmd_arr.getSize();i+=2){	
		if(cmd_arr[i].empty() || !isFloat((cmd_arr[i+1])))
			return false;
	}
	return true;
}



bool validateMineFormat(Array<string> cmd_arr){
	return cmd_arr.getSize()==2 && isNumber(cmd_arr[1]);
}

bool validateBalanceFormat(Array<string> cmd_arr){
	return cmd_arr.getSize()==2;
}

bool validateTxnFormat(Array<string> cmd_arr){
	return isHash(cmd_arr[1]) && cmd_arr.getSize() == 2;
}

bool validateLoadFormat(Array<string> cmd_arr){
	return (cmd_arr.getSize()==2);
}

bool validateOptBlockFormat(Array<string> cmd_arr){
	return isHash(cmd_arr[1]) && cmd_arr.getSize() == 2;
}

bool validateLoadedAlgochain(Array<Block> block_array){
	size_t bk_it;

	if(!validateGenesis(block_array[0]))
		return false;

	for (bk_it = 0; bk_it < block_array.getSize(); bk_it++){
		if(!(block_array[bk_it].getHeader().validateHeaderDifficulty())){
			cerr << "The " << bk_it << " block does not verifies the given difficulty." << endl;
			return false;
		}
		if(bk_it > 0){
			if(!(block_array[bk_it].getHeader().getPrev_block() == block_array[bk_it - 1].getBlockHash())){
				cerr << "The prev_block of the block " << bk_it << " doesn't match the hash of the block " << bk_it - 1 << endl;
				return false;
			}
		}
		if(!(block_array[bk_it].getHeader().getTxns_hash() == block_array[bk_it].getMerkleHash())){
			cerr << "The txn_hash of the block's header " << bk_it << " doesn't match the Mekle hash of it's transactions." << endl;
			return false;
		}
		if(!validateDoubleSpending(block_array, bk_it)){
			return false;
		}
		if(bk_it > 0){
			if(!validateTxnFunds(block_array, bk_it)){
				return false;
			}
		}
	}
	
	return true;
}

bool validateDoubleSpending(Array<Block> block_array, size_t bk_it){
	size_t txn_it, bk_it2, txn_it2, out_it, inp_it;


	for (txn_it = 0; txn_it < block_array[bk_it].getBody().getTx_arr().getSize(); txn_it++){
		for(out_it = 0; out_it < block_array[bk_it].getBody().getTx_arr()[txn_it].getOutput_arr().getSize(); out_it++){
			Outpoint outpoint_ref(block_array[bk_it].getBody().getTx_arr()[txn_it].getTxHash(), out_it);
			int output_appears = 0;

			txn_it2 = txn_it + 1;
			for ( ; txn_it2 < block_array[bk_it].getBody().getTx_arr().getSize() && output_appears < 2; txn_it2++){
				for(inp_it = 0; inp_it < block_array[bk_it].getBody().getTx_arr()[txn_it2].getInput_arr().getSize(); inp_it++){
					if(block_array[bk_it].getBody().getTx_arr()[txn_it2].getInput_arr()[inp_it].getOutpoint() == outpoint_ref){
						output_appears += 1;
					}
				}
			}
			if(output_appears >= 2){
				cerr << "There's double spending of the output " << out_it << " the block " << bk_it << endl;
				return false;
			}
			bk_it2 = bk_it + 1;
			for( ; bk_it2 < block_array.getSize(); bk_it2++){
				for (txn_it2 = 0 ; txn_it2 < block_array[bk_it2].getBody().getTx_arr().getSize() && output_appears < 2; txn_it2++){
					for(inp_it = 0; inp_it < block_array[bk_it2].getBody().getTx_arr()[txn_it2].getInput_arr().getSize(); inp_it++){
						if(block_array[bk_it2].getBody().getTx_arr()[txn_it2].getInput_arr()[inp_it].getOutpoint() == outpoint_ref){
							output_appears += 1;
						}
					}
				}
			}
			if(output_appears >= 2){
				cerr << "There's double spending of the output " << out_it << " the block " << bk_it << endl;
				return false;
			}
		}
	}
	return true;

}

bool validateGenesis(Block block){
	if(!(block.getHeader().getPrev_block() == NULL_HASH)){
		cerr << "The first block is not a valid genesis block. The prev block of the genesis block is not null." << endl;
		return false;
	}
	if(!(block.getBody().getTxn_count() == 1)){
		cerr << "The first block is not a valid genesis block. The genesis block must have one and only one transaction." << endl;
		return false;
	}
	if(!(block.getBody().getTx_arr()[0].getN_tx_in() == 1)){
		cerr << "The first block is not a valid genesis block. The genesis block must have one and only one input." << endl;
		return false;
	}
	if(!(block.getBody().getTx_arr()[0].getN_tx_out() == 1)){
		cerr << "The first block is not a valid genesis block. The genesis block must have one and only one output." << endl;
		return false;
	}
	Input input = block.getBody().getTx_arr()[0].getInput_arr()[0];

	if(!(input.getOutpoint().getTx_id() == NULL_HASH)){
		cerr << "The first block is not a valid genesis block. The tx_id of the genesis' input must be a null hash." << endl;
		return false;
	}
	if(!(input.getOutpoint().getIdx() == 0)){
		cerr << "The first block is not a valid genesis block. The idx of the genesis' input must be 0." << endl;
		return false;
	}
	if(!(input.getAddr() == NULL_HASH)){
		cerr << "The first block is not a valid genesis block. The addr of the genesis' input must be a null hash." << endl;
		return false;
	}
	return true;
}

bool validateTxnFunds(Array<Block> block_array, size_t bk_it){
	size_t txn_it, inp_it, bk_it2, out_it, txn_it2;

	for(txn_it = 0; txn_it < block_array[bk_it].getBody().getTx_arr().getSize(); txn_it++){
		float input_funds = 0;
		float output_funds = 0;

		for(inp_it = 0; inp_it < block_array[bk_it].getBody().getTx_arr()[txn_it].getInput_arr().getSize(); inp_it++){
			Outpoint outpoint_ref = block_array[bk_it].getBody().getTx_arr()[txn_it].getInput_arr()[inp_it].getOutpoint();
			string addr_ref = block_array[bk_it].getBody().getTx_arr()[txn_it].getInput_arr()[inp_it].getAddr();
			for(bk_it2 = 0; bk_it2 <= bk_it; bk_it2++){
				for(txn_it2 = 0; txn_it2 < block_array[bk_it2].getBody().getTx_arr().getSize(); txn_it2++){
					if(bk_it2 == bk_it && txn_it2 == txn_it){
						cerr << "Funds of the input not found" << endl;
						return false;
					}
					if(outpoint_ref.getTx_id() == block_array[bk_it2].getBody().getTx_arr()[txn_it2].getTxHash()){
						if(block_array[bk_it2].getBody().getTx_arr()[txn_it2].getOutput_arr()[outpoint_ref.getIdx()].getAddr() == addr_ref){
							input_funds += block_array[bk_it2].getBody().getTx_arr()[txn_it2].getOutput_arr()[outpoint_ref.getIdx()].getValue();
						}else
						{
							cerr << "The addrs of the input and it's referenced output don't match" << endl;
							return false;
						}
						bk_it2 = bk_it + 1;
						break;
					}
				}
			}
		}
		for(out_it = 0; out_it < block_array[bk_it].getBody().getTx_arr()[txn_it].getOutput_arr().getSize(); out_it++){
			output_funds += block_array[bk_it].getBody().getTx_arr()[txn_it].getOutput_arr()[out_it].getValue();
		}
		if(!(input_funds == output_funds)){
			cerr << "The funds in the inputs don't match the funds in the outputs of the transaction " << txn_it << " (block " << bk_it << ")" << endl;
			return false;
		}
	}
	return true;
}
