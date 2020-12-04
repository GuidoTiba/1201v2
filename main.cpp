#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

#include "file.h"
#include "transaction.h"
#include "array.h"
#include "block.h"
#include "list.h"
#include "validation.h"
#include "cmdline.h"
#include "sha256.h"
#include "header.h"
#include "messages.h"
#include "utility.h"
#include "options.h"

using namespace std;

// A continuación, se definen las funciones para el manejo de opciones por línea de comandos.

static void opt_input(string const &);
static void opt_output(string const &);


static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{0, },
};
static istream *iss = 0;
static ostream *oss = 0;
static fstream ifs;
static fstream ofs;


static void
opt_input(string const &arg){
	// Si el nombre del archivos es "-", usaremos la entrada
	// estándar. De lo contrario, abrimos un archivo en modo
	// de lectura.
	
	if (arg == "-") {
		iss = &cin;
	} else {
		ifs.open(arg.c_str(), ios::in);
		iss = &ifs;
	}

	// Verificamos que el stream este OK.
	
	if (!iss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void
opt_output(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la salida
	// estándar. De lo contrario, abrimos un archivo en modo
	// de escritura.
	
	if (arg == "-") {
		oss = &cout;
	} else {
		ofs.open(arg.c_str(), ios::out);
		oss = &ofs;
	}

	// Verificamos que el stream este OK.
	
	if (!oss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}



int main(int argc, char * const argv[]){
	//  Definición de instancias a utilizar.
	string option_response;
	List <Block> algochain;
	Array<Transaction> mempool(0);
	Block block;
	Array<string> arr_to_file(0);
	Array<string> fvalues(0);
	string user_input;
	map<string,int> opt_map = optionsMap();

	// Manejo de opciones por línea de comandos. Aquí se definen los flujos de entrada y salida que se utilizarán
	// en la ejecución del programa.

	cmdline cmdl(options);
	cmdl.parse(argc, argv); 

	// Lectura del archivo de entrada (si es que se recibe) y asignación del contenido a un vector de
	// strings (fvalues). readFile contempla el caso en el que no se reciba un archivo de entrada 
	// (se habilita la inserción de transacciones por medio del flujo de entrada cin).

	size_t i;

	if (iss==&cin){
		while(!(*iss).eof()){
			user_input = readCin();
			if (optionValid(user_input)){
				option_response = callOption(user_input, opt_map, &algochain, &mempool);
				cout << option_response << endl;
				arr_to_file.addValueEnd(option_response);
			}
			else if (user_input == "\0")
			{}
			else cerr << ERR_WRONG_CMD << endl;
		}	
	}
	else{
		fvalues = readFile(ifs);
		for(i = 0; i<fvalues.getSize(); i++){
			if(optionValid(fvalues[i]))
				arr_to_file.addValueEnd(callOption(fvalues[i], opt_map, &algochain, &mempool)+ BREAK_LINE);
			else cerr<<MSG_LINE_NUMBER<< i + 1 << ERR_INVALID_FILE_FORMAT<<endl;
		}
		if(oss==&cout)
			cout << arr_to_file;
	}
	writeFile(ofs, arr_to_file);

	return 0;

}
