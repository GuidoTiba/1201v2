#ifndef LIST_H
#define LIST_H

#include <iostream>

#include "block.h"
#include "sha256.h"
#include <string>


#define STATUS_FAIL "FAIL"

template <typename T> class List;

template<typename T>
class Node {
	friend class List <T>;

private:
	T data;
	Node <T> *next;
	Node <T> *prev;

public:
	Node(T &value);
	T getData();
	Array <T> getAll();
	Node <T>* getNext();

	void remove(List<T> *);
	void removeAll();

};


template <typename T>
class List{
private:
	Node<T> *first;

public:
	List();
	~List();


	Node<Block>* findByBlockHash(string blockid);
	string findByTxnHash(string txnid);
	Node<T>* getFirst();
	void setFirst(Node<T> *);
	void addNodeEnd(Node <T> node);
	void deleteFirstNode();
	Node<T> getLastNode();
	void destroy();

	void print() const;
	bool isEmpty() const;
};

// En esta clase fueron definidos métodos pensados para utilizar en futuros trabajos prácticos, ya que exceden el 
// alcance de éste. Con esto presente, algunas de las funciones contienen mensajes "hardcodeados" dentro.

using namespace std;

template <typename N> Node<N> ::Node(N &value){
	data = value; 
	next = 0; 
}

template <typename T> T Node<T>::getData(){
	return data;
}

template <typename T> List<T>::List(){
	first = 0;
}

template <typename T> List<T>::~List(){
	Node<T> *aux = first;

	while (!isEmpty()){
		first = first->next;
		delete aux;
		aux = first;
	}
}


template <typename T> void List<T>::addNodeEnd(Node <T> node){
	Node<T> *aux1;
	Node<T> *aux2;
	aux1 = new Node<T>(node);

	if(isEmpty()){             //si la lista está vacia, lo agrega al comienzo 
 		first = aux1;
 		first->next = 0;
		first->prev = 0;
 	} else{
		aux2 = first;
		while(aux2->next)
			aux2 = aux2->next;
		aux2->next =aux1;
		aux1->prev =aux2;
		aux1->next = 0;
	}
}


template <typename T> void List<T>::print() const{
	int i=0;
	Node<T> *aux=first;

	if(!isEmpty()){
		while(aux){
			cout << "El nodo " << i <<" de la lista contiene el valor " << endl << aux->data <<endl;
			aux = aux->next;
			i++;
		}
	}
}

template <typename T> bool List<T>:: isEmpty() const{
	return (first==0);
}


template <typename T> void List<T>::deleteFirstNode(){
	Node<T> *aux1, *aux2;

	if (!isEmpty()){
		if (first->next==first){
			delete first;
			first=0;
		}
	else
	{
		aux1=first;
		aux2=aux1->next;
		while (aux2->next!=first)
			aux2=aux2->next;
		aux2->next=aux1->next;
		first =first->next;
		delete aux1;
	}
	}
}


template <typename T> void List<T>::setFirst(Node<T> * node){
	first = node;
}



template <typename T> void Node<T>::remove(List<T> * list){
	if(prev && next){
		prev->next = next;
		delete this;
	}else if(prev){
		prev->next = 0;
		delete this;
	}else if(next){
		next->prev = 0;
		list->setFirst(next);
		delete this;
	}else{
		next = 0;
		list->destroy();
	}
}

template <typename T> void Node<T>::removeAll(){
	if(next)
		next->removeAll();
	delete this;
}

template <typename T> Node<T> List<T>::getLastNode(){
	Node<T> *aux = first;
	
	while(aux->next)
		aux = aux->next;
	return (*aux);
}

template <typename T> void List<T>::destroy(){
	
	while(!isEmpty()){
		first->removeAll();
		first = 0;
	}
	
	return;

}

template <typename T> Node<Block>* List<T>::findByBlockHash(string blockid){
	Node <Block> *aux = first;
	string block_hash;

	while(aux){
		block_hash = (aux->data).getBlockHash();
		if (block_hash==blockid)
			return aux;
		aux=aux->next;
	}
	return NULL;
}




template <typename T> Array <T> Node<T>::getAll(){
	Node <T> *aux = this;
	Array <T> arr;

	while(aux){
		arr.addValueEnd((*aux).getData());
		aux = aux->next;
	}
	return arr;
}


template <typename T> Node <T>* List<T>::getFirst(){
	return first;
}

template <typename T> Node <T>* Node<T>::getNext(){
	return next;
}

template <typename T> string List<T>::findByTxnHash(string txnid){
	Node <Block> *aux = first;
	Array<Transaction> txn_arr;
	unsigned int txn_count;
	size_t i;
	
	while(aux){
		txn_count = ((aux->data).getBody()).getTxn_count();
		txn_arr = ((aux->data).getBody()).getTx_arr();

		for(i = 0; i < txn_count; i++){
			if(txnid == txn_arr[i].getTxHash()){
				return txn_arr[i].toString();
			}
		}
		aux=aux->next;
	}
	return STATUS_FAIL;
}

#endif

