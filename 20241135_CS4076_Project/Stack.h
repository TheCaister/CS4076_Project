#ifndef STACK_H
#define STACK_H

#include<vector>
#include<iostream>

using std::vector;

template <typename T>
class Stack{
private:
    vector<T> objectList;
public:
    Stack();
    Stack(vector<T> list);
    T peek();
    T pop();
    T push();
};

#endif // STACK_H
