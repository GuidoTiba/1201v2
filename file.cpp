#include <fstream>
#include <string>
#include "array.h"
#include "file.h"
#include "messages.h"


using namespace std;

// Función que realiza la lectura del archivo de entrada (input file). Se leen las líneas del archivo de texto y se las
// inserta a un vector de cadenas (mediante addValueEnd). Luego se retorna dicho vector para ser validado posteriormente.
// Además, si no se recibe ningún archivo vía línea de comandos, se utiliza el flujo de entrada cin.

string readCin(){
	string str;

	cout << MSG_CMD_INSERT_FX << endl;
	cout << "> ";
	getline(cin, str);
	if (str != "\0")
		return str;
	return str;
}


Array<string> readFile(fstream &fileName){
	Array<string> v(0);
	string str;

	if (fileName.is_open()){
		while(getline(fileName,str)){
			v.addValueEnd(str);
		}
 		fileName.close();
  }
	else
		cerr << ERR_CANT_OPEN_FILE << endl;
 	return v;
}


// Función que recibe un bloque validado y procesado y lo imprime en un archivo de salida .txt. Si no se recibe
// ningún archivo de salida por línea de comandos, se procede a imprimir el bloque por el flujo de salida cout.

void writeFile(fstream &outFile, Array <string> arr){
	if(outFile.is_open()){
		outFile << arr;
		outFile.close();
	}
}

