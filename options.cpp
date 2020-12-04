#include "validation.h"
#include "list.h"
#include "block.h"
#include "sha256.h"
#include <map>
#include "array.h"
#include "utility.h"
#include "options.h"
#include "file.h"
#include <fstream>
#include <iostream>
#include "messages.h"
#include "unspent.h"

#define ERR_TRANSFER_FORMAR "Incorrect transfer option format"

map<string,int> optionsMap(){
	map<string,int> options;
	options[OPT_1] = 0;
	options[OPT_2] = 1;
	options[OPT_3] = 2;
	options[OPT_4] = 3;
	options[OPT_5] = 4;
	options[OPT_6] = 5;
	options[OPT_7] = 6;
	options[OPT_8] = 7;
	return options;
}

string callOption(string str, map<string,int> opt_map, List<Block> * list, Array <Transaction> *mempool){
	Array<string> cmd_arr = splitStr(str, ' ');
	string str_to_file;
	
	switch (opt_map[cmd_arr[0]]){
		case 0:
			str_to_file = optInit(cmd_arr, list, mempool);
			break;

		case 1:
			str_to_file = optTransfer(cmd_arr,list, mempool);
			break;

		case 2:
			str_to_file = optMine(cmd_arr,list, mempool);
			break;

		case 3:
			str_to_file = optBalance(cmd_arr, list, mempool);
			break;

		case 4:
			str_to_file = optBlock(cmd_arr, list);
			break;

		case 5:
			str_to_file = optTxn(cmd_arr, list);
			break;

		case 6:
			str_to_file = optLoad(cmd_arr, list, mempool);
			break;

		case 7:
			str_to_file = optSave(cmd_arr, list);
			break;
	}
	return str_to_file;

}

bool optionValid(string opt){
	Array <string> cmd_str;

	cmd_str = splitStr(opt, ' ');
	return cmd_str[0] == OPT_1 || cmd_str[0] == OPT_2  || cmd_str[0] == OPT_3 || cmd_str[0] == OPT_4  || cmd_str[0] == OPT_5  || 
	cmd_str[0] == OPT_6  || cmd_str[0] == OPT_7  || cmd_str[0] == OPT_8 ;
}


string optInit (Array<string> cmd_arr, List <Block> * algochain, Array<Transaction> * mempool){
	if (!validateInitFormat(cmd_arr))
		return STATUS_FAIL;
		
	string block_hash;

	if(!(*algochain).isEmpty()){
		(*algochain).destroy();
	}

	Output out(stof(cmd_arr[2]), sha256(cmd_arr[1]));
	Input in; // Constructor de Input vacío que a su vez llama a constructor de outpoint vacío (el input debe referenciar un outpoint nulo) 

	in.setAddr(NULL_HASH);

	Array<Input> arr_in(0);
	arr_in.addValueEnd(in);
	Array<Output> arr_out(0);
	arr_out.addValueEnd(out);

	Transaction txn(arr_in, arr_out);

	Array<Transaction> arr_txn(0);
	arr_txn.addValueEnd(txn);

	Body bod(GENESIS_N_TXN, arr_txn);
	Header hdr(NULL_HASH, DEFAULT_TXN_HASH , (unsigned int)stoi(cmd_arr[3]), DEFAULT_NONCE );

	Block block(hdr, bod);
	block.setMerkleHash();
	hdr = block.getHeader();

	hdr.validateHash(stoul(cmd_arr[3]));

	block.setHeader(hdr);

	Node<Block> node(block);
	(*algochain).addNodeEnd(node);

	(*mempool).restartArray();

	return block.getBlockHash();

}


string optBlock (Array<string> cmd_arr, List <Block> * algochain){
	if(!validateOptBlockFormat(cmd_arr))
		return STATUS_FAIL;

	Node <Block> * bk_node;
	Block bk;

	bk_node = (*algochain).findByBlockHash(cmd_arr[1]);

	if(bk_node == NULL)
		return STATUS_FAIL;
	
	return ((*bk_node).getData()).toString();
}




string optSave(Array <string> cmd_arr,  List <Block> * algochain){
	if(!validateSaveFormat(cmd_arr))
		return STATUS_FAIL;

	ofstream savefile;

	Array<Block> blocks_arr(0);
	Node <Block> *first_node = (*algochain).getFirst();

	blocks_arr = (*first_node).getAll();

	savefile.open(cmd_arr[1]);

	if(savefile.is_open()){
		for(size_t i = 0; i<blocks_arr.getSize();i++)
			savefile << blocks_arr[i].toString()<<endl;
		savefile.close();
		return STATUS_OK;
	}
	else return STATUS_FAIL;
}

