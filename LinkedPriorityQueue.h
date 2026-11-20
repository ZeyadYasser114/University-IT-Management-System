#pragma once
#include <iostream>
#include "Node.h"               // From Omar Abdou
#include "Ticket.h"             // From Youssef Haitham
#include "Technician.h"         // From Youssef Haitham

template <typename T>
class LinkedPriorityQueue
{
    public:
        typedef bool (*Comparator)(const T&, const T&);     // Returns true if the first argument must come before the second

    protected:
        Node<T>* head;          // Front of the list, always the highest priority item
        int count;
        Comparator comp;        // Comparison function for priority ordering

    public:
        LinkedPriorityQueue(Comparator comparator);
        virtual ~LinkedPriorityQueue();

        bool enqueue(T data);
        bool dequeue(T& outItem);
        bool peek(T& outItem) const;
        bool isEmpty() const;
        int getCount() const;
        void print() const;
        void clear();
};

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue(Comparator comparator)
{
    head = nullptr;
    count = 0;
    comp = comparator;
}

template <typename T>
LinkedPriorityQueue<T>::~LinkedPriorityQueue()
{
    clear();
}

template <typename T>
bool LinkedPriorityQueue<T>::isEmpty() const
{
    return head == nullptr;
}

template <typename T>
int LinkedPriorityQueue<T>::getCount() const
{
    return count;
}

template <typename T>
bool LinkedPriorityQueue<T>::peek(T& outItem) const
{
    if (head == nullptr)
        return false;
    outItem = head->data;
    return true;
}

template <typename T>
bool LinkedPriorityQueue<T>::dequeue(T& outItem)
{
    if (head == nullptr)
        return false;
    Node<T>* old = head;
    outItem = old->data;
    head = head->next;          // Move head first, then delete the old node
    delete old;                 // Deletes the node only, not the stored Ticket*/Technician*
    count--;
    return true;
}

template <typename T>
void LinkedPriorityQueue<T>::clear()
{
    Node<T>* cur = head;
    while (cur != nullptr)
    {
        Node<T>* nxt = cur->next;       // Save next before deleting cur
        delete cur;
        cur = nxt;
    }
    head = nullptr;
    count = 0;
}

template <typename T>
bool LinkedPriorityQueue<T>::enqueue(T data)
{
    Node<T>* newNode = new Node<T>(data);

    // Empty queue, or the new data goes before the current head
    if (head == nullptr || comp(data, head->data))
    {
        newNode->next = head;
        head = newNode;
        count++;
        return true;
    }

    // Stop at the last node the new data must come after
    Node<T>* cur = head;
    while (cur->next != nullptr && !comp(data, cur->next->data))
        cur = cur->next;

    newNode->next = cur->next;
    cur->next = newNode;
    count++;
    return true;
}

template <typename T>
void LinkedPriorityQueue<T>::print() const
{
    Node<T>* cur = head;
    while (cur != nullptr)
    {
        std::cout << *(cur->data) << " ";       // Needs operator<< on the pointed-to class
        cur = cur->next;
    }
    std::cout << std::endl;
}

// Waiting_UR: higher PR first, then earlier QT, then smaller RID
inline bool urOrder(Ticket* const& a, Ticket* const& b)
{
    if (a->getPR() != b->getPR())
        return a->getPR() > b->getPR();
    if (a->getQT() != b->getQT())
        return a->getQT() < b->getQT();
    return a->getRID() < b->getRID();
}

// Free_ST and Free_GT: smallest TID first
inline bool freeTechOrder(Technician* const& a, Technician* const& b)
{
    return a->getTID() < b->getTID();
}

// Completed_Tickets: earlier FT first, then smaller RID
inline bool completedOrder(Ticket* const& a, Ticket* const& b)
{
    if (a->getFT() != b->getFT())
        return a->getFT() < b->getFT();
    return a->getRID() < b->getRID();
}