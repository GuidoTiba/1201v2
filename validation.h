#ifndef _VALIDATION_H_INCLUDED_
#define _VALIDATION_H_INCLUDED_

#include "array.h"
#include "block.h"
#include <string>
#include <map>

#define TX_DELIM " "

using namespace std;

bool validateTxFormat(Array<string> fvalues, int *amount);
bool validateInputFormat(string input);
bool validateOutputFormat(string output);
bool validateBlockFormat(Array<string> block);


bool isNumber(const string & s);
bool isHexa(const string & s);
bool isFloat(const string& s);
bool isHash(string str);

map<char,int> zeroBitsMap();
bool validateDifficulty(string, map<char,int>, int);
bool validateInitFormat(Array<string> cmd_arr);
bool validateUserFormat(string user);
bool validateValueFormat(string value);
bool validateBitsFormat(string bits);
bool validateSaveFormat(Array <string> cmd_arr);
bool validateTransferFormat(Array<string> cmd_arr);
bool validateMineFormat(Array<string> cmd_arr);
bool validateBalanceFormat(Array<string> cmd_arr);
bool validateTxnFormat(Array<string> cmd_arr);
bool validateLoadFormat(Array<string> cmd_arr);
bool validateOptBlockFormat(Array<string> cmd_arr);
bool validateLoadedAlgochain(Array<Block> block_array);
bool validateGenesis(Block block);
bool validateDoubleSpending(Array<Block> block_array, size_t bk_it);
bool validateTxnFunds(Array<Block> block_array, size_t bk_it);

#endif