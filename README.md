# Algochain
Esta es una versión simplificada de la blockchain (espacio virtual de las criptomonedas "Bitcoin") donde se emplean los métodos criptográficos básicos sobre los cuales se basan las transacciones de Bitcoins.
El programa está codeado en el lenguaje C++ y el trabajo fue realizado en grupo:
Integrantes:
Aguirre, Pedro
Fernandez Irungaray, Martina
Tibaudin, Guido (yo)

El programa se puede correr ejecutando en linea de comandos:

make (para compilar los archivos)

./tp1 (para ejecutar el programa)


Para interactuar con el programa utilizar las siguientes funciones: 

  -init <user><value><bits>: Genera un bloque génesis para inicializar la Algochain. El bloque
asignará un monto inicial value a la dirección del usuario user. El bloque deberá minarse con la
dificultad bits indicada. Retorna el hash del bloque génesis.
  
  -transfer <src><dst1><value1>... <dstN><valueN>: Genera una nueva transacción en la
que el usuario src transferirá fondos a una cantidad N de usuarios, a cada uno su correspondiente
value. Retorna el Hash de la transacción en caso de éxito y FAIL en caso de falla por invalidez.
  
  -mine <bits>: Ensambla y agrega a la Algochain un nuevo bloque a partir de todas las
transacciones en la mempool. El minado se efectúa con la dificultad dada por bits. Devuelve
el hash del bloque en caso de éxito y FAIL en caso de falla.
  
  -balance <user> Retorna el saldo disponible en la dirección del usuario user.
  -block <id>:Consulta la información del bloque representado por el hash id. Retorna los campos
  
del bloque en caso de éxito y FAIL en caso de recibir un hash inválido.
  -txn <id>:Consulta la información de la transacción representada por el hash id. Retorna los
campos de la transacción en caso de éxito y FAIL en caso de recibir un hash inválido.
  
  -load <filename>: Carga la Algochain presente en el archivo. En caso de éxito devuelve el hash
del ultimo bloque y FAIL en caso de falla.
  
  -save <filename>: Guarda una copia de la Algochain en su estado actual al archivo indicado.
Retorna OK en caso de éxito y FAIL en caso de falla
