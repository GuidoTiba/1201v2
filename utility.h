#ifndef UTILITY__H
#define UTILITY__H

#include <iostream>
#include <string>
#include <map>
#include "transaction.h"
#include "array.h"
#include "list.h"
#include "block.h"
#include "unspent.h"

Array<string> splitStr(string str_, char delim);
Array<Array<string>> txArr2Vec(Array<string> str_vec);
Array<Transaction> txArrSet(Array<Array<string>> str_vec_vec);
map<char,int> zeroBitsMap();

Array<Array<string>> fileToBlock(Array<string> fvalues);
Block stringToBlock(Array<string> string_block);
float sumValue(Array<string> cmd_arr);

void userUnspent(string hash_user, List <Unspent> * unspent_list, List <Block> * algochain);
float sumUnspent(List<Unspent> *unspent_list);
float totalUnspent(List<Unspent> *unspent_list, Array<Transaction> *mempool, string hash_user);
#endif



