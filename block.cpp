#include "block.h"
#include "messages.h"
#include <string>
#include "sha256.h"

#define BREAK_LINE '\n'

using namespace std;

// Constructores

Block::Block(){
}

Block::Block(Header h,Body b){
	header = h;
	body = b;
}

Block::Block(const Block &b){
	header = b.header;
	body = b.body;
}

// Destructor.

Block::~Block(){
}

// Sobrecarga de operadores.

Block &Block::operator=(const Block &b){
	header = b.header;
	body = b.body;
	return *this;
}

bool Block::operator==(const Block &b){
	return (header == b.header && body == b.body);
}

ostream & operator<< (ostream &os, Block block){
	os << block.header << endl;
	os << block.body;
	return os;
}

// Métodos de instancia.

Header Block::getHeader(){
	return header;
}

Body Block::getBody(){
	return body;
} 

void Block::setHeader(Header h){
	header = h;
}

void Block::setBody(Body b){
	body = b;
}


void Block::setTxnsHash(){
	Array<string> str_vec(0);
	Array<Transaction> tx_arr = body.getTx_arr();

	for (size_t i = 0; i < tx_arr.getSize(); i++){
		str_vec.addValueEnd(tx_arr[i].toString());
	}
	header.setTxnsHash(str_vec);
}

string Block::toString(){
	string block_str;
	size_t i;

	block_str += header.getPrev_block() + BREAK_LINE;
	block_str += header.getTxns_hash() + BREAK_LINE;
	block_str += to_string(header.getBits()) + BREAK_LINE;
	block_str += to_string(header.getNonce()) + BREAK_LINE;

	block_str += to_string(body.getTxn_count()) + BREAK_LINE;
	for (i = 0; i < body.getTx_arr().getSize() - 1; i++){
		block_str += body.getTx_arr()[i].toString() + BREAK_LINE;
	}
	block_str += body.getTx_arr()[i].toString();

	return block_str;
}

string Block::getBlockHash(){
	return (sha256(sha256(toString() + BREAK_LINE)));
}


void Block::setMerkleHash(){
	Array<string> hash_arr(0);

	for (size_t i = 0; i < body.getTx_arr().getSize(); i++){
		hash_arr.addValueEnd(body.getTx_arr()[i].getTxHash());
	}

	if(hash_arr.getSize()==1){
		header.setTxnsHashManually(sha256(sha256(hash_arr[0]+hash_arr[0])));
		return;
	}

	header.setTxnsHashManually(merkleRoot(hash_arr));

}

string Block::getMerkleHash(){
	Array<string> hash_arr(0);

	for (size_t i = 0; i < body.getTx_arr().getSize(); i++){
		hash_arr.addValueEnd(body.getTx_arr()[i].getTxHash());
	}

	if(hash_arr.getSize()==1){
		return sha256(sha256(hash_arr[0]+hash_arr[0]));
	}

	return merkleRoot(hash_arr);
}


string merkleRoot(Array<string> hash_arr){
	size_t i;
	
	if(hash_arr.getSize()==1)
		return hash_arr[0];

	Array<string> new_hash_arr(0);

	if (hash_arr.getSize()%2 ==0){
		for(i=0;i<hash_arr.getSize(); i+=2){
			new_hash_arr.addValueEnd(sha256(sha256(hash_arr[i]+hash_arr[i+1])));
		}
	}
	else
	{
		for(i=0;i<hash_arr.getSize()-1; i+=2){
			new_hash_arr.addValueEnd(sha256(sha256(hash_arr[i]+hash_arr[i+1])));
		}
		new_hash_arr.addValueEnd(sha256(sha256(hash_arr[i]+hash_arr[i])));
	}
	return merkleRoot(new_hash_arr);
}