string optLoad(Array <string> cmd_arr,  List <Block> * algochain, Array<Transaction> * mempool){
	if(!validateLoadFormat(cmd_arr))
		return STATUS_FAIL;

	Array<string> fvalues(0);
	fstream loadfile;
	Array<Array<string>> block_string_arr(0);
	Array<Block> block_array(0);
	size_t i;

	loadfile.open(cmd_arr[1]);

	if (loadfile.is_open()){
		fvalues = readFile(loadfile);
		block_string_arr = fileToBlock(fvalues);
	}else
	{
		cerr << ERR_CANT_OPEN_FILE << endl;
		return STATUS_FAIL;
	}
	for(size_t i=0;i<block_string_arr.getSize() ;i++){
		if(!validateBlockFormat(block_string_arr[i])){
			return STATUS_FAIL;
		}
	}

	for (i = 0; i < block_string_arr.getSize(); i++){
		Block curr_block;
		curr_block = stringToBlock(block_string_arr[i]);
		block_array.addValueEnd(curr_block);
	}

	if(!validateLoadedAlgochain(block_array))
		return STATUS_FAIL;

	(*algochain).destroy();

	for (i = 0; i < block_array.getSize(); i++){
		Node<Block> node(block_array[i]);
		(*algochain).addNodeEnd(node);
	}

	(*mempool).restartArray();
		
	return block_array[i - 1].getBlockHash();
}

string optTxn (Array<string> cmd_arr, List <Block> * algochain){
	if(!validateTxnFormat(cmd_arr)){
		return STATUS_FAIL;
	}

	string txn;

	txn = (*algochain).findByTxnHash(cmd_arr[1]); 
	
	if(txn==STATUS_FAIL)
		return STATUS_FAIL;
	
	return txn;
}



string optTransfer(Array<string> cmd_arr,List <Block> * algochain, Array<Transaction> * mempool){
	if(!validateTransferFormat(cmd_arr)){
		return STATUS_FAIL;
	}

	float sum;
	float unspent;
	float funds = 0;

	List<Unspent> unspent_list;
	Array<Output> out_arr(0);
	Array<Input> in_arr(0);

	sum = sumValue(cmd_arr); 
	userUnspent(sha256(cmd_arr[1]), &unspent_list, algochain);

	unspent = totalUnspent(&unspent_list, mempool,sha256(cmd_arr[1]));

	if(unspent < sum || unspent == 0)
			return STATUS_FAIL;

	Node <Unspent> *it_unspent = unspent_list.getFirst();
	
	for(size_t i = 2; i<cmd_arr.getSize();i+=2){	
		out_arr.addValueEnd(Output(stof(cmd_arr[i+1]), sha256(cmd_arr[i])));
	}
	while(it_unspent && funds < sum)
	{
		Input new_input(it_unspent->getData().getOutpoint(),sha256(cmd_arr[1]));
		funds+=it_unspent->getData().getValue();
		in_arr.addValueEnd(new_input);
	}
	if((funds-sum)>0)
		out_arr.addValueEnd(Output(funds-sum, sha256(cmd_arr[1])));

	Transaction new_tx(in_arr,out_arr);

	(*mempool).addValueEnd(new_tx);

	return new_tx.getTxHash();
}

string optBalance(Array<string> cmd_arr, List <Block> * algochain, Array<Transaction> * mempool){
	if(!validateBalanceFormat(cmd_arr))
		return STATUS_FAIL;
	
	List<Unspent> unspent_list;
	float unspent;

	userUnspent(sha256(cmd_arr[1]), &unspent_list, algochain);
	unspent = totalUnspent(&unspent_list, mempool,sha256(cmd_arr[1]));
	return to_string(unspent);
}

string optMine(Array<string> cmd_arr, List<Block> *algochain,  Array <Transaction> *mempool){
	if(!validateMineFormat(cmd_arr))
		return STATUS_FAIL;

	Body new_body;
	Header new_header;
	string prev_block_hash;

	new_body.setTxn_count((*mempool).getSize());
	new_body.setTx_arr(*mempool);

	Block prev_block = (*algochain).getLastNode().getData();
	new_header.setPrevBlock(prev_block.getBlockHash());

	new_header.setBits(stoul(cmd_arr[1]));
	new_header.setNonce(DEFAULT_NONCE);

	Block new_block(new_header,new_body);
	new_block.setMerkleHash();

	
	new_header = new_block.getHeader(); 
	new_header.validateHash(stoul(cmd_arr[1]));

	new_block.setHeader(new_header);

	(*mempool).restartArray();

	Node <Block> node(new_block);
	(*algochain).addNodeEnd(node);

	return new_block.getBlockHash();

}
