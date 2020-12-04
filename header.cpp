#include <string>
#include <iostream>
#include "header.h"
#include "sha256.h"
#include "validation.h"
#include "messages.h"
#include "utility.h"


using namespace std;

// Constructores.

Header::Header(){
}

Header::Header(string prevb, string txhash, unsigned int b, unsigned int n){
    prev_block = prevb;
    txns_hash = txhash;
    bits = b;
    nonce = n;
}

Header::Header(const Header &headr){
    prev_block = headr.prev_block;
    txns_hash = headr.txns_hash;
    bits = headr.bits;
    nonce = headr.nonce;
}

// Destructor.

Header::~Header(){
    prev_block.clear();
    txns_hash.clear();
    bits = 0;
    nonce = 0;
}

// Sobrecarga de operadores

Header &Header::operator=(const Header &headr){
    prev_block = headr.prev_block;
    txns_hash = headr.txns_hash;
    bits = headr.bits;
    nonce = headr.nonce;
    return *this;
}

bool Header::operator==(const Header &headr){
    return (prev_block == headr.prev_block &&
    txns_hash == headr.txns_hash &&
    bits == headr.bits &&
    nonce == headr.nonce);
}

// Métodos de instancia.

string const Header:: getPrev_block(){
    return prev_block;
}

string const Header:: getTxns_hash(){
    return txns_hash;
}

unsigned int const Header:: getBits(){
    return bits;
}

unsigned int const Header:: getNonce(){
    return nonce;
}

void Header:: setPrevBlock(string str){
    prev_block = str;
}
void Header:: setTxnsHashManually(string str){
    txns_hash = str;
}

 // Método que recibe un arreglo de cadenas de caracteres donde cada posición corresponde a una línea del archivo de entrada. 
 // Luego procede a generar una sola cadena de caracteres con dicha información y procede a calcular el doble hash de dicha cadena 
 // para cargar el valor al miembro txns_hash.

void Header:: setTxnsHash(Array<string> str_arr){
    string tx_str,hash;
    size_t i = 0;

    while(i<str_arr.getSize()){
        tx_str.append(str_arr[i] + '\n');
        i++;
    }
    hash = sha256(sha256(tx_str));
    txns_hash = hash;
}

void Header:: setBits(unsigned int b){
    bits = b;
}

void Header:: setNonce(unsigned int n){
    nonce = n;
}

ostream & operator<< (ostream &os, Header headr){
    os << headr.prev_block << endl;
    os << headr.txns_hash << endl;
    os << headr.bits << endl;
    os << headr.nonce;
    return os;
}

// Método que devuelve una cadena de caracteres con todos los miembros de la clase separados por un salto de línea.

string Header:: toStr(){
    string hdr_str;

    hdr_str += getPrev_block() + BREAK_LINE;
    hdr_str += getTxns_hash() + BREAK_LINE;
    hdr_str += to_string(getBits()) + BREAK_LINE;
    hdr_str += to_string(getNonce()) + BREAK_LINE;
    
    return hdr_str;
}

// Este metodo comienza realizando el doble hash del header completo y luego, en base al valor de la dificultad ingresado por linea de comando
// procede a validar el hash, mientras no logre cumplir con dicho parametro, irá incrementando el campo nonce y volverá a calcular el doble hash 
// del header completo hasta hallar el nonce que cumpla la dificultad. 

void Header:: validateHash(unsigned int difficulty){
    string hdr_str,hdr_hash;
    hdr_str = toStr();
    hdr_hash = sha256(sha256(hdr_str));
    map<char,int> zeros(zeroBitsMap());
    
    while(!validateDifficulty(hdr_hash, zeros, difficulty)){
        setNonce(getNonce() + NONCE_INCREM);
        hdr_str = toStr();
        hdr_hash = sha256(sha256(hdr_str));
    }   
}

bool Header::validateHeaderDifficulty(){
    map<char,int> zeros(zeroBitsMap());
    string hdr_hash = sha256(sha256(toStr()));

    return validateDifficulty(hdr_hash, zeros, bits);
}
