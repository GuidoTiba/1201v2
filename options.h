#ifndef _OPTIONS_H_INCLUDED_
#define _OPTIONS_H_INCLUDED_

#include "array.h"
#include <string>
#include <map>
#include "list.h"

#define TX_DELIM " "
#define GENESIS_N_TXN 1
#define NULL_HASH "0000000000000000000000000000000000000000000000000000000000000000"
#define DEFAULT_TXN_HASH "TRANSACTION HASH ACA"

#define OPT_1 "init"
#define OPT_2 "transfer"
#define OPT_3 "mine"
#define OPT_4 "balance"
#define OPT_5 "block"
#define OPT_6 "txn"
#define OPT_7 "load"
#define OPT_8 "save"

#define STATUS_FAIL "FAIL"
#define STATUS_OK "OK"

using namespace std;

string optBlock (Array<string> cmd_arr, List <Block> * algochain);
string optSave(Array <string> cmd_arr,  List <Block> * algochain);
string optTxn(Array <string> cmd_arr,  List <Block> * algochain);
string optInit (Array<string> cmd_arr, List <Block> * algochain, Array<Transaction> * mempool);
string optLoad(Array <string> cmd_arr,  List <Block> * algochain, Array<Transaction> * mempool);
string optBalance(Array<string> cmd_arr, List <Block> * algochain, Array<Transaction> * mempool);
string optMine(Array<string> cmd_arr, List<Block> *algochain, Array <Transaction> *mempool);
string optTransfer (Array<string> cmd_arr, List <Block> * algochain, Array<Transaction> * mempool);

bool optionValid(string opt);
map<string,int> optionsMap();

string callOption(string str, map<string,int> opt_map, List<Block> * list, Array <Transaction> *mempool);

#endif