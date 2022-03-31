#ifndef STACK_H
#define STACK_H

#include<vector>
#include<iostream>

using std::vector;

template <typename T>
class Stack{
private:
    vector<T> objectList;
    void setObjectList(vector<T>);
    int maxAmount = 10;
public:
    Stack(){};
    Stack(vector<T>);
    T peek();

    T pop(){
        T output = NULL;
        if(this->objectList.size() > 0){
            output = this->objectList.at(this->objectList.size() - 1);
            this->objectList.pop_back();
            return output;
        }
        return output;
    };

    T popDelete();
    void clear();

    void push(T& item){
        // If the size is exceeded, remove the bottom of the stack
        // and then add the new item
        if((int) this->objectList.size() <= this->maxAmount){
            this->objectList.push_back(item);
        }
        else{
            this->objectList.erase(this->objectList.begin());
            this->objectList.push_back(item);
        }
    };
};

#endif // STACK_H
