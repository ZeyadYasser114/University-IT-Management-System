// Omar Abdou
#include <iostream>
#include<string>
using namespace std;

template <class T>
class Node {
public:
    T data;
    Node<T>* next;

    Node(T value, Node<T>* ptr = nullptr) {
        data = value;
        next = ptr;
    }
};
