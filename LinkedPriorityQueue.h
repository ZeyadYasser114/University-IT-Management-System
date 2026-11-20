#pragma once
#include <functional>
#include <iostream>
#include "Node.h"               // From Omar Abdou
#include "Ticket.h"             // From Youssef Haitham
#include "Technician.h"         // From Youssef Haitham

template <typename T>
class LinkedPriorityQueue
{
    protected:
        Node<T>* head;          // Front of the list, always the highest priority item
        int count;
        std::function<bool(const T&, const T&)> comp;               // Comparison function for priority ordering

    public:
        LinkedPriorityQueue(std::function<bool(const T&, const T&)> comparator);
        virtual ~LinkedPriorityQueue();

        bool enqueue(T item);
        bool dequeue(T& outItem);
        bool peek(T& outItem) const;
        bool isEmpty() const;
        int getCount() const;
        void print() const;
        void clear();
};

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue(std::function<bool(const T&, const T&)> comparator)
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
    outItem = head->item;
    return true;
}

template <typename T>
bool LinkedPriorityQueue<T>::dequeue(T& outItem)
{
    if (head == nullptr)
        return false;
    Node<T>* old = head;
    outItem = old->item;
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
bool LinkedPriorityQueue<T>::enqueue(T item)
{
    Node<T>* newNode = new Node<T>(item);

    // Empty queue, or the new item goes before the current head
    if (head == nullptr || comp(item, head->item))
    {
        newNode->next = head;
        head = newNode;
        count++;
        return true;
    }

    // Stop at the last node the new item must come after
    Node<T>* cur = head;
    while (cur->next != nullptr && !comp(item, cur->next->item))
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
        std::cout << cur->item << " ";
        cur = cur->next;
    }
    std::cout << std::endl;
}

// Waiting_UR: higher PR first, then earlier QT, then smaller RID
inline bool urOrder(Ticket* const& a, Ticket* const& b)
{
    if (a->pr != b->pr)
        return a->pr > b->pr;
    if (a->qt != b->qt)
        return a->qt < b->qt;
    return a->rid < b->rid;
}

// Free_ST and Free_GT: smallest TID first
inline bool freeTechOrder(Technician* const& a, Technician* const& b)
{
    return a->tid < b->tid;
}

// Completed_Tickets: earlier FT first, then smaller RID
inline bool completedOrder(Ticket* const& a, Ticket* const& b)
{
    if (a->ft != b->ft)
        return a->ft < b->ft;
    return a->rid < b->rid;
}