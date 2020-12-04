#ifndef FILE__H
#define FILE__H

#include <string>
#include <iostream>

#include "block.h"

using namespace std;

Array<string> readFile(fstream &fileName);
Array<string> splitStr(string str_, char delim);
void writeFile(fstream &outFile, Array <string> arr);
string readCin();

#endif