#ifndef ARRAY__H
#define ARRAY__H

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>   
#include "messages.h"

#define INIT_SIZE 0
#define ARR_DEFAULT_SIZE 2 
#define DELIM_TX ' '

using namespace std;

template <typename T>
class Array{
    T * data;
    size_t size = INIT_SIZE;
    size_t alloc_size = ARR_DEFAULT_SIZE;

public:
    
    Array(const Array<T> &);
    Array(int n=ARR_DEFAULT_SIZE);
    ~Array();

    size_t getSize()const;

    Array <T> & operator=(const Array <T> &);
    bool operator==(const Array <T> &)const;
    bool operator!=(const Array <T> &)const;
    T & operator[](int i) const;
    void addValueEnd(T info);
    void restartArray();
  
    template <typename TT> 
    friend ostream& operator<<(ostream &os, Array<TT> const&arr);
};

// Constructores de instancias de la clase Array.

template <typename T>
Array<T>::Array(int n){
    data = new T[n];
    alloc_size = n;
    size = INIT_SIZE;
}

template <typename T>
Array<T>::Array(const Array<T> & arr){
    size = arr.size;
    data = new T[size];

    for (size_t i=0; i < size; i++)
        data[i] = arr[i];
}

// Destructor.

template <typename T>
Array <T>::~Array(){
    if(data != NULL)
        delete []data;
}

// Métodos de instancia.

template <typename T>
size_t Array<T>::getSize() const{
    return size;
}

// Sobrecarga de operadores.

template <typename T>
Array <T> & Array <T>::operator=(const Array <T> & r){
    if(&r != this){
        if (size != r.size){
            T * aux;
            aux = new T[r.size];
            delete[] data;
            size = r.size;
            data = aux;
        }
        for (size_t i = 0; i < size; i++)
            data[i] = r.data[i];
    }
    return *this;
}

template <typename T>
bool Array<T>::operator==(const Array & r)const{
    size_t i;
    if(size != r.size)
        return false;
    else
        for(i=0; i<size; i++){ 
            if(!(data[i] == r.data[i]))
                return false;
        }
    return true;
}

template <typename T>
bool Array<T>:: operator!=(const Array <T> &r) const{

    return !(*this).operator==(r);

}

template <typename T>
T & Array <T>::operator[](int i) const{
    if(i<0)
    {   
        cerr<<ERR_INVALID_INDEX<<endl;
        exit(1);
    }
    else
        return data[i];
}

template <typename T>
ostream & operator<< (ostream &os, Array<T> const&arr){
    size_t i;

    if(arr.size == 0)
        return os;
    for(i=0; i<arr.getSize() - 1; i++){
        os << arr[i] ;
    }
    os << arr[i];

    return os;
}

//  Método que recibe un dato u objeto y lo agrega al final del vector. Para esto, se solicita memoria para la cantidad de 
//elementos que se almacenarán en el vector de modo de reubicar los valores en dichas direcciones y 
//finalmente se libera la memoria utilizada previamente por el vector.

template <typename T>
void Array<T>::addValueEnd(T info){
    T * aux = new T[size+1];
    for (size_t i=0; i<size; i++){
        aux[i] = data[i];
    }
    size++;
    delete[] data;
    data = aux;

    data[size - 1] = info;
}

// Método que reinicia un array, es decir, lo reemplaza por uno vacio.

template <typename T>
void Array<T>::restartArray(){
    Array <T> aux(0);
	*this = aux;
}

#endif