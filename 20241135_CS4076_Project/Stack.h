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
public:
    Stack();
    Stack(vector<T>);
    T peek();
    T pop();
    T popDelete();
    void clear();
    void push(T);
};

#endif // STACK_H
