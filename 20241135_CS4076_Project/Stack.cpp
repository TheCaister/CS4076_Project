#include "Stack.h"
#include "Room.h"

//template <typename T>
//Stack<T>::Stack(){
//}

template <typename T>
Stack<T>::Stack(vector<T> list){
    this->objectList = list;
}

template <typename T>
T Stack<T>::peek(){
    if(this->objectList.size() > 0){
        return this->objectList.at(this->objectList.size() - 1);
    }
}

//Room* Stack<Room*>::pop(){
//    Room* output = NULL;
//    if(this->objectList.size() > 0){
//        output = this->objectList.at(this->objectList.size() - 1);
//        this->objectList.pop_back();
//        return output;
//    }
//    return output;
//}
//T Stack<T>::pop(){
//    T output = NULL;
//    if(this->objectList.size() > 0){
//        output = this->objectList.at(this->objectList.size() - 1);
//        this->objectList.pop_back();
//        return output;
//    }
//    return output;
//}

// For deleting off the heap
template <typename T>
T Stack<T>::popDelete(){
    T output;
    T newObjectList = this->objectList;
    if(this->objectList.size() > 0){
        output = newObjectList.at(newObjectList.size() - 1);
        delete newObjectList.at(newObjectList.size() - 1);
        this->objectList.pop_back();
        return output;
    }
    return NULL;
}

//template <typename T>
//void Stack<T>::push(T item){
//    this->objectList.push_back(item);
//}

template <typename T>
void Stack<T>::clear(){
    while(this->objectList.size() != 0){

    }
}
