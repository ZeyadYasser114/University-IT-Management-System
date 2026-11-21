#pragma once
#include "LinkedQueue.h"
#include "Ticket.h"

class NRQueue : public LinkedQueue<Ticket*> {
public:
    Ticket* findByID(int id) {
        Node<Ticket*>* curr = front;
        while (curr != nullptr) {
            if (curr->data->getRID() == id) {
                return curr->data;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    Ticket* cancelByID(int id) {
        Node<Ticket*>* prev = nullptr;
        Node<Ticket*>* curr = front;

        while (curr != nullptr) {
            if (curr->data->getRID() == id) {
                Ticket* target = curr->data;

                if (curr == front) {
                    front = front->next;
                    if (front == nullptr) {
                        rear = nullptr;
                    }
                } else {
                    prev->next = curr->next;
                    if (curr == rear) {
                        rear = prev;
                    }
                }

                curr->next = nullptr;
                delete curr;
                curr = nullptr;
                count--;
                return target;
            }

            prev = curr;
            curr = curr->next;
        }

        return nullptr;
    }
};
